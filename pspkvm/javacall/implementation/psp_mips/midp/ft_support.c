#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_support.h"

/* 8x12 Basic ASCII Font Bitmap array */
static const uint8_t font_bits_8x12[128][12] = {
    [32] = {0,0,0,0,0,0,0,0,0,0,0,0}, /* Space */
    ['0'] = {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0,0,0,0,0},
    ['1'] = {0x0C,0x1C,0x0C,0x0C,0x0C,0x0C,0x3E,0,0,0,0,0},
    ['A'] = {0x1C,0x36,0x63,0x7F,0x63,0x63,0x63,0,0,0,0,0},
    ['B'] = {0x7E,0x63,0x63,0x7E,0x63,0x63,0x7E,0,0,0,0,0},
    ['C'] = {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0,0,0,0,0},
};

javacall_result ftc_javacall_font_set_font(javacall_font_face face, javacall_font_style style, javacall_font_size size) {
    (void)face; (void)style; (void)size;
    return JAVACALL_OK;
}

javacall_result ftc_javacall_font_get_info(javacall_font_face face, javacall_font_style style, javacall_font_size size, int* ascent, int* descent, int* leading) {
    (void)face; (void)style; (void)size;
    if (ascent) *ascent = 10;
    if (descent) *descent = 2;
    if (leading) *leading = 2;
    return JAVACALL_OK;
}

int ftc_javacall_font_get_width(javacall_font_face face, javacall_font_style style, javacall_font_size size, const javacall_utf16* charArray, int charArraySize) {
    (void)face; (void)style; (void)size;
    if (!charArray || charArraySize <= 0) return 0;
    return charArraySize * 8;
}

javacall_result ftc_javacall_font_draw(javacall_pixel color, int clipX1, int clipY1, int clipX2, int clipY2, javacall_pixel* destBuffer, int destBufferHoriz, int destBufferVert, int x, int y, const javacall_utf16* text, int textLen) {
    if (!destBuffer || !text || textLen <= 0) return JAVACALL_OK;

    int cur_x = x;
    for (int i = 0; i < textLen; i++) {
        uint16_t ch = text[i];
        if (ch >= 128) ch = '?';
        const uint8_t *bmp = font_bits_8x12[ch];

        for (int row = 0; row < 12; row++) {
            int py = y + row;
            if (py < clipY1 || py > clipY2 || py >= destBufferVert) continue;
            uint8_t bits = bmp[row];

            for (int col = 0; col < 8; col++) {
                int px = cur_x + col;
                if (px < clipX1 || px > clipX2 || px >= destBufferHoriz) continue;

                if (bits & (1 << (7 - col))) {
                    destBuffer[py * destBufferHoriz + px] = color;
                }
            }
        }
        cur_x += 8;
    }

    return JAVACALL_OK;
}
