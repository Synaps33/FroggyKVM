#ifndef _JVM_CONFIG_H_
#define _JVM_CONFIG_H_

#define ENABLE_PCSL 1
#define ENABLE_ISOLATES 1
#define MAX_ISOLATES 1
#define ENABLE_EXTENSIONS 1
#define JAVACALL_MIPS 1
#define MSW_FIRST_FOR_DOUBLE 0
#define ENABLE_EMBEDDED_CALLINFO 1
#define ENABLE_NATIVE_ORDER_REWRITING 0
#define ENABLE_BYTECODE_FLUSHING 0
#define ENABLE_MONET 0
#define PRODUCT 1
#define ENABLE_REFLECTION 0
#define ENABLE_ROM_GENERATOR 0
#define ENABLE_CPU_VARIANT 0
#define ENABLE_EXCESSIVE_GC 0
#define USE_EVENT_LOGGER 0
#define ENABLE_JVMPI 0
#define ENABLE_JAR_ENTRY_CACHE 0
#define USE_OPT_FORWARD_BRANCH 0
#define ENABLE_BRUTE_FORCE_ICACHE_FLUSH 0
#define ENABLE_FLOAT 1
#ifndef HARDWARE_LITTLE_ENDIAN
#define HARDWARE_LITTLE_ENDIAN 1
#endif
#ifndef MSW_FIRST_FOR_LONG
#define MSW_FIRST_FOR_LONG (!HARDWARE_LITTLE_ENDIAN)
#endif
#ifndef MSW_FIRST_FOR_DOUBLE
#define MSW_FIRST_FOR_DOUBLE (!HARDWARE_LITTLE_ENDIAN)
#endif
#ifndef TARGET_MSW_FIRST_FOR_LONG
#define TARGET_MSW_FIRST_FOR_LONG MSW_FIRST_FOR_LONG
#endif
#ifndef TARGET_MSW_FIRST_FOR_DOUBLE
#define TARGET_MSW_FIRST_FOR_DOUBLE MSW_FIRST_FOR_DOUBLE
#endif

#if ENABLE_ISOLATES
#define ISOLATES_PARAM(x) x,
#else
#define ISOLATES_PARAM(x)
#endif

#include <stdint.h>
#include "kni_md.h"

typedef uint16_t JvmPathChar;

typedef struct { int length; int elements[1]; } jint_array;
typedef struct { int length; short elements[1]; } jshort_array;
typedef struct { int length; signed char elements[1]; } jbyte_array;
typedef struct { int length; unsigned short elements[1]; } jchar_array;

struct Java_javax_microedition_lcdui_Image {
    short *imageData;
    int width;
    int height;
};

struct Java_javax_microedition_lcdui_Graphics {
    int transX;
    int transY;
    int clipX1;
    int clipY1;
    int clipX2;
    int clipY2;
    int pixel;
    struct Java_javax_microedition_lcdui_Image *img;
};

#endif /* _JVM_CONFIG_H_ */
