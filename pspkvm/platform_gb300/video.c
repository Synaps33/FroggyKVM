#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libretro.h"
#include "psp_compat.h"
#include "font8x8.h"

#define GB300_SCREEN_WIDTH  320
#define GB300_SCREEN_HEIGHT 240

static uint16_t gb300_framebuffer[GB300_SCREEN_WIDTH * GB300_SCREEN_HEIGHT] __attribute__((aligned(16)));
static int frame_skip_counter = 0;
static int frame_skip_target = 0; // 0 = no skip

extern retro_video_refresh_t gb300_get_video_cb(void);
extern void gb300_poll_events(void);
static const uint16_t *last_active_fb = gb300_framebuffer;

void gb300_video_init(void) {
    memset(gb300_framebuffer, 0, sizeof(gb300_framebuffer));
    last_active_fb = gb300_framebuffer;
}

uint16_t* gb300_video_get_framebuffer(void) {
    return (uint16_t*)last_active_fb;
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

/* Draws a single 8x8 character in RGB565 */
static void draw_char8x8(int x, int y, char c, uint16_t fg, uint16_t bg) {
    if (c < 32 || c > 126) c = ' ';
    const uint8_t *glyph = font8x8_basic[c - 32];
    for (int row = 0; row < 8; row++) {
        uint8_t line = glyph[row];
        int py = y + row;
        if (py < 0 || py >= GB300_SCREEN_HEIGHT) continue;
        uint16_t *dst = &gb300_framebuffer[py * GB300_SCREEN_WIDTH + x];
        for (int col = 0; col < 8; col++) {
            int px = x + col;
            if (px >= 0 && px < GB300_SCREEN_WIDTH) {
                if (line & (0x80 >> col)) {
                    dst[col] = fg;
                } else if (bg != 0) {
                    dst[col] = bg;
                }
            }
        }
    }
}

/* Draws a null-terminated string using 8x8 font */
void gb300_draw_text(int x, int y, const char *str, uint16_t fg, uint16_t bg) {
    if (!str) return;
    int cur_x = x;
    while (*str) {
        if (*str == '\n') {
            cur_x = x;
            y += 10;
        } else {
            draw_char8x8(cur_x, y, *str, fg, bg);
            cur_x += 8;
            if (cur_x + 8 > GB300_SCREEN_WIDTH) break;
        }
        str++;
    }
}

/* Draws a filled rectangle */
void gb300_fill_rect(int x, int y, int w, int h, uint16_t color) {
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > GB300_SCREEN_WIDTH) w = GB300_SCREEN_WIDTH - x;
    if (y + h > GB300_SCREEN_HEIGHT) h = GB300_SCREEN_HEIGHT - y;
    if (w <= 0 || h <= 0) return;

    for (int row = 0; row < h; row++) {
        uint16_t *line = &gb300_framebuffer[(y + row) * GB300_SCREEN_WIDTH + x];
        for (int col = 0; col < w; col++) {
            line[col] = color;
        }
    }
}

/* ========================================================================= */
/* Hacker Loader Console Implementation                                      */
/* ========================================================================= */

#define HACKER_MAX_LOGS 15
#define HACKER_LINE_MAX 42

static char s_hacker_logs[HACKER_MAX_LOGS][HACKER_LINE_MAX];
static int  s_hacker_log_count = 0;
static char s_rom_name[48] = {0};
static char s_main_class[48] = {0};
static uint32_t s_boot_start_ms = 0;
static int  s_boot_active = 0;
static uint32_t s_last_log_draw_ms = 0;
static int  s_hacker_progress = 0;

/* Color constants in RGB565 */
#define COLOR_BLACK         0x0000
#define COLOR_SCANLINE      0x0080  /* Subtle CRT scanline */
#define COLOR_MATRIX_GREEN  0x07E0  /* Classic Terminal Green text */
#define COLOR_AMBER         0xFFE0  /* Amber / Yellow */
#define COLOR_WHITE         0xFFFF  /* White */
#define COLOR_CYAN          0x07FF  /* Cyan */

extern uint32_t gb300_timer_get_ms(void);

static void hacker_render_screen(void) {
    /* 1. Black background with CRT scanlines */
    for (int y = 0; y < GB300_SCREEN_HEIGHT; y++) {
        uint16_t line_color = (y & 3) ? COLOR_BLACK : COLOR_SCANLINE;
        uint16_t *line = &gb300_framebuffer[y * GB300_SCREEN_WIDTH];
        for (int x = 0; x < GB300_SCREEN_WIDTH; x++) {
            line[x] = line_color;
        }
    }

    /* 2. Top info: ROM File & Main Class */
    char rom_line[42];
    snprintf(rom_line, sizeof(rom_line), "ROM:  %.32s", s_rom_name[0] ? s_rom_name : "-");
    gb300_draw_text(8, 6, rom_line, COLOR_CYAN, 0);

    char cls_line[42];
    snprintf(cls_line, sizeof(cls_line), "MAIN: %.32s", s_main_class[0] ? s_main_class : "...");
    gb300_draw_text(8, 18, cls_line, COLOR_AMBER, 0);

    /* Separator line */
    gb300_fill_rect(8, 29, GB300_SCREEN_WIDTH - 16, 1, COLOR_SCANLINE);

    /* 3. Log stream */
    int log_start_y = 34;
    for (int i = 0; i < s_hacker_log_count && i < HACKER_MAX_LOGS; i++) {
        int text_y = log_start_y + i * 11;
        uint16_t text_color = (i == s_hacker_log_count - 1) ? COLOR_WHITE : COLOR_MATRIX_GREEN;
        gb300_draw_text(8, text_y, s_hacker_logs[i], text_color, 0);
    }

    /* 4. Bottom Progress Bar & Percentage */
    gb300_fill_rect(8, 205, GB300_SCREEN_WIDTH - 16, 1, COLOR_SCANLINE);

    char status_str[32];
    snprintf(status_str, sizeof(status_str), "BOOTING: %3d%%", s_hacker_progress);
    gb300_draw_text(8, 209, status_str, COLOR_CYAN, 0);

    /* Progress bar rectangle */
    int bar_x = 8;
    int bar_y = 221;
    int bar_w = GB300_SCREEN_WIDTH - 16; /* 304 px */
    int bar_h = 11;

    /* Outline */
    gb300_fill_rect(bar_x, bar_y, bar_w, 1, COLOR_SCANLINE);
    gb300_fill_rect(bar_x, bar_y + bar_h - 1, bar_w, 1, COLOR_SCANLINE);
    gb300_fill_rect(bar_x, bar_y, 1, bar_h, COLOR_SCANLINE);
    gb300_fill_rect(bar_x + bar_w - 1, bar_y, 1, bar_h, COLOR_SCANLINE);

    /* Fill */
    int max_fill = bar_w - 4;
    int fill_w = (s_hacker_progress * max_fill) / 100;
    if (fill_w > max_fill) fill_w = max_fill;
    if (fill_w > 0) {
        gb300_fill_rect(bar_x + 2, bar_y + 2, fill_w, bar_h - 4, COLOR_MATRIX_GREEN);
    }
}

static void hacker_flush_to_screen(void) {
    hacker_render_screen();
    last_active_fb = gb300_framebuffer;
    retro_video_refresh_t vcb = gb300_get_video_cb();
    if (vcb) {
        vcb(gb300_framebuffer, GB300_SCREEN_WIDTH, GB300_SCREEN_HEIGHT, GB300_SCREEN_WIDTH * sizeof(uint16_t));
    }
    gb300_poll_events();
}

void gb300_hacker_log(const char *tag, const char *msg, int pct) {
    if (!s_boot_active) return;
    if (pct > s_hacker_progress) {
        s_hacker_progress = pct;
        if (s_hacker_progress > 100) s_hacker_progress = 100;
    }
    if (s_boot_start_ms == 0) {
        s_boot_start_ms = gb300_timer_get_ms();
    }
    uint32_t now = gb300_timer_get_ms();
    uint32_t elapsed_ms = (now >= s_boot_start_ms) ? (now - s_boot_start_ms) : 0;
    uint32_t sec = elapsed_ms / 1000;
    uint32_t msec = (elapsed_ms % 1000) / 10;

    char formatted[HACKER_LINE_MAX];
    snprintf(formatted, sizeof(formatted), "[%u.%02us] [%-4.4s] %.24s",
             (unsigned)sec, (unsigned)msec, tag ? tag : "SYS", msg ? msg : "");

    if (s_hacker_log_count < HACKER_MAX_LOGS) {
        strncpy(s_hacker_logs[s_hacker_log_count], formatted, HACKER_LINE_MAX - 1);
        s_hacker_logs[s_hacker_log_count][HACKER_LINE_MAX - 1] = '\0';
        s_hacker_log_count++;
    } else {
        memmove(&s_hacker_logs[0], &s_hacker_logs[1], (HACKER_MAX_LOGS - 1) * HACKER_LINE_MAX);
        strncpy(s_hacker_logs[HACKER_MAX_LOGS - 1], formatted, HACKER_LINE_MAX - 1);
        s_hacker_logs[HACKER_MAX_LOGS - 1][HACKER_LINE_MAX - 1] = '\0';
    }

    s_last_log_draw_ms = now;
    hacker_flush_to_screen();
}

void gb300_hacker_init(const char *rom_path) {
    s_boot_active = 1;
    s_hacker_log_count = 0;
    s_hacker_progress = 5;
    s_boot_start_ms = gb300_timer_get_ms();
    s_last_log_draw_ms = 0;
    s_main_class[0] = '\0';

    if (rom_path && *rom_path) {
        const char *b = strrchr(rom_path, '/');
        if (b) b++; else b = rom_path;
        snprintf(s_rom_name, sizeof(s_rom_name), "%.40s", b);
    } else {
        strcpy(s_rom_name, "ROM_STREAM");
    }

    gb300_hacker_log("SYS", "CORE INIT: JZ4775 MIPS32r2", 0);
    gb300_hacker_log("VFS", "MOUNT /dev/sdcard ... [OK]", 0);
}

void gb300_hacker_set_main_class(const char *main_class) {
    if (main_class && *main_class) {
        snprintf(s_main_class, sizeof(s_main_class), "%.40s", main_class);
    }
}

void gb300_hacker_log_class(const char *classname) {
    if (!s_boot_active || !classname || !*classname) return;

    if (s_hacker_progress < 98) {
        static int class_counter = 0;
        if (++class_counter % 2 == 0) {
            s_hacker_progress++;
        }
    }

    /* Don't redraw too frequently (min 35ms) unless early in boot */
    uint32_t now = gb300_timer_get_ms();
    if (s_hacker_log_count >= 8 && (now - s_last_log_draw_ms < 35)) {
        return;
    }

    /* Shorten package name, e.g. "javax/microedition/lcdui/Canvas" -> "lcdui/Canvas" */
    const char *short_name = classname;
    const char *last_slash = strrchr(classname, '/');
    if (last_slash && last_slash != classname) {
        const char *prev_slash = last_slash - 1;
        while (prev_slash > classname && *prev_slash != '/') prev_slash--;
        if (*prev_slash == '/') prev_slash++;
        short_name = prev_slash;
    }

    char msg[32];
    snprintf(msg, sizeof(msg), "LOAD: %.24s", short_name);
    gb300_hacker_log("LOAD", msg, 0);
}

void gb300_hacker_stop(void) {
    s_boot_active = 0;
}

void gb300_hacker_exit(int exit_code) {
    s_boot_active = 1;
    s_hacker_progress = 100;
    char exit_msg[32];
    snprintf(exit_msg, sizeof(exit_msg), "JVM EXITED (CODE %d)", exit_code);
    gb300_hacker_log("EXIT", exit_msg, 0);
    gb300_hacker_log("HALT", "SYS HALTED. SELECT+START", 0);
}

/* Backwards compatible wrapper */
void gb300_video_draw_splash(const char *title, const char *rom_name, const char *status) {
    if (!s_boot_active) {
        gb300_hacker_init(rom_name);
    }
    if (status && *status) {
        gb300_hacker_log("MSG", status, 0);
    }
}

/* Flushes RGB565 source buffer into GB300 framebuffer, centering if dimensions differ */
void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch) {
    if (!src || gb300_video_should_skip()) return;

    /* Hand off screen to game on first frame */
    s_boot_active = 0;

    static int last_w = -1, last_h = -1;
    const uint16_t *out_frame = gb300_framebuffer;

    /* Fast path: full screen 320x240 buffer - zero copy direct refresh */
    if (src_w == GB300_SCREEN_WIDTH && src_h == GB300_SCREEN_HEIGHT &&
        src_pitch == (int)(GB300_SCREEN_WIDTH * sizeof(uint16_t)) &&
        (((uintptr_t)src & 3) == 0)) {
        out_frame = src;
        last_w = src_w;
        last_h = src_h;
    } else {
        int dst_x = (GB300_SCREEN_WIDTH - src_w) / 2;
        int dst_y = (GB300_SCREEN_HEIGHT - src_h) / 2;

        if (dst_x < 0) dst_x = 0;
        if (dst_y < 0) dst_y = 0;

        int copy_w = (src_w > GB300_SCREEN_WIDTH) ? GB300_SCREEN_WIDTH : src_w;
        int copy_h = (src_h > GB300_SCREEN_HEIGHT) ? GB300_SCREEN_HEIGHT : src_h;

        /* Only clear framebuffer if viewport dimensions changed to save memory bus bandwidth */
        if (last_w != src_w || last_h != src_h) {
            memset(gb300_framebuffer, 0, sizeof(gb300_framebuffer));
            last_w = src_w;
            last_h = src_h;
        }

        for (int y = 0; y < copy_h; y++) {
            uint16_t *dst_line = &gb300_framebuffer[(dst_y + y) * GB300_SCREEN_WIDTH + dst_x];
            const uint16_t *src_line = (const uint16_t *)((const uint8_t *)src + y * src_pitch);
            memcpy(dst_line, src_line, copy_w * sizeof(uint16_t));
        }
        out_frame = gb300_framebuffer;
    }

    last_active_fb = out_frame;

    retro_video_refresh_t vcb = gb300_get_video_cb();
    if (vcb) {
        vcb(out_frame, GB300_SCREEN_WIDTH, GB300_SCREEN_HEIGHT, GB300_SCREEN_WIDTH * sizeof(uint16_t));
    }

    /* Poll inputs, check hotkeys, process audio without artificial delay! */
    gb300_poll_events();
}
