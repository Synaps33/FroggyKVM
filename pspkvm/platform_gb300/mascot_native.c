/*
 * Native replacement for the Mascot Capsule V3 triangle rasteriser.
 *
 * Why this exists
 * ---------------
 * The vendored MascotME rasteriser is pure Java running on top of our C
 * bytecode interpreter. Profiling showed the interpreter itself is fast
 * (~47 M bytecodes/s); the cost is that filling a 320x240 frame costs roughly
 * 100 bytecodes *per pixel*, i.e. ~50 ms per frame, which is 1-2 FPS on the
 * GB300. A micro-benchmark of the identical inner loop written in C and
 * reading the same Java arrays through KNI measured 0.8 ms per frame, i.e.
 * about 60x faster.
 *
 * KNI only offers per-element access, which looks fatal at first - but the
 * comparison above is exactly that: three KNI calls per pixel still beat
 * ~100 bytecodes per pixel by a wide margin, because the arithmetic stays in C
 * and never enters the interpreter dispatch loop.
 *
 * This file mirrors fillTriangleAffineT_replaceFast() from
 * com/mascotcapsule/micro3d/v3/Rasterizer.java exactly, including the
 * fixed-point arithmetic (fp = 12), the seam-avoiding rounding correction and
 * the clip clamping. The Java implementation is kept as a fallback and is used
 * whenever the native path reports itself unavailable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "psp_compat.h"
#include <kni.h>

#define M3D_FP  12
#define M3D_FP1 (1 << M3D_FP)

/* Returns 1 so the Java side knows the native path is present.
 * Set MASCOT_NATIVE=0 to force the Java fallback, which makes an A/B
 * performance comparison possible without rebuilding. */
KNIEXPORT KNI_RETURNTYPE_INT
Java_com_mascotcapsule_micro3d_v3_Rasterizer_nInit(void)
{
    const char *v = getenv("MASCOT_NATIVE");
    if (v != NULL && v[0] == '0') {
        return 0;
    }
    return 1;
}

/*
 * Port of fillTriangleAffineT_replaceFast:
 *   textured, affine mapping, REPLACE blending, 256-entry palette.
 *
 * Parameters are taken straight off the Java stack in declaration order.
 */
KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_mascotcapsule_micro3d_v3_Rasterizer_nFillAffineTReplaceFast(void)
{
    jint fbWidth   = KNI_GetParameterAsInt(2);
    jint clipX1    = KNI_GetParameterAsInt(3);
    jint clipX2    = KNI_GetParameterAsInt(4);
    jint yStart    = KNI_GetParameterAsInt(5);
    jint yEnd      = KNI_GetParameterAsInt(6);
    jint uStart    = KNI_GetParameterAsInt(7);
    jint duStart   = KNI_GetParameterAsInt(8);
    jint du        = KNI_GetParameterAsInt(9);
    jint vStart    = KNI_GetParameterAsInt(10);
    jint dvStart   = KNI_GetParameterAsInt(11);
    jint dv        = KNI_GetParameterAsInt(12);
    jint texWBit   = KNI_GetParameterAsInt(14);
    jint texLenMask= KNI_GetParameterAsInt(16);
    jint xStart    = KNI_GetParameterAsInt(17);
    jint dxStart   = KNI_GetParameterAsInt(18);
    jint xEnd      = KNI_GetParameterAsInt(19);
    jint dxEnd     = KNI_GetParameterAsInt(20);
    jint fbLen;

    KNI_StartHandles(3);
    KNI_DeclareHandle(frameBuffer);
    KNI_DeclareHandle(texBitmap);
    KNI_DeclareHandle(texPal);
    KNI_GetParameterAsObject(1, frameBuffer);
    KNI_GetParameterAsObject(13, texBitmap);
    KNI_GetParameterAsObject(15, texPal);

    if (frameBuffer && texBitmap && texPal) {
        /* Corrected rounding to avoid texture seams (same as the Java code). */
        if (duStart != 0) uStart += (duStart > 0) ? 1 : -1;
        if (dvStart != 0) vStart += (dvStart > 0) ? 1 : -1;

        fbLen = KNI_GetArrayLength(frameBuffer);

        yStart *= fbWidth;
        yEnd   *= fbWidth;

        for (; yStart < yEnd;
             yStart += fbWidth,
             uStart  += duStart, vStart += dvStart,
             xStart  += dxStart, xEnd   += dxEnd) {
            jint x1 = xStart >> M3D_FP;
            jint x2 = xEnd   >> M3D_FP;
            /* Subpixel precision, ceil rounding. */
            jint tempI = M3D_FP1 - (xStart & (M3D_FP1 - 1));
            jint u = uStart + ((du * tempI) >> M3D_FP);
            jint v = vStart + ((dv * tempI) >> M3D_FP);

            if (x1 < clipX1) {
                tempI = x1 - clipX1;
                u -= du * tempI;
                v -= dv * tempI;
                x1 = clipX1;
            }
            if (x2 > clipX2) x2 = clipX2;
            x1 += yStart;
            x2 += yStart;

            if (x1 < 0) x1 = 0;
            if (x2 > fbLen) x2 = fbLen;

            for (; x1 < x2; u += du, v += dv, x1++) {
                jint idx = ((((v >> M3D_FP) << texWBit) | (u >> M3D_FP))
                           & texLenMask);
                /* The Java code masks the *byte value* with 0xFF before it
                 * indexes the palette, not the texture index. Skipping that
                 * makes a signed jbyte negative and reads outside the palette,
                 * which shows up as garbled colours. */
                jint entry = ((jint)KNI_GetByteArrayElement(texBitmap, idx)) & 0xFF;
                KNI_SetIntArrayElement(frameBuffer, x1,
                    KNI_GetIntArrayElement(texPal, entry));
            }
        }
    }
    KNI_EndHandles();
}
