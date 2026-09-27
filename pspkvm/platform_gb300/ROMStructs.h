#ifndef _ROM_STRUCTS_H_
#define _ROM_STRUCTS_H_

#include <stdint.h>
#include "kni_md.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char jboolean;
typedef signed char jbyte;
typedef int jint;

typedef struct { void *klass; int length; int elements[1]; } jint_array;
typedef struct { void *klass; int length; short elements[1]; } jshort_array;
typedef struct { void *klass; int length; signed char elements[1]; } jbyte_array;
typedef struct { void *klass; int length; unsigned short elements[1]; } jchar_array;

struct Java_java_lang_Object {
    void * __do_not_use__;
};

struct Java_java_lang_String {
    void * __do_not_use__;
    jchar_array *value;
    int offset;
    int count;
};

struct Java_javax_microedition_lcdui_ImageData {
    void * __do_not_use__;
    int width;
    int height;
    jbyte_array *pixelData;
    jbyte_array *alphaData;
    int nativePixelData;
    int nativeAlphaData;
    jboolean isMutable;
    jbyte ___pad1;
    jbyte ___pad2;
    jbyte ___pad3;
};

struct Java_javax_microedition_lcdui_Image {
    void * __do_not_use__;
    int width;
    int height;
    struct Java_javax_microedition_lcdui_ImageData *imageData;
};

struct Java_javax_microedition_lcdui_Font {
    void * __do_not_use__;
    int face;
    int style;
    int size;
    int baseline;
    int height;
};

struct Java_javax_microedition_lcdui_Graphics {
    void * __do_not_use__;
    int transX;
    int transY;
    int ax;
    int ay;
    int rgbColor;
    int gray;
    int pixel;
    int style;
    struct Java_javax_microedition_lcdui_Font *currentFont;
    int displayId;
    struct Java_javax_microedition_lcdui_Image *img;
    void *creator;
    short clipX1;
    short clipY1;
    short clipX2;
    short clipY2;
    short systemClipX1;
    short systemClipY1;
    short systemClipX2;
    short systemClipY2;
    short maxWidth;
    short maxHeight;
    jboolean clipped;
    jboolean runtimeClipEnforce;
    jbyte ___pad1;
    jbyte ___pad2;
};

#ifdef __cplusplus
}
#endif

#endif /* _ROM_STRUCTS_H_ */
