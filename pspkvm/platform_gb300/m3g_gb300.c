/*
 * JSR-184 (Mobile 3D Graphics / M3G) software renderer for the GB300.
 *
 * The MIDP class library ships a complete javax.microedition.m3g API whose
 * data path ends in native methods.  Until now all of those natives were
 * declared as weak stubs returning 0, so Graphics3D's constructor always got
 * handle 0 from nBind(), left nativeBound == false and every render() call
 * was a silent no-op - games showed their 2D fallback only.
 *
 * This file implements the subset actually needed to put 3D on screen:
 *   - a handle registry for vertex arrays / vertex buffers / index-free
 *     triangle strips / appearances / materials / textures / cameras,
 *   - geometry and texture storage fed by the Java side (short[] / byte[]),
 *   - a camera, a viewport, a z-buffer,
 *   - a textured, depth-tested triangle rasteriser.
 *
 * The API objects hand us most data directly (positions and texture
 * coordinates arrive as short[], texture pixels as byte[], transforms as
 * float[]), so no KNI is needed for the geometry itself.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "psp_compat.h"
#include <kni.h>

/* the MIDP system screen buffer: 16-bit RGB565, one array for the whole
 * 320x240 screen.  Declared in jcapp_export.c. */
#include "gxj_putpixel.h"
extern gxj_screen_buffer gxj_system_screen_buffer;


/* ------------------------------------------------------------------ */
/* debug tracing                                                       */

static int m3g_trace = -1;

/* Master switch. The rasteriser draws, but the transform chain is not
 * correct yet: geometry lands outside the viewport, which would overwrite a
 * game's 2D fallback with a black screen. Until that is fixed the module
 * stays inert by default; set M3G_ENABLE=1 to try it. */
static int m3g_enabled = -1;

static int m3g_on(void) {
    if (m3g_enabled < 0) {
        m3g_enabled = (getenv("M3G_ENABLE") != NULL) ? 1 : 0;
    }
    return m3g_enabled;
}
static int m3g_dump = 2;      /* how many render calls to dump */
static int m3g_rendercalls = 0;

static int m3g_debug(void) {
    if (m3g_trace < 0) {
        m3g_trace = (getenv("M3G_TRACE") != NULL) ? 1 : 0;
    }
    return m3g_trace;
}

#define M3G_LOG(...)                     \
    do {                                 \
        if (m3g_debug()) {               \
            printf(__VA_ARGS__);          \
            fflush(stdout);               \
        }                                \
    } while (0)

/* ------------------------------------------------------------------ */
/* handle registry                                                     */

#define M3G_MAX_OBJECTS 512

typedef enum {
    OBJ_FREE = 0,
    OBJ_VERTEX_ARRAY,
     OBJ_VERTEX_BUFFER,
    OBJ_APPEARANCE,
    OBJ_MATERIAL,
    OBJ_COMPOSITING_MODE,
    OBJ_POLYGON_MODE,
    OBJ_TEXTURE,
    OBJ_IMAGE,
    OBJ_CAMERA,
    OBJ_BACKGROUND
} m3g_objtype_t;

typedef struct m3g_object m3g_object_t;

/* M3G vertex array formats (javax.microedition.m3g.VertexArray) */
#define M3G_VA_BYTE    0
#define M3G_VA_SHORT   1
#define M3G_VA_FLOAT   2

struct m3g_object {
    m3g_objtype_t type;
    int            refcount;
    union {
        struct {                   /* OBJ_VERTEX_ARRAY */
            int     numVertices;
            int     numComponents;  /* 3 for positions, 2 for texcoords */
            int     componentSize;  /* bytes per component: 2 = short */
            int     componentType;  /* M3G_VA_* */
            int     count;          /* stored component values */
            short  *sh;             /* M3G_VA_SHORT data */
            signed char *by;        /* M3G_VA_BYTE data */
        } va;
        struct {                   /* OBJ_VERTEX_BUFFER */
            int vaPositions;        /* handle, -1 = none */
            int vaTexCoords[8];     /* handles, -1 = none */
            float posScale;
            float posBias[3];
            float texScale[8];
            float texBias[8][3];
            int   vertexCount;
        } vb;
        struct {                   /* OBJ_APPEARANCE */
            int material;           /* handle, -1 = none */
            int compositingMode;    /* handle, -1 = none */
            int polygonMode;        /* handle, -1 = none */
            int background;         /* handle, -1 = none */
            int texture[8];         /* handles, -1 = none */
        } app;
        struct {                   /* OBJ_MATERIAL */
            int color;
            int ambient;
            int emissive;
            int specular;
            int shininess;
            int vertexColorTracking;
        } mat;
        struct {                   /* OBJ_COMPOSITING_MODE */
            int blending;
            int alphaThreshold;
            int depthTest;
            int depthWrite;
            int colorWrite;
            int alphaWrite;
            int depthOffsetZNear;
            int depthOffsetZFar;
        } comp;
        struct {                   /* OBJ_POLYGON_MODE */
            int culling;
            int shading;
            int winding;
            int twoSidedLighting;
            int localCameraLighting;
            int perspectiveCorrection;
        } poly;
        struct {                   /* OBJ_TEXTURE */
            int image;             /* handle, -1 = none */
            int blending;
            int filterMode;         /* nearest / linear */
            int wrapS, wrapT;
            int blendColor;
        } tex;
        struct {                   /* OBJ_IMAGE */
            int     width;
            int     height;
            int     format;
            unsigned char *rgb;    /* 3 bytes per pixel, 0xRRGGBB */
        } img;
        struct {                   /* OBJ_CAMERA */
            int   projection;       /* 0 perspective, 1 parallel, 2 generic */
            float params[16];
        } cam;
        struct {                   /* OBJ_BACKGROUND */
            int color;
        } bg;
    } u;
};

static m3g_object_t g_m3g_objects[M3G_MAX_OBJECTS];
static int g_m3g_next_handle = 1;

static m3g_object_t *m3g_get(int handle) {
    if (handle <= 0 || handle >= M3G_MAX_OBJECTS) {
        return NULL;
    }
    if (g_m3g_objects[handle].type == OBJ_FREE) {
        return NULL;
    }
    return &g_m3g_objects[handle];
}

static int m3g_alloc(m3g_objtype_t type) {
    int i;
    for (i = 1; i < M3G_MAX_OBJECTS; i++) {
        if (g_m3g_objects[i].type == OBJ_FREE) {
            memset(&g_m3g_objects[i], 0, sizeof(m3g_object_t));
            g_m3g_objects[i].type = type;
            g_m3g_objects[i].refcount = 1;
            return i;
        }
    }
    M3G_LOG("[M3G] out of handles\n");
    return 0;
}

static void m3g_free_handle(int handle) {
    m3g_object_t *o = m3g_get(handle);
    if (!o) {
        return;
    }
    switch (o->type) {
    case OBJ_VERTEX_ARRAY:
        free(o->u.va.sh);
        free(o->u.va.by);
        break;
    case OBJ_IMAGE:
        free(o->u.img.rgb);
        break;
    default:
        break;
    }
    o->type = OBJ_FREE;
}

static void m3g_release(int handle) {
    m3g_object_t *o = m3g_get(handle);
    if (!o) {
        return;
    }
    if (--o->refcount <= 0) {
        m3g_free_handle(handle);
    }
}

/* ------------------------------------------------------------------ */
/* render context                                                      */

typedef struct {
    int active;
    int target;             /* KNI handle of the bound MIDP image */
    int width;
    int height;
    int vpX, vpY, vpW, vpH;
    int clipX, clipY, clipW, clipH;
    float zNear, zFar;
    int handle;             /* our own "nBind" handle */

    unsigned int *color;    /* width*height, 0x00RRGGBB */
    float *depth;           /* width*height, 1.0 = far */
} m3g_ctx_t;

static m3g_ctx_t g_m3g;

static void m3g_blit_to_screen(void) {
    int x, y;
    gxj_pixel_type *dst = gxj_system_screen_buffer.pixelData;
    int dw = gxj_system_screen_buffer.width;
    int dh = gxj_system_screen_buffer.height;

    if (!dst || dw <= 0 || dh <= 0 || !g_m3g.color) {
        return;
    }
    for (y = 0; y < g_m3g.height && y < dh; y++) {
        for (x = 0; x < g_m3g.width && x < dw; x++) {
            unsigned int c = g_m3g.color[y * g_m3g.width + x];
            int r = (int)((c >> 16) & 0xFF);
            int gg = (int)((c >> 8) & 0xFF);
            int b = (int)(c & 0xFF);
            dst[y * dw + x] = (gxj_pixel_type)(((r & 0xF8) << 8) |
                                                ((gg & 0xFC) << 3) |
                                                 (b >> 3));
        }
    }
}
static int g_m3g_cam_handle = 0;
static float g_m3g_cam_world[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

static void m3g_free_targets(void) {
    free(g_m3g.color);
    free(g_m3g.depth);
    g_m3g.color = NULL;
    g_m3g.depth = NULL;
}

static int m3g_alloc_targets(int w, int h) {
    size_t n;
    if (w <= 0 || h <= 0 || w > 1024 || h > 1024) {
        return 0;
    }
    m3g_free_targets();
    n = (size_t)w * (size_t)h;
    g_m3g.color = (unsigned int *)calloc(n, sizeof(unsigned int));
    g_m3g.depth = (float *)calloc(n, sizeof(float));
    if (!g_m3g.color || !g_m3g.depth) {
        m3g_free_targets();
        return 0;
    }
    g_m3g.width = w;
    g_m3g.height = h;
    return 1;
}

/* ------------------------------------------------------------------ */
/* small 4x4 matrix helpers (M3G uses row-major float[16])            */

typedef struct { float m[16]; } m3g_mat_t;

static void mat_identity(m3g_mat_t *r) {
    int i;
    for (i = 0; i < 16; i++) {
        r->m[i] = 0.0f;
    }
    r->m[0] = r->m[5] = r->m[10] = r->m[15] = 1.0f;
}

static void mat_mul(m3g_mat_t *r, const m3g_mat_t *a, const m3g_mat_t *b) {
    m3g_mat_t t;
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float sum = 0.0f;
            for (k = 0; k < 4; k++) {
                sum += a->m[i * 4 + k] * b->m[k * 4 + j];
            }
            t.m[i * 4 + j] = sum;
        }
    }
    *r = t;
}

static void mat_from_rows(m3g_mat_t *r, const float *rows) {
    int i;
    if (!rows) {
        mat_identity(r);
        return;
    }
    for (i = 0; i < 16; i++) {
        r->m[i] = rows[i];
    }
}

static void mat_translate(m3g_mat_t *r, float x, float y, float z) {
    mat_identity(r);
    r->m[3] = x;
    r->m[7] = y;
    r->m[11] = z;
}

static void mat_transform_point(const m3g_mat_t *m, float x, float y, float z,
                                float *ox, float *oy, float *oz, float *ow) {
    *ox = m->m[0] * x + m->m[1] * y + m->m[2] * z + m->m[3];
    *oy = m->m[4] * x + m->m[5] * y + m->m[6] * z + m->m[7];
    *oz = m->m[8] * x + m->m[9] * y + m->m[10] * z + m->m[11];
    *ow = m->m[12] * x + m->m[13] * y + m->m[14] * z + m->m[15];
}

static void mat_transform_vec(const m3g_mat_t *m, float x, float y, float z,
                              float *ox, float *oy, float *oz) {
    *ox = m->m[0] * x + m->m[1] * y + m->m[2] * z;
    *oy = m->m[4] * x + m->m[5] * y + m->m[6] * z;
    *oz = m->m[8] * x + m->m[9] * y + m->m[10] * z;
}

/* Perspective projection from a symmetric frustum, then to screen space. */
static void mat_perspective(m3g_mat_t *r, float fovy, float aspect,
                            float zNear, float zFar) {
    float f, dz;
    float t = (float)tan(fovy * 0.5);
    if (t == 0.0f) {
        t = 0.0001f;
    }
    f = 1.0f / t;
    dz = zNear - zFar;
    if (dz == 0.0f) {
        dz = -0.0001f;
    }
    memset(r->m, 0, sizeof(r->m));
    r->m[0]  = f / aspect;
    r->m[5]  = f;
    r->m[10] = (zFar + zNear) / dz;
    r->m[11] = -1.0f;
    r->m[14] = (2.0f * zFar * zNear) / dz;
}

/* Orthographic projection. */
static void mat_parallel(m3g_mat_t *r, float height, float aspect,
                         float zNear, float zFar) {
    float w = height * aspect;
    float dz = zFar - zNear;
    if (dz == 0.0f) {
        dz = 0.0001f;
    }
    memset(r->m, 0, sizeof(r->m));
    r->m[0]  = 2.0f / w;
    r->m[5]  = 2.0f / height;
    r->m[10] = -2.0f / dz;
    r->m[14] = -(zFar + zNear) / dz;
    r->m[15] = 1.0f;
}

/* The view matrix is the inverse of the camera transform. */
static void mat_invert_rigid(m3g_mat_t *r, const m3g_mat_t *m) {
    /* Only the rotation part is inverted; translation is negated. */
    r->m[0]  = m->m[0]; r->m[1]  = m->m[4]; r->m[2]  = m->m[8];
    r->m[4]  = m->m[1]; r->m[5]  = m->m[5]; r->m[6]  = m->m[9];
    r->m[8]  = m->m[2]; r->m[9]  = m->m[6]; r->m[10] = m->m[10];

    r->m[3]  = -(r->m[0] * m->m[3] + r->m[1] * m->m[7] + r->m[2] * m->m[11]);
    r->m[7]  = -(r->m[4] * m->m[3] + r->m[5] * m->m[7] + r->m[6] * m->m[11]);
    r->m[11] = -(r->m[8] * m->m[3] + r->m[9] * m->m[7] + r->m[10] * m->m[11]);
    r->m[12] = 0.0f; r->m[13] = 0.0f; r->m[14] = 0.0f; r->m[15] = 1.0f;
}

/* ------------------------------------------------------------------ */
/* render primitives                                                   */

typedef struct {
    float x, y, z;      /* screen x, screen y, ndc z */
    float u, v;
    int   culled;
} m3g_vtx_t;

static int clampi(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static unsigned int sample_tex(const m3g_object_t *tex, float u, float v) {
    m3g_object_t *img;
    int x, y;
    const unsigned char *p;
    int tw, th;

    if (!tex || tex->u.tex.image <= 0) {
        return 0xFFFFFF;
    }
    img = m3g_get(tex->u.tex.image);
    if (!img || img->type != OBJ_IMAGE || !img->u.img.rgb) {
        return 0xFFFFFF;
    }
    tw = img->u.img.width;
    th = img->u.img.height;
    if (tw <= 0 || th <= 0) {
        return 0xFFFFFF;
    }

    /* wrapping */
    if (tex->u.tex.wrapS) {
        u = u - (float)(int)floorf(u);
    } else {
        if (u < 0.0f) u = 0.0f;
        if (u > 1.0f) u = 1.0f;
    }
    if (tex->u.tex.wrapT) {
        v = v - (float)(int)floorf(v);
    } else {
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;
    }

    if (tex->u.tex.filterMode) {
        /* linear */
        float fx = u * tw - 0.5f;
        float fy = v * th - 0.5f;
        int x0 = (int)floorf(fx), y0 = (int)floorf(fy);
        float ax = fx - x0, ay = fy - y0;
        int xi[2], yi[2], i, j;
        float acc[3] = {0, 0, 0}, wsum = 0.0f;

        xi[0] = x0; xi[1] = x0 + 1;
        yi[0] = y0; yi[1] = y0 + 1;
        for (j = 0; j < 2; j++) {
            float wy = j ? ay : (1.0f - ay);
            int yy = yi[j];
            if (yy < 0) yy = 0;
            if (yy >= th) yy = th - 1;
            for (i = 0; i < 2; i++) {
                float wx = i ? ax : (1.0f - ax);
                int xx = xi[i];
                if (xx < 0) xx = 0;
                if (xx >= tw) xx = tw - 1;
                p = img->u.img.rgb + ((size_t)yy * tw + xx) * 3;
                acc[0] += p[0] * wx * wy;
                acc[1] += p[1] * wx * wy;
                acc[2] += p[2] * wx * wy;
                wsum += wx * wy;
            }
        }
        if (wsum <= 0.0f) wsum = 1.0f;
        {
            int r = (int)(acc[0] / wsum + 0.5f);
            int g = (int)(acc[1] / wsum + 0.5f);
            int b = (int)(acc[2] / wsum + 0.5f);
            return (unsigned int)((clampi(r, 0, 255) << 16) |
                                   (clampi(g, 0, 255) << 8) |
                                    clampi(b, 0, 255));
        }
    } else {
        /* nearest */
        x = (int)(u * tw);
        y = (int)(v * th);
        x = clampi(x, 0, tw - 1);
        y = clampi(y, 0, th - 1);
        p = img->u.img.rgb + ((size_t)y * tw + x) * 3;
        return (unsigned int)((p[0] << 16) | (p[1] << 8) | p[2]);
    }
}

static void put_pixel(int x, int y, unsigned int rgb, float z, int blend,
                      int colorWrite, int depthWrite, int depthTest) {
    int idx;
    float zmin, zmax;

    if (x < g_m3g.vpX || x >= g_m3g.vpX + g_m3g.vpW) return;
    if (y < g_m3g.vpY || y >= g_m3g.vpY + g_m3g.vpH) return;
    if (x < g_m3g.clipX || x >= g_m3g.clipX + g_m3g.clipW) return;
    if (y < g_m3g.clipY || y >= g_m3g.clipY + g_m3g.clipH) return;

    idx = y * g_m3g.width + x;

    zmin = 0.0f;
    zmax = 1.0f;
    if (depthTest && z >= zmin && z <= zmax) {
        if (z >= g_m3g.depth[idx]) {
            return;             /* occluded by something closer */
        }
    }
    if (depthWrite) {
        g_m3g.depth[idx] = z;
    }
    if (!colorWrite) {
        return;
    }
    if (blend) {
        unsigned int dst = g_m3g.color[idx];
        int dr = (int)((dst >> 16) & 0xFF);
        int dg = (int)((dst >> 8) & 0xFF);
        int db = (int)(dst & 0xFF);
        int sr = (int)((rgb >> 16) & 0xFF);
        int sg = (int)((rgb >> 8) & 0xFF);
        int sb = (int)(rgb & 0xFF);
        g_m3g.color[idx] = (unsigned int)((clampi((dr + sr) / 2, 0, 255) << 16) |
                                           (clampi((dg + sg) / 2, 0, 255) << 8) |
                                            clampi((db + sb) / 2, 0, 255));
    } else {
        g_m3g.color[idx] = rgb & 0xFFFFFFu;
    }
}

static void raster_triangle(const m3g_vtx_t *v0, const m3g_vtx_t *v1,
                            const m3g_vtx_t *v2, const m3g_object_t *tex,
                            unsigned int baseColor, int blend, int depthTest,
                            int depthWrite, int colorWrite) {
    int minx, maxx, miny, maxy, x, y;
    float area, ax, ay, bx, by, cx, cy;
    float vpx = v0->x, vpy = v0->y;
    float w0x = v1->x, w0y = v1->y;
    float w1x = v2->x, w1y = v2->y;

    area = (w0x - vpx) * (w1y - vpy) - (w1x - vpx) * (w0y - vpy);
    if (area == 0.0f) {
        return;
    }
    if (area < 0.0f) {
        /* keep winding consistent; culling is handled separately */
        area = -area;
    }

    ax = v0->x; ay = v0->y; bx = v1->x; by = v1->y; cx = v2->x; cy = v2->y;

    minx = (int)floorf(fminf(fminf(ax, bx), cx));
    maxx = (int)ceilf (fmaxf(fmaxf(ax, bx), cx));
    miny = (int)floorf(fminf(fminf(ay, by), cy));
    maxy = (int)ceilf (fmaxf(fmaxf(ay, by), cy));

    minx = clampi(minx, g_m3g.vpX, g_m3g.vpX + g_m3g.vpW - 1);
    maxx = clampi(maxx, g_m3g.vpX, g_m3g.vpX + g_m3g.vpW - 1);
    miny = clampi(miny, g_m3g.vpY, g_m3g.vpY + g_m3g.vpH - 1);
    maxy = clampi(maxy, g_m3g.vpY, g_m3g.vpY + g_m3g.vpH - 1);

    for (y = miny; y <= maxy; y++) {
        float py = (float)y + 0.5f;
        for (x = minx; x <= maxx; x++) {
            float px = (float)x + 0.5f;
            float w0 = ((bx - px) * (cy - py) - (cx - px) * (by - py)) / area;
            float w1 = ((cx - px) * (ay - py) - (ax - px) * (cy - py)) / area;
            float w2 = 1.0f - w0 - w1;
            float z, u, v;
            unsigned int rgb;

            if (w0 < -0.001f || w1 < -0.001f || w2 < -0.001f) {
                continue;
            }
            z = v0->z * w2 + v1->z * w0 + v2->z * w1;
            u  = v0->u * w2 + v1->u * w0 + v2->u * w1;
            v  = v0->v * w2 + v1->v * w0 + v2->v * w1;

            rgb = tex ? sample_tex(tex, u, v) : baseColor;
            put_pixel(x, y, rgb, z, blend, colorWrite, depthWrite, depthTest);
        }
    }
}

/* ------------------------------------------------------------------ */
/* geometry gather + object constructor                               */

static int m3g_new(m3g_objtype_t t) {
    m3g_object_t *o = m3g_get(m3g_alloc(t));
    if (!o) {
        return 0;
    }
    if (t == OBJ_APPEARANCE) {
        int i;
        o->u.app.material = -1;
        o->u.app.compositingMode = -1;
        o->u.app.polygonMode = -1;
        o->u.app.background = -1;
        for (i = 0; i < 8; i++) {
            o->u.app.texture[i] = -1;
        }
    } else if (t == OBJ_COMPOSITING_MODE) {
        o->u.comp.blending = 2;      /* REPLACE */
        o->u.comp.depthTest = 1;
        o->u.comp.depthWrite = 1;
        o->u.comp.colorWrite = 1;
        o->u.comp.alphaWrite = 1;
        o->u.comp.alphaThreshold = 64;
    } else if (t == OBJ_POLYGON_MODE) {
        o->u.poly.culling = 2;       /* BACK */
        o->u.poly.shading = 1;       /* SMOOTH */
        o->u.poly.winding = 0;
        o->u.poly.perspectiveCorrection = 1;
    } else if (t == OBJ_MATERIAL) {
        o->u.mat.color = 0xFFFFFFFF;
    } else if (t == OBJ_TEXTURE) {
        o->u.tex.image = -1;
        o->u.tex.blending = 2;
        o->u.tex.filterMode = 0;
        o->u.tex.wrapS = 1;
        o->u.tex.wrapT = 1;
        o->u.tex.blendColor = 0;
    }
    return (int)(o - g_m3g_objects);
}

static int m3g_gather_positions(int vbHandle, float **outPos) {
    m3g_object_t *vb = m3g_get(vbHandle);
    m3g_object_t *va;
    int vcount, nc, stride, i;
    float *pos;

    if (!vb || vb->type != OBJ_VERTEX_BUFFER) {
        return 0;
    }
    if (vb->u.vb.vaPositions <= 0) {
        return 0;
    }
    va = m3g_get(vb->u.vb.vaPositions);
    if (!va || va->type != OBJ_VERTEX_ARRAY || !va->u.va.sh) {
        return 0;
    }
    vcount = va->u.va.numVertices;
    nc = va->u.va.numComponents;
    if (nc < 3) {
        nc = 3;
    }
    if (vcount <= 0) {
        return 0;
    }
    stride = nc;
    pos = (float *)malloc(sizeof(float) * (size_t)vcount * 3);
    if (!pos) {
        return 0;
    }
    for (i = 0; i < vcount; i++) {
        const short *sh = va->u.va.sh + (size_t)i * stride;
        pos[i * 3 + 0] = (float)sh[0] * vb->u.vb.posScale + vb->u.vb.posBias[0];
        pos[i * 3 + 1] = (float)sh[1] * vb->u.vb.posScale + vb->u.vb.posBias[1];
        pos[i * 3 + 2] = (float)sh[2] * vb->u.vb.posScale + vb->u.vb.posBias[2];
    }
    *outPos = pos;
    return vcount;
}

static int m3g_gather_texcoords(int vbHandle, int unit, float **outUV) {
    m3g_object_t *vb = m3g_get(vbHandle);
    m3g_object_t *va;
    int i, vcount, stride;
    float *uv;

    if (!vb || vb->type != OBJ_VERTEX_BUFFER) {
        return 0;
    }
    if (unit < 0 || unit >= 8 || vb->u.vb.vaTexCoords[unit] <= 0) {
        return 0;
    }
    va = m3g_get(vb->u.vb.vaTexCoords[unit]);
    if (!va || va->type != OBJ_VERTEX_ARRAY || !va->u.va.sh) {
        return 0;
    }
    vcount = va->u.va.numVertices;
    stride = va->u.va.numComponents;
    if (stride < 2) {
        stride = 1;
    }
    if (vcount <= 0) {
        return 0;
    }
    uv = (float *)malloc(sizeof(float) * (size_t)vcount * 2);
    if (!uv) {
        return 0;
    }
    for (i = 0; i < vcount; i++) {
        const short *sh = va->u.va.sh + (size_t)i * stride;
        uv[i * 2 + 0] = (float)sh[0] * vb->u.vb.texScale[unit] + vb->u.vb.texBias[unit][0];
        uv[i * 2 + 1] = (float)sh[1] * vb->u.vb.texScale[unit] + vb->u.vb.texBias[unit][1];
    }
    *outUV = uv;
    return vcount;
}

/* ------------------------------------------------------------------ */
/* Graphics3D                                                          */

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nBind(void) {
    /* bindTarget() calls nBind(target, hints, depthBufferEnabled); the
     * target's real size comes from the width/height that the game passes to
     * setViewport(), so fall back to the 320x240 MIDP screen. */
    jint hints = KNI_GetParameterAsInt(2);
    jint depth = KNI_GetParameterAsInt(3);
    int w = 320, h = 240;

    m3g_free_targets();
    memset(&g_m3g, 0, sizeof(g_m3g));
    g_m3g.width = w;
    g_m3g.height = h;
    g_m3g.vpX = 0; g_m3g.vpY = 0;
    g_m3g.vpW = w; g_m3g.vpH = h;
    g_m3g.clipX = 0; g_m3g.clipY = 0;
    g_m3g.clipW = w; g_m3g.clipH = h;
    g_m3g.zNear = 1.0f;
    g_m3g.zFar = 100.0f;
    if (!m3g_alloc_targets(w, h)) {
        M3G_LOG("[M3G] nBind failed for %dx%d\n", w, h);
        return 0;
    }
    g_m3g.handle = g_m3g_next_handle++;
    M3G_LOG("[M3G] nBind %dx%d hints=0x%x depth=%d -> handle %d\n",
            w, h, (int)hints, (int)depth, g_m3g.handle);
    return g_m3g.handle;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nRelease(void) {


    m3g_free_targets();
    g_m3g.active = 0;
    return 0;
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Graphics3D_nSetViewport(void) {
    jint x = KNI_GetParameterAsInt(1);
    jint y = KNI_GetParameterAsInt(2);
    jint w = KNI_GetParameterAsInt(3);
    jint h = KNI_GetParameterAsInt(4);

    M3G_LOG("[M3G] viewport %d,%d %dx%d\n", (int)x, (int)y, (int)w, (int)h);
    g_m3g.vpX = (int)x; g_m3g.vpY = (int)y;
    g_m3g.vpW = (int)w; g_m3g.vpH = (int)h;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Graphics3D_nSetClipRect(void) {
    jint x = KNI_GetParameterAsInt(1);
    jint y = KNI_GetParameterAsInt(2);
    jint w = KNI_GetParameterAsInt(3);
    jint h = KNI_GetParameterAsInt(4);

    g_m3g.clipX = (int)x; g_m3g.clipY = (int)y;
    g_m3g.clipW = (int)w; g_m3g.clipH = (int)h;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Graphics3D_nSetDepthRange(void) {
    jfloat zn = KNI_GetParameterAsFloat(1);
    jfloat zf = KNI_GetParameterAsFloat(2);

    g_m3g.zNear = zn;
    g_m3g.zFar = zf;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nClear(void) {
    jint mask = KNI_GetParameterAsInt(1);

    size_t n = (size_t)g_m3g.width * (size_t)g_m3g.height, i;
    if ((mask & 0x1) && g_m3g.depth) {
        for (i = 0; i < n; i++) {
            g_m3g.depth[i] = 1.0f;
        }
    }
    if ((mask & 0x2) && g_m3g.color) {
        for (i = 0; i < n; i++) {
            g_m3g.color[i] = 0u;
        }
    }
    return 1;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nSetCamera(void) {
    jint cameraHandle = KNI_GetParameterAsInt(1);

    m3g_object_t *cam = m3g_get(cameraHandle);
    int ret = 0;

    if (cam && cam->type == OBJ_CAMERA) {
        /* applyCamera() passes the camera's world transform as float[16] rows. */
        KNI_StartHandles(1);
        KNI_DeclareHandle(params);
        KNI_GetParameterAsObject(2, params);
        if (params) {
            jsize n = KNI_GetArrayLength(params);
            int i;
            for (i = 0; i < n && i < 16; i++) {
                g_m3g_cam_world[i] = KNI_GetFloatArrayElement(params, i);
            }
            M3G_LOG("[M3G] setCamera world tx=%.1f,%.1f,%.1f\n",
                    g_m3g_cam_world[3], g_m3g_cam_world[7], g_m3g_cam_world[11]);
        }
        ret = 1;
        KNI_EndHandles();
    }
    return ret;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nAddLight(void) {
    jint lightHandle = KNI_GetParameterAsInt(1);

    (void)lightHandle;
    return 0;
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Graphics3D_nClearLights(void) {


    KNI_ReturnVoid();
}

/* ------------------------------------------------------------------ */
/* the actual draw                                                     */

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Graphics3D_nRenderImmediate(void) {
    jint vbHandle = KNI_GetParameterAsInt(1);
    jint ibHandle = KNI_GetParameterAsInt(2);
    jint appHandle = KNI_GetParameterAsInt(3);
    jint subMeshIndex = KNI_GetParameterAsInt(4);

    m3g_object_t *app = m3g_get(appHandle);
    m3g_object_t *cam;
    m3g_object_t *mat = NULL;
    m3g_object_t *comp = NULL;
    m3g_object_t *poly = NULL;
    m3g_object_t *tex = NULL;
    unsigned int baseColor = 0xFFFFFFu;
    int blend = 0, depthTest = 1, depthWrite = 1, colorWrite = 1;
    m3g_mat_t model, view, proj, mvp;
    m3g_mat_t camWorld;
    float *pos = NULL, *uv = NULL;
    int vcount, uvcount, i, j;
    m3g_vtx_t *vtx = NULL;
    m3g_mat_t objXform, mv2;

    (void)ibHandle;
    (void)subMeshIndex;

    if (!m3g_on()) {
        return 0;               /* inert: leave the game's 2D output alone */
    }
    if (!g_m3g.color || !g_m3g.depth) {
        return 0;
    }
    M3G_LOG("[M3G] nRenderImmediate vb=%d ib=%d app=%d\n",
            (int)vbHandle, (int)ibHandle, (int)appHandle);
    vcount = m3g_gather_positions(vbHandle, &pos);
    if (vcount <= 0) {
        M3G_LOG("[M3G] render: no positions for vb=%d\n", (int)vbHandle);
        return 0;
    }
    uvcount = m3g_gather_texcoords(vbHandle, 0, &uv);

    vtx = (m3g_vtx_t *)calloc(vcount, sizeof(m3g_vtx_t));
    if (!vtx) {
        free(pos);
        free(uv);
        return 0;
    }

    m3g_rendercalls++;
    if (m3g_rendercalls <= m3g_dump) {
        int r;
        printf("[M3G-X] call %d: vcount=%d uv=%d app=%d\n",
               m3g_rendercalls, vcount, uvcount, (int)appHandle);
        printf("[M3G-X] objXform rows:");
        for (r = 0; r < 4; r++) {
            printf(" [%.2f %.2f %.2f %.2f]", objXform.m[r*4], objXform.m[r*4+1],
                   objXform.m[r*4+2], objXform.m[r*4+3]);
        }
        printf("\n[M3G-X] camWorld rows:");
        for (r = 0; r < 4; r++) {
            printf(" [%.2f %.2f %.2f %.2f]", camWorld.m[r*4], camWorld.m[r*4+1],
                   camWorld.m[r*4+2], camWorld.m[r*4+3]);
        }
        printf("\n[M3G-X] MVP rows:");
        for (r = 0; r < 4; r++) {
            printf(" [%.4f %.4f %.4f %.4f]", mvp.m[r*4], mvp.m[r*4+1],
                   mvp.m[r*4+2], mvp.m[r*4+3]);
        }
        printf("\n[M3G-X] raw pos[0..3] (x,y,z):");
        for (r = 0; r < 4 && r < vcount; r++) {
            printf(" (%.1f,%.1f,%.1f)", pos[r*3], pos[r*3+1], pos[r*3+2]);
        }
        printf("\n");
        fflush(stdout);
    }

    if (app && app->type == OBJ_APPEARANCE) {
        mat  = m3g_get(app->u.app.material);
        comp = m3g_get(app->u.app.compositingMode);
        poly = m3g_get(app->u.app.polygonMode);
        tex  = m3g_get(app->u.app.texture[0]);
    }
    if (mat && mat->type == OBJ_MATERIAL && mat->u.mat.color) {
        baseColor = (unsigned int)(mat->u.mat.color & 0xFFFFFF);
    }
    if (comp && comp->type == OBJ_COMPOSITING_MODE) {
        blend      = comp->u.comp.blending;
        depthTest  = comp->u.comp.depthTest;
        depthWrite = comp->u.comp.depthWrite;
        colorWrite = comp->u.comp.colorWrite;
    }

    /* the object's transform is passed as float[16] rows; the camera's world
     * transform as float[16] in parameter 5. */
    /* parameter 4 is the object transform; the camera's world transform was
     * captured earlier in nSetCamera (parameter 5 here is an int index). */
    KNI_StartHandles(1);
    KNI_DeclareHandle(rows);

    mat_identity(&objXform);
    KNI_GetParameterAsObject(4, rows);
    if (rows) {
        jsize n = KNI_GetArrayLength(rows);
        int r;
        for (r = 0; r < 16 && r < n; r++) {
            objXform.m[r] = KNI_GetFloatArrayElement(rows, r);
        }
    }

    mat_identity(&camWorld);
    cam = m3g_get(g_m3g_cam_handle);
    {
        int r;
        for (r = 0; r < 16; r++) {
            camWorld.m[r] = g_m3g_cam_world[r];
        }
    }
    mat_invert_rigid(&view, &camWorld);

    /* projection */
    {
        /* The camera's own near/far plane matters: the game works in a world
         * of thousands of units (nSetPerspective reported near=40 far=5500),
         * so using our default 1..100 clipped every triangle away. */
        float aspect = (g_m3g.vpH > 0 && g_m3g.vpW > 0)
                     ? (float)g_m3g.vpW / (float)g_m3g.vpH : 1.0f;
        float fovy = 1.0472f;          /* 60 degrees, a sane default */
        float zn = g_m3g.zNear;
        float zf = g_m3g.zFar;
        int   parallel = 0;
        if (cam && cam->type == OBJ_CAMERA) {
            if (cam->u.cam.projection == 1) {
                parallel = 1;
            }
            /* this game passes the field of view in DEGREES (45), while the
             * M3G spec uses radians, so accept both */
            if (cam->u.cam.params[0] > 0.01f) {
                fovy = (cam->u.cam.params[0] > 3.2f)
                     ? (float)(cam->u.cam.params[0] * 3.14159265 / 180.0)
                     : cam->u.cam.params[0];
            }
            if (cam->u.cam.params[2] > 0.0f) {
                zn = cam->u.cam.params[2];
            }
            if (cam->u.cam.params[3] > zn) {
                zf = cam->u.cam.params[3];
            }
        }
        if (parallel) {
            mat_parallel(&proj, fovy, aspect, zn, zf);
        } else {
            mat_perspective(&proj, fovy, aspect, zn, zf);
        }
    }

    /* MVP = projection * view * object */
    mat_mul(&mv2, &view, &objXform);
    mat_mul(&mvp, &proj, &mv2);

    for (i = 0; i < vcount; i++) {
        float cx, cy, cz, cw;
        mat_transform_point(&mvp, pos[i * 3], pos[i * 3 + 1], pos[i * 3 + 2],
                            &cx, &cy, &cz, &cw);
        if (cw <= 0.0001f || cw < 0.0f) {
            vtx[i].culled = 1;
            continue;
        }
        cx /= cw; cy /= cw; cz /= cw;
        vtx[i].x = g_m3g.vpX + (cx * 0.5f + 0.5f) * g_m3g.vpW;
        vtx[i].y = g_m3g.vpY + (cy * 0.5f + 0.5f) * g_m3g.vpH;
        vtx[i].z = cz;
        if (uv && i < uvcount) {
            vtx[i].u = uv[i * 2];
            vtx[i].v = uv[i * 2 + 1];
        }
    }

    /* Consecutive vertices form a triangle strip. */
    for (i = 0; i + 2 < vcount; i += 2) {
        int a = i, b = i + 1, c = i + 2;
        if (vtx[a].culled || vtx[b].culled || vtx[c].culled) {
            continue;
        }
        if ((i & 2) == 0) {
            raster_triangle(&vtx[a], &vtx[b], &vtx[c], tex, baseColor, blend,
                            depthTest, depthWrite, colorWrite);
        } else {
            raster_triangle(&vtx[a], &vtx[c], &vtx[b], tex, baseColor, blend,
                            depthTest, depthWrite, colorWrite);
        }
    }
    (void)poly; (void)j;

    m3g_blit_to_screen();
    KNI_EndHandles();
    free(vtx);
    free(pos);
    free(uv);
    return 1;
}

/* ------------------------------------------------------------------ */
/* VertexArray / VertexBuffer                                          */

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_VertexArray_nCreate(void) {
    /* VertexArray.createDeferred() calls
     *     nCreate(numVertices, numComponents, componentSize)
     * where componentSize is the size of one component in BYTES. */
    jint numVertices   = KNI_GetParameterAsInt(1);
    jint numComponents = KNI_GetParameterAsInt(2);
    jint componentSize = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(m3g_alloc(OBJ_VERTEX_ARRAY));
    int total;
    if (!o) {
        return 0;
    }
    o->u.va.numVertices   = (int)numVertices;
    o->u.va.numComponents = (int)numComponents;
    o->u.va.componentSize = (int)componentSize;
    o->u.va.componentType = (componentSize == 2) ? M3G_VA_SHORT
                       : (componentSize == 4) ? M3G_VA_FLOAT : M3G_VA_BYTE;
    total = (int)numVertices * ((int)numComponents > 0 ? (int)numComponents : 1);
    if (total < 0 || total > 65536) {
        total = (int)numVertices * 4;
    }
    o->u.va.count = total;
    M3G_LOG("[M3G] VertexArray nCreate v=%d comps=%d size=%dB (%s) -> handle %d\n",
            (int)numVertices, (int)numComponents, (int)componentSize,
            o->u.va.componentType == M3G_VA_SHORT ? "short"
            : (o->u.va.componentType == M3G_VA_FLOAT ? "float" : "byte"),
            (int)(o - g_m3g_objects));
    return (jint)(o - g_m3g_objects);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexArray_nSetShort(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint offset = KNI_GetParameterAsInt(2);
    jint count = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(handle);
    jsize n;
    int stride, total, start, i;

    KNI_StartHandles(1);
    KNI_DeclareHandle(data);
    KNI_GetParameterAsObject(4, data);

    if (o && o->type == OBJ_VERTEX_ARRAY && data) {
        n = KNI_GetArrayLength(data);
        if (n < count) {
            count = n;
        }
        stride = o->u.va.numComponents;
        if (stride < 1) {
            stride = 1;
        }
        total = o->u.va.numVertices * stride;
        if (o->u.va.count < total) {
            total = o->u.va.count;
        }
        if (!o->u.va.sh) {
            o->u.va.sh = (short *)calloc(total > 0 ? total : 1, sizeof(short));
        }
        if (o->u.va.sh) {
            start = (int)offset / 2;
            for (i = 0; i < (int)count; i++) {
                int idx = start + i;
                if (idx >= 0 && idx < total) {
                    o->u.va.sh[idx] = KNI_GetShortArrayElement(data, i);
                }
            }
            M3G_LOG("[M3G] VertexArray nSetShort h=%d off=%d count=%d stride=%d total=%d\n",
                    (int)handle, (int)offset, (int)count, stride, total);
        }
    }
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexArray_nSetByte(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint offset = KNI_GetParameterAsInt(2);
    jint count = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(handle);
    jsize n;
    int total, i;

    KNI_StartHandles(1);
    KNI_DeclareHandle(data);
    KNI_GetParameterAsObject(4, data);

    if (o && o->type == OBJ_VERTEX_ARRAY && data) {
        n = KNI_GetArrayLength(data);
        if (n < count) {
            count = n;
        }
        total = o->u.va.numVertices * (o->u.va.numComponents > 0
                                       ? o->u.va.numComponents : 1);
        if (!o->u.va.by) {
            o->u.va.by = (signed char *)calloc(total > 0 ? total : 1, 1);
        }
        if (o->u.va.by) {
            for (i = 0; i < (int)count; i++) {
                int idx = (int)offset + i;
                if (idx >= 0 && idx < total) {
                    o->u.va.by[idx] = KNI_GetByteArrayElement(data, i);
                }
            }
        }
    }
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_VertexBuffer_nCreate(void) {


    m3g_object_t *o;
    int i;
    o = m3g_get(m3g_alloc(OBJ_VERTEX_BUFFER));
    if (!o) {
        return 0;
    }
    o->u.vb.vaPositions = -1;
    for (i = 0; i < 8; i++) {
        o->u.vb.vaTexCoords[i] = -1;
        o->u.vb.texScale[i] = 1.0f;
    }
    o->u.vb.posScale = 1.0f;
    return (jint)(o - g_m3g_objects);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexBuffer_nSetPositions(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint vertexArray = KNI_GetParameterAsInt(2);
    jfloat scale = KNI_GetParameterAsFloat(3);

    m3g_object_t *o = m3g_get(handle);

    KNI_StartHandles(1);
    KNI_DeclareHandle(bias);
    KNI_GetParameterAsObject(4, bias);

    if (o && o->type == OBJ_VERTEX_BUFFER) {
        o->u.vb.vaPositions = (int)vertexArray;
        o->u.vb.posScale = scale;
        if (bias) {
            jsize n = KNI_GetArrayLength(bias);
            int i;
            for (i = 0; i < n && i < 3; i++) {
                o->u.vb.posBias[i] = KNI_GetFloatArrayElement(bias, i);
            }
        }
        M3G_LOG("[M3G] VertexBuffer nSetPositions h=%d va=%d scale=%.5f bias=%.4f,%.4f,%.4f\n",
                (int)handle, (int)vertexArray, scale, o->u.vb.posBias[0],
                o->u.vb.posBias[1], o->u.vb.posBias[2]);
    }
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexBuffer_nSetTexCoords(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint unit = KNI_GetParameterAsInt(2);
    jint vertexArray = KNI_GetParameterAsInt(3);
    jfloat scale = KNI_GetParameterAsFloat(4);

    m3g_object_t *o = m3g_get(handle);

    KNI_StartHandles(1);
    KNI_DeclareHandle(bias);
    KNI_GetParameterAsObject(5, bias);

    if (o && o->type == OBJ_VERTEX_BUFFER && unit >= 0 && unit < 8) {
        o->u.vb.vaTexCoords[unit] = (int)vertexArray;
        o->u.vb.texScale[unit] = scale;
        if (bias) {
            jsize n = KNI_GetArrayLength(bias);
            int i;
            for (i = 0; i < n && i < 2; i++) {
                o->u.vb.texBias[unit][i] = KNI_GetFloatArrayElement(bias, i);
            }
        }
        M3G_LOG("[M3G] VertexBuffer nSetTexCoords h=%d unit=%d va=%d scale=%.5f\n",
                (int)handle, (int)unit, (int)vertexArray, scale);
    }
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_VertexBuffer_nGetVertexCount(void) {
    jint handle = KNI_GetParameterAsInt(1);

    m3g_object_t *vb = m3g_get(handle);
    m3g_object_t *va;
    if (!vb || vb->type != OBJ_VERTEX_BUFFER) {
        return 0;
    }
    va = m3g_get(vb->u.vb.vaPositions);
    if (va && va->type == OBJ_VERTEX_ARRAY) {
        vb->u.vb.vertexCount = va->u.va.numVertices;
    }
    return vb->u.vb.vertexCount;
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexBuffer_nSetNormals(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint vertexArray = KNI_GetParameterAsInt(2);

    (void)handle; (void)vertexArray;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexBuffer_nSetColors(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint vertexArray = KNI_GetParameterAsInt(2);

    (void)handle; (void)vertexArray;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_VertexBuffer_nSetDefaultColor(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint color = KNI_GetParameterAsInt(2);

    (void)handle; (void)color;
    KNI_ReturnVoid();
}

/* ------------------------------------------------------------------ */
/* Appearance and friends                                              */

KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_Appearance_nCreate(void) {

 return m3g_new(OBJ_APPEARANCE); }
KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_Material_nCreate(void) {

 return m3g_new(OBJ_MATERIAL); }
KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_CompositingMode_nCreate(void) {

 return m3g_new(OBJ_COMPOSITING_MODE); }
KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_PolygonMode_nCreate(void) {

 return m3g_new(OBJ_POLYGON_MODE); }
KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_Camera_nCreate(void) {

 return m3g_new(OBJ_CAMERA); }
KNIEXPORT KNI_RETURNTYPE_INT Java_javax_microedition_m3g_Background_nCreate(void) {

 return m3g_new(OBJ_BACKGROUND); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetMaterial(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_APPEARANCE) { o->u.app.material = (int)m; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetCompositingMode(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_APPEARANCE) { o->u.app.compositingMode = (int)m; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetPolygonMode(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_APPEARANCE) { o->u.app.polygonMode = (int)m; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetFog(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);

    (void)h; (void)m;
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetTexture(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint unit = KNI_GetParameterAsInt(2);
    jint t = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_APPEARANCE && unit >= 0 && unit < 8) {
        o->u.app.texture[unit] = (int)t;
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Appearance_nSetLayer(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint l = KNI_GetParameterAsInt(2);
 (void)h; (void)l; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetMaterial(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_APPEARANCE) ? o->u.app.material : -1;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetCompositingMode(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_APPEARANCE) ? o->u.app.compositingMode : -1;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetPolygonMode(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_APPEARANCE) ? o->u.app.polygonMode : -1;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetFog(void) {
    jint h = KNI_GetParameterAsInt(1);
 (void)h; return -1; }

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetTexture(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint unit = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_APPEARANCE && unit >= 0 && unit < 8) {
        return o->u.app.texture[unit];
    }
    return -1;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Appearance_nGetLayer(void) {
    jint h = KNI_GetParameterAsInt(1);
 (void)h; return 0; }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Material_nSetColor(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint col = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_MATERIAL) { o->u.mat.color = (int)col; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Material_nGetColor(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint which = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    (void)which;
    if (o && o->type == OBJ_MATERIAL) { return o->u.mat.color; }
    return 0xFFFFFFFF;
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Material_nSetShininess(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat s = KNI_GetParameterAsFloat(2);
 (void)h; (void)s; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Material_nSetVertexColorTracking(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint v = KNI_GetParameterAsInt(2);
 (void)h; (void)v; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nSetBlending(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint b = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_COMPOSITING_MODE) { o->u.comp.blending = (int)b; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nEnableDepthTest(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint en = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_COMPOSITING_MODE) { o->u.comp.depthTest = en ? 1 : 0; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nEnableDepthWrite(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint en = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_COMPOSITING_MODE) { o->u.comp.depthWrite = en ? 1 : 0; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nEnableColorWrite(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint en = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_COMPOSITING_MODE) { o->u.comp.colorWrite = en ? 1 : 0; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nEnableAlphaWrite(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint en = KNI_GetParameterAsInt(2);
 (void)h; (void)en; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nSetAlphaThreshold(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat t = KNI_GetParameterAsFloat(2);
 (void)h; (void)t; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_CompositingMode_nSetDepthOffset(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat n = KNI_GetParameterAsFloat(2);
    jfloat f = KNI_GetParameterAsFloat(3);
 (void)h; (void)n; (void)f; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetCulling(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint cu = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_POLYGON_MODE) { o->u.poly.culling = (int)cu; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetShading(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint sh = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_POLYGON_MODE) { o->u.poly.shading = (int)sh; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetWinding(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint w = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_POLYGON_MODE) { o->u.poly.winding = (int)w; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetTwoSidedLighting(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint v = KNI_GetParameterAsInt(2);
 (void)h; (void)v; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetLocalCameraLighting(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint v = KNI_GetParameterAsInt(2);
 (void)h; (void)v; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_PolygonMode_nSetPerspectiveCorrection(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint v = KNI_GetParameterAsInt(2);
 (void)h; (void)v; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Background_nSetColor(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint col = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_BACKGROUND) { o->u.bg.color = (int)col; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Background_nSetImage(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint img = KNI_GetParameterAsInt(2);
 (void)h; (void)img; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Background_nSetImageMode(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);
    jint v = KNI_GetParameterAsInt(3);
 (void)h; (void)m; (void)v; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Background_nSetCrop(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint a = KNI_GetParameterAsInt(2);
    jint b = KNI_GetParameterAsInt(3);
    jint cc = KNI_GetParameterAsInt(4);
    jint d = KNI_GetParameterAsInt(5);
 (void)h; (void)a; (void)b; (void)cc; (void)d; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Background_nSetEnable(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint m = KNI_GetParameterAsInt(2);
    jint v = KNI_GetParameterAsInt(3);
 (void)h; (void)m; (void)v; KNI_ReturnVoid(); }

/* ------------------------------------------------------------------ */
/* Camera                                                              */

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Camera_nSetPerspective(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat fovy = KNI_GetParameterAsFloat(2);
    jfloat aspect = KNI_GetParameterAsFloat(3);
    jfloat zNear = KNI_GetParameterAsFloat(4);
    jfloat zFar = KNI_GetParameterAsFloat(5);

    m3g_object_t *o = m3g_get(h);
    g_m3g_cam_handle = (int)h;
    if (o && o->type == OBJ_CAMERA) {
        o->u.cam.projection = 0;
        o->u.cam.params[0] = fovy;
        o->u.cam.params[1] = aspect;
        o->u.cam.params[2] = zNear;
        o->u.cam.params[3] = zFar;
        M3G_LOG("[M3G] camera perspective fovy=%.3f aspect=%.3f near=%.1f far=%.1f\n",
                fovy, aspect, zNear, zFar);
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Camera_nSetParallel(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat height = KNI_GetParameterAsFloat(2);
    jfloat aspect = KNI_GetParameterAsFloat(3);
    jfloat zNear = KNI_GetParameterAsFloat(4);
    jfloat zFar = KNI_GetParameterAsFloat(5);

    m3g_object_t *o = m3g_get(h);
    g_m3g_cam_handle = (int)h;
    if (o && o->type == OBJ_CAMERA) {
        o->u.cam.projection = 1;
        o->u.cam.params[0] = height;
        o->u.cam.params[1] = aspect;
        o->u.cam.params[2] = zNear;
        o->u.cam.params[3] = zFar;
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Camera_nSetGeneric(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_CAMERA) { o->u.cam.projection = 2; }
    KNI_ReturnVoid();
}

/* ------------------------------------------------------------------ */
/* Transformable: the transform is normally passed explicitly to      */
/* render(), so only identity defaults are needed here.                */

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nSetTransform(void) {
    jint h = KNI_GetParameterAsInt(1);
 (void)h; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nGetTransform(void) {
    jint h = KNI_GetParameterAsInt(1);

    KNI_StartHandles(1);
    KNI_DeclareHandle(out);
    KNI_GetParameterAsObject(2, out);
    if (out) {
        jsize n = KNI_GetArrayLength(out);
        int i;
        float ident[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
        for (i = 0; i < n && i < 16; i++) {
            KNI_SetFloatArrayElement(out, i, ident[i]);
        }
    }
    (void)h;
    KNI_EndHandles();
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nGetCompositeTransform(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    KNI_StartHandles(1);
    KNI_DeclareHandle(out);
    KNI_GetParameterAsObject(2, out);
    if (out) {
        jsize n = KNI_GetArrayLength(out);
        int i;
        float ident[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
        for (i = 0; i < n && i < 16; i++) {
            KNI_SetFloatArrayElement(out, i, ident[i]);
        }
    }
    (void)o;
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nSetTranslation(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat x = KNI_GetParameterAsFloat(2);
    jfloat y = KNI_GetParameterAsFloat(3);
    jfloat z = KNI_GetParameterAsFloat(4);
 (void)h; (void)x; (void)y; (void)z; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nTranslate(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat x = KNI_GetParameterAsFloat(2);
    jfloat y = KNI_GetParameterAsFloat(3);
    jfloat z = KNI_GetParameterAsFloat(4);
 (void)h; (void)x; (void)y; (void)z; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nSetScale(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat x = KNI_GetParameterAsFloat(2);
    jfloat y = KNI_GetParameterAsFloat(3);
    jfloat z = KNI_GetParameterAsFloat(4);
 (void)h; (void)x; (void)y; (void)z; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nScale(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat x = KNI_GetParameterAsFloat(2);
    jfloat y = KNI_GetParameterAsFloat(3);
    jfloat z = KNI_GetParameterAsFloat(4);
 (void)h; (void)x; (void)y; (void)z; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nSetOrientation(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat a = KNI_GetParameterAsFloat(2);
    jfloat b = KNI_GetParameterAsFloat(3);
    jfloat c = KNI_GetParameterAsFloat(4);
    jfloat d = KNI_GetParameterAsFloat(5);
 (void)h; (void)a; (void)b; (void)c; (void)d; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nPreRotate(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat a = KNI_GetParameterAsFloat(2);
    jfloat b = KNI_GetParameterAsFloat(3);
    jfloat c = KNI_GetParameterAsFloat(4);
    jfloat d = KNI_GetParameterAsFloat(5);
 (void)h; (void)a; (void)b; (void)c; (void)d; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nPostRotate(void) {
    jint h = KNI_GetParameterAsInt(1);
    jfloat a = KNI_GetParameterAsFloat(2);
    jfloat b = KNI_GetParameterAsFloat(3);
    jfloat c = KNI_GetParameterAsFloat(4);
    jfloat d = KNI_GetParameterAsFloat(5);
 (void)h; (void)a; (void)b; (void)c; (void)d; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nGetTranslation(void) {
    jint h = KNI_GetParameterAsInt(1);

    KNI_StartHandles(1);
    KNI_DeclareHandle(out);
    KNI_GetParameterAsObject(2, out);
    if (out) {
        jsize n = KNI_GetArrayLength(out);
        int i;
        for (i = 0; i < n && i < 3; i++) {
            KNI_SetFloatArrayElement(out, i, 0.0f);
        }
    }
    (void)h;
    KNI_EndHandles();
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nGetScale(void) {
    jint h = KNI_GetParameterAsInt(1);

    KNI_StartHandles(1);
    KNI_DeclareHandle(out);
    KNI_GetParameterAsObject(2, out);
    if (out) {
        jsize n = KNI_GetArrayLength(out);
        int i;
        for (i = 0; i < n && i < 3; i++) {
            KNI_SetFloatArrayElement(out, i, 1.0f);
        }
    }
    (void)h;
    KNI_EndHandles();
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Transformable_nGetOrientation(void) {
    jint h = KNI_GetParameterAsInt(1);

    KNI_StartHandles(1);
    KNI_DeclareHandle(out);
    KNI_GetParameterAsObject(2, out);
    if (out) {
        jsize n = KNI_GetArrayLength(out);
        int i;
        for (i = 0; i < n && i < 4; i++) {
            KNI_SetFloatArrayElement(out, i, (i == 3) ? 1.0f : 0.0f);
        }
    }
    (void)h;
    KNI_EndHandles();
    KNI_ReturnVoid();
}

/* ------------------------------------------------------------------ */
/* Texture2D / Image2D                                                 */

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Image2D_nCreate(void) {
    jint w = KNI_GetParameterAsInt(1);
    jint h = KNI_GetParameterAsInt(2);
    jint format = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(m3g_alloc(OBJ_IMAGE));
    if (!o) {
        return 0;
    }
    o->u.img.width  = (int)w;
    o->u.img.height = (int)h;
    o->u.img.format = (int)format;
    M3G_LOG("[M3G] Image2D nCreate %dx%d format=%d\n", (int)w, (int)h, (int)format);
    return (jint)(o - g_m3g_objects);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Image2D_nSetImage(void) {
    jint handle = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(handle);
    jsize n;
    int w, h, fmt, bpp, x, y;

    KNI_StartHandles(1);
    KNI_DeclareHandle(data);
    KNI_GetParameterAsObject(2, data);

    if (o && o->type == OBJ_IMAGE && data) {
        n = KNI_GetArrayLength(data);
        w = o->u.img.width;
        h = o->u.img.height;
        fmt = o->u.img.format;
        if (n > 0 && w > 0 && h > 0) {
            bpp = (fmt == 2 || fmt == 4 || fmt == 5) ? 3 : 1;
            free(o->u.img.rgb);
            o->u.img.rgb = (unsigned char *)calloc((size_t)w * h * 3, 1);
            if (o->u.img.rgb) {
                for (y = 0; y < h; y++) {
                    for (x = 0; x < w; x++) {
                        jsize si = (jsize)((y * w + x) * bpp);
                        size_t di = (size_t)(y * w + x) * 3;
                        if (si >= n) {
                            break;
                        }
                        if (bpp == 3) {
                            o->u.img.rgb[di + 0] = (unsigned char)KNI_GetByteArrayElement(data, si);
                            o->u.img.rgb[di + 1] = (unsigned char)KNI_GetByteArrayElement(data, si + 1);
                            o->u.img.rgb[di + 2] = (unsigned char)KNI_GetByteArrayElement(data, si + 2);
                        } else {
                            unsigned char g = (unsigned char)KNI_GetByteArrayElement(data, si);
                            o->u.img.rgb[di + 0] = g;
                            o->u.img.rgb[di + 1] = g;
                            o->u.img.rgb[di + 2] = g;
                        }
                    }
                }
                M3G_LOG("[M3G] Image2D nSetImage %dx%d fmt=%d bytes=%d\n", w, h, fmt, (int)n);
            }
        }
    }
    KNI_EndHandles();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Image2D_nSetPalette(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint unused = KNI_GetParameterAsInt(2);
    jint p = KNI_GetParameterAsInt(3);
 (void)h; (void)unused; (void)p; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Image2D_nSetSubImage(void) {
    jint h = KNI_GetParameterAsInt(1);
 (void)h; KNI_ReturnVoid(); }
KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Image2D_nCommit(void) {
    jint h = KNI_GetParameterAsInt(1);
 (void)h; KNI_ReturnVoid(); }

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Image2D_nGetWidth(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_IMAGE) ? o->u.img.width : 0;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Image2D_nGetHeight(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_IMAGE) ? o->u.img.height : 0;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Image2D_nGetFormat(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_IMAGE) ? o->u.img.format : 0;
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Texture2D_nCreate(void) {
    jint blending = KNI_GetParameterAsInt(1);

    int hnd = m3g_new(OBJ_TEXTURE);
    m3g_object_t *o = m3g_get(hnd);
    if (o && o->type == OBJ_TEXTURE) {
        o->u.tex.blending = (int)blending;
    }
    return hnd;
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Texture2D_nSetImage(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint img = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_TEXTURE) {
        o->u.tex.image = (int)img;
        M3G_LOG("[M3G] Texture2D nSetImage tex=%d -> image=%d\n", (int)h, (int)img);
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Texture2D_nSetFiltering(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint mag = KNI_GetParameterAsInt(2);
    jint min = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_TEXTURE) {
        o->u.tex.filterMode = (mag == 2 || min == 2) ? 1 : 0;
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Texture2D_nSetWrapping(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint ws = KNI_GetParameterAsInt(2);
    jint wt = KNI_GetParameterAsInt(3);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_TEXTURE) {
        o->u.tex.wrapS = (ws == 0) ? 1 : 0;
        o->u.tex.wrapT = (wt == 0) ? 1 : 0;
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Texture2D_nSetBlending(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint b = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_TEXTURE) { o->u.tex.blending = (int)b; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_javax_microedition_m3g_Texture2D_nSetBlendColor(void) {
    jint h = KNI_GetParameterAsInt(1);
    jint c = KNI_GetParameterAsInt(2);

    m3g_object_t *o = m3g_get(h);
    if (o && o->type == OBJ_TEXTURE) { o->u.tex.blendColor = (int)c; }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_m3g_Texture2D_nGetImage(void) {
    jint h = KNI_GetParameterAsInt(1);

    m3g_object_t *o = m3g_get(h);
    return (o && o->type == OBJ_TEXTURE) ? o->u.tex.image : -1;
}
