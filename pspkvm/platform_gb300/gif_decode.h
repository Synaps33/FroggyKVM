#ifndef FROGGY_GIF_DECODE_H
#define FROGGY_GIF_DECODE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool froggy_decode_gif(const unsigned char *data, int len,
                       int target_w, int target_h,
                       uint16_t *dst_pixels, unsigned char *dst_alpha,
                       int *out_w, int *out_h);

#ifdef __cplusplus
}
#endif

#endif
