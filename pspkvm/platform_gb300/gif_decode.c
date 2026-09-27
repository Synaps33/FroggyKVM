#define STBI_ONLY_GIF
#define STB_IMAGE_IMPLEMENTATION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "stb_image.h"
#pragma GCC diagnostic pop

#include "gif_decode.h"
#include <string.h>

bool froggy_decode_gif(const unsigned char *data, int len,
                       int target_w, int target_h,
                       uint16_t *dst_pixels, unsigned char *dst_alpha,
                       int *out_w, int *out_h) {
    if (!data || len <= 0 || !dst_pixels) return false;

    int w = 0, h = 0, comp = 0;
    unsigned char *rgba = stbi_load_from_memory(data, len, &w, &h, &comp, 4);
    if (!rgba) return false;

    if (out_w) *out_w = w;
    if (out_h) *out_h = h;

    int copy_w = (target_w > 0 && target_w < w) ? target_w : w;
    int copy_h = (target_h > 0 && target_h < h) ? target_h : h;
    int total_dst = target_w * target_h;

    memset(dst_pixels, 0, total_dst * sizeof(uint16_t));
    if (dst_alpha) memset(dst_alpha, 0, total_dst);

    bool has_alpha = false;
    for (int y = 0; y < copy_h; y++) {
        for (int x = 0; x < copy_w; x++) {
            int src_idx = (y * w + x) * 4;
            int dst_idx = y * target_w + x;
            unsigned char r = rgba[src_idx + 0];
            unsigned char g = rgba[src_idx + 1];
            unsigned char b = rgba[src_idx + 2];
            unsigned char a = rgba[src_idx + 3];

            uint16_t p = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
            dst_pixels[dst_idx] = p;
            if (dst_alpha) {
                dst_alpha[dst_idx] = a;
            }
            if (a != 255) {
                has_alpha = true;
            }
        }
    }

    stbi_image_free(rgba);
    return has_alpha;
}
