#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_support.h"
#include "font8x8.h"

javacall_result ftc_javacall_font_set_font(javacall_font_face face, javacall_font_style style, javacall_font_size size) {
    (void)face; (void)style; (void)size;
    return JAVACALL_OK;
}

javacall_result ftc_javacall_font_get_info(javacall_font_face face, javacall_font_style style, javacall_font_size size, int* ascent, int* descent, int* leading) {
    (void)face; (void)style; (void)size;
    if (ascent) *ascent = 7;
    if (descent) *descent = 1;
    if (leading) *leading = 0;
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
        const uint8_t *bmp;
        if (ch >= 32 && ch <= 126) {
            bmp = font8x8_basic[ch - 32];
        } else if (ch > 126) {
            bmp = font8x8_basic['?' - 32];
        } else {
            bmp = font8x8_basic[0]; /* Space */
        }

        for (int row = 0; row < 8; row++) {
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
