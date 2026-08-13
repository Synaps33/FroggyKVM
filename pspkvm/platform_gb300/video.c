#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libretro.h"
#include "psp_compat.h"

#define GB300_SCREEN_WIDTH  320
#define GB300_SCREEN_HEIGHT 240

static uint16_t gb300_framebuffer[GB300_SCREEN_WIDTH * GB300_SCREEN_HEIGHT] __attribute__((aligned(16)));
static int frame_skip_counter = 0;
static int frame_skip_target = 0; // 0 = no skip

void gb300_video_init(void) {
    memset(gb300_framebuffer, 0, sizeof(gb300_framebuffer));
}

uint16_t* gb300_video_get_framebuffer(void) {
    return gb300_framebuffer;
}

void gb300_video_set_frameskip(int skip) {
    frame_skip_target = skip;
}

int gb300_video_should_skip(void) {
    if (frame_skip_target <= 0) return 0;
    frame_skip_counter++;
    if (frame_skip_counter > frame_skip_target) {
        frame_skip_counter = 0;
        return 0;
    }
    return 1;
}

extern retro_video_refresh_t gb300_get_video_cb(void);

/* Flushes RGB565 source buffer into GB300 framebuffer, centering if dimensions differ */
void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch) {
    if (!src || gb300_video_should_skip()) return;

    if (src_w == GB300_SCREEN_WIDTH && src_h == GB300_SCREEN_HEIGHT && src_pitch == (int)(GB300_SCREEN_WIDTH * sizeof(uint16_t))) {
        memcpy(gb300_framebuffer, src, sizeof(gb300_framebuffer));
    } else {
        int dst_x = (GB300_SCREEN_WIDTH - src_w) / 2;
        int dst_y = (GB300_SCREEN_HEIGHT - src_h) / 2;

        if (dst_x < 0) dst_x = 0;
        if (dst_y < 0) dst_y = 0;

        int copy_w = (src_w > GB300_SCREEN_WIDTH) ? GB300_SCREEN_WIDTH : src_w;
        int copy_h = (src_h > GB300_SCREEN_HEIGHT) ? GB300_SCREEN_HEIGHT : src_h;

        /* Clear margins if rendering smaller viewport */
        if (copy_w < GB300_SCREEN_WIDTH || copy_h < GB300_SCREEN_HEIGHT) {
            memset(gb300_framebuffer, 0, sizeof(gb300_framebuffer));
        }

        for (int y = 0; y < copy_h; y++) {
            uint16_t *dst_line = &gb300_framebuffer[(dst_y + y) * GB300_SCREEN_WIDTH + dst_x];
            const uint16_t *src_line = (const uint16_t *)((const uint8_t *)src + y * src_pitch);
            memcpy(dst_line, src_line, copy_w * sizeof(uint16_t));
        }
    }

    retro_video_refresh_t vcb = gb300_get_video_cb();
    if (vcb) {
        vcb(gb300_framebuffer, GB300_SCREEN_WIDTH, GB300_SCREEN_HEIGHT, GB300_SCREEN_WIDTH * sizeof(uint16_t));
    }
}
