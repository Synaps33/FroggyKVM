/* Generated from src/fluid_config.cmake for the GB300 build (no CMake). */
#ifndef _FLUID_CONFIG_H
#define _FLUID_CONFIG_H

#define DEBUG 0
#define VERSION "1.2.2"
/* The GB300 is little endian, so WORDS_BIGENDIAN stays undefined. */
#define SF3_DISABLED 0
#define SF3_XIPH_VORBIS 1
#define SF3_STB_VORBIS 2
#define SF3_SUPPORT SF3_DISABLED
/* double samples: cheaper on this target than float+soft-float conversions */
#define HAVE_STRING_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDARG_H 1
#define HAVE_MATH_H 1
#define HAVE_LIMITS_H 1
#define HAVE_FCNTL_H 1

#endif /* _FLUID_CONFIG_H */
