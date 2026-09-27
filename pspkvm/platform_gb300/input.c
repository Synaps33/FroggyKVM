#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <kni.h>
#include "psp_compat.h"
#include "javacall_keypress.h"
#include "midlet_meta.h"

extern void xlog(const char *fmt, ...);
extern void javacall_lcd_set_resolution(int w, int h);

static uint32_t last_buttons = 0;
static int s_screen_width = 320;
static int s_screen_height = 240;

typedef struct {
    const char *name;
    uint32_t psp_bit;
    javacall_key default_jkey;
    javacall_key current_jkey;
} button_config_t;

/* GB300 / RetroPad button mapping table */
static button_config_t btn_configs[] = {
    { "UP",       PSP_CTRL_UP,       JAVACALL_KEY_2,        JAVACALL_KEY_2 },        /* D-pad -> phone keypad */
    { "DOWN",     PSP_CTRL_DOWN,     JAVACALL_KEY_8,        JAVACALL_KEY_8 },
    { "LEFT",     PSP_CTRL_LEFT,     JAVACALL_KEY_4,        JAVACALL_KEY_4 },
    { "RIGHT",    PSP_CTRL_RIGHT,    JAVACALL_KEY_6,        JAVACALL_KEY_6 },
    { "A",        PSP_CTRL_CROSS,    JAVACALL_KEY_5,        JAVACALL_KEY_5 },        /* A -> 5 */
    { "B",        PSP_CTRL_CIRCLE,   JAVACALL_KEY_3,        JAVACALL_KEY_3 },        /* B -> 3 */
    { "X",        PSP_CTRL_SQUARE,   JAVACALL_KEY_0,        JAVACALL_KEY_0 },        /* X -> 0 */
    { "Y",        PSP_CTRL_TRIANGLE, JAVACALL_KEY_1,        JAVACALL_KEY_1 },        /* Y -> 1 */
    { "L",        PSP_CTRL_LTRIGGER, JAVACALL_KEY_SOFT1,    JAVACALL_KEY_SOFT1 },    /* L -> left softkey */
    { "R",        PSP_CTRL_RTRIGGER, JAVACALL_KEY_SOFT2,    JAVACALL_KEY_SOFT2 },    /* R -> right softkey */
    { "SELECT",   PSP_CTRL_SELECT,   JAVACALL_KEY_ASTERISK, JAVACALL_KEY_ASTERISK }, /* Select -> * */
    { "START",    PSP_CTRL_START,    JAVACALL_KEY_POUND,    JAVACALL_KEY_POUND }     /* Start -> # */
};

#define NUM_BUTTON_CONFIGS (sizeof(btn_configs) / sizeof(btn_configs[0]))

static bool s_enable_combos = true;

/* Configurable combo targets */
static javacall_key s_combo_select_up    = JAVACALL_KEY_UP;
static javacall_key s_combo_select_down  = JAVACALL_KEY_DOWN;
static javacall_key s_combo_select_left  = JAVACALL_KEY_LEFT;
static javacall_key s_combo_select_right = JAVACALL_KEY_RIGHT;

static javacall_key s_combo_start_y = JAVACALL_KEY_1;
static javacall_key s_combo_start_x = JAVACALL_KEY_3;
static javacall_key s_combo_start_b = JAVACALL_KEY_7;
static javacall_key s_combo_start_a = JAVACALL_KEY_9;

/* Modifier tracking */
static bool s_select_held       = false;
static bool s_select_combo_used = false;
static bool s_start_held        = false;
static bool s_start_combo_used  = false;

/* Active key emitted per button config (to cleanly release the exact key that was pressed) */
static javacall_key s_active_jkey[NUM_BUTTON_CONFIGS];

static javacall_key get_configured_key(uint32_t psp_bit) {
    for (size_t i = 0; i < NUM_BUTTON_CONFIGS; i++) {
        if (btn_configs[i].psp_bit == psp_bit) {
            return btn_configs[i].current_jkey;
        }
    }
    return JAVACALL_KEY_INVALID;
}

static char *trim_str(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

static javacall_key parse_key_value(const char *val) {
    if (!val || !*val) return JAVACALL_KEY_INVALID;

    if (strcasecmp(val, "UP") == 0) return JAVACALL_KEY_UP;
    if (strcasecmp(val, "DOWN") == 0) return JAVACALL_KEY_DOWN;
    if (strcasecmp(val, "LEFT") == 0) return JAVACALL_KEY_LEFT;
    if (strcasecmp(val, "RIGHT") == 0) return JAVACALL_KEY_RIGHT;
    if (strcasecmp(val, "SELECT") == 0 || strcasecmp(val, "FIRE") == 0 ||
        strcasecmp(val, "OK") == 0 || strcasecmp(val, "ENTER") == 0) return JAVACALL_KEY_SELECT;
    if (strcasecmp(val, "SOFT1") == 0 || strcasecmp(val, "SOFT_LEFT") == 0 ||
        strcasecmp(val, "LSK") == 0 || strcasecmp(val, "MENU") == 0) return JAVACALL_KEY_SOFT1;
    if (strcasecmp(val, "SOFT2") == 0 || strcasecmp(val, "SOFT_RIGHT") == 0 ||
        strcasecmp(val, "RSK") == 0 || strcasecmp(val, "BACK") == 0) return JAVACALL_KEY_SOFT2;
    if (strcasecmp(val, "CLEAR") == 0 || strcasecmp(val, "CLR") == 0) return JAVACALL_KEY_CLEAR;
    if (strcasecmp(val, "SEND") == 0 || strcasecmp(val, "CALL") == 0) return JAVACALL_KEY_SEND;
    if (strcasecmp(val, "END") == 0) return JAVACALL_KEY_END;
    if (strcasecmp(val, "SPACE") == 0) return JAVACALL_KEY_SPACE;

    if (strcasecmp(val, "0") == 0 || strcasecmp(val, "NUM0") == 0) return JAVACALL_KEY_0;
    if (strcasecmp(val, "1") == 0 || strcasecmp(val, "NUM1") == 0) return JAVACALL_KEY_1;
    if (strcasecmp(val, "2") == 0 || strcasecmp(val, "NUM2") == 0) return JAVACALL_KEY_2;
    if (strcasecmp(val, "3") == 0 || strcasecmp(val, "NUM3") == 0) return JAVACALL_KEY_3;
    if (strcasecmp(val, "4") == 0 || strcasecmp(val, "NUM4") == 0) return JAVACALL_KEY_4;
    if (strcasecmp(val, "5") == 0 || strcasecmp(val, "NUM5") == 0) return JAVACALL_KEY_5;
    if (strcasecmp(val, "6") == 0 || strcasecmp(val, "NUM6") == 0) return JAVACALL_KEY_6;
    if (strcasecmp(val, "7") == 0 || strcasecmp(val, "NUM7") == 0) return JAVACALL_KEY_7;
    if (strcasecmp(val, "8") == 0 || strcasecmp(val, "NUM8") == 0) return JAVACALL_KEY_8;
    if (strcasecmp(val, "9") == 0 || strcasecmp(val, "NUM9") == 0) return JAVACALL_KEY_9;

    if (strcasecmp(val, "*") == 0 || strcasecmp(val, "STAR") == 0 || strcasecmp(val, "ASTERISK") == 0) return JAVACALL_KEY_ASTERISK;
    if (strcasecmp(val, "#") == 0 || strcasecmp(val, "POUND") == 0 || strcasecmp(val, "HASH") == 0) return JAVACALL_KEY_POUND;

    if (strcasecmp(val, "GAMEA") == 0 || strcasecmp(val, "GAME_A") == 0) return JAVACALL_KEY_GAMEA;
    if (strcasecmp(val, "GAMEB") == 0 || strcasecmp(val, "GAME_B") == 0) return JAVACALL_KEY_GAMEB;
    if (strcasecmp(val, "GAMEC") == 0 || strcasecmp(val, "GAME_C") == 0) return JAVACALL_KEY_GAMEC;
    if (strcasecmp(val, "GAMED") == 0 || strcasecmp(val, "GAME_D") == 0) return JAVACALL_KEY_GAMED;

    if (strcasecmp(val, "NONE") == 0 || strcasecmp(val, "DISABLED") == 0) return JAVACALL_KEY_INVALID;

    xlog("[PSPKVM-INPUT] Unknown key name '%s' in keymap\n", val);
    return JAVACALL_KEY_INVALID;
}

void gb300_input_init(void) {
    last_buttons = 0;
    s_select_held       = false;
    s_select_combo_used = false;
    s_start_held        = false;
    s_start_combo_used  = false;
    s_enable_combos     = true;

    s_combo_select_up    = JAVACALL_KEY_UP;
    s_combo_select_down  = JAVACALL_KEY_DOWN;
    s_combo_select_left  = JAVACALL_KEY_LEFT;
    s_combo_select_right = JAVACALL_KEY_RIGHT;

    s_combo_start_y      = JAVACALL_KEY_1;
    s_combo_start_x      = JAVACALL_KEY_3;
    s_combo_start_b      = JAVACALL_KEY_7;
    s_combo_start_a      = JAVACALL_KEY_9;

    for (size_t i = 0; i < NUM_BUTTON_CONFIGS; i++) {
        btn_configs[i].current_jkey = btn_configs[i].default_jkey;
        s_active_jkey[i] = JAVACALL_KEY_INVALID;
    }
}

int g_custom_heap_size = 0;

static bool parse_keymap_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) return false;

    xlog("[PSPKVM-INPUT] Loading keymap from: %s\n", path);
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '#' || *p == ';' || *p == '\0' || *p == '\r' || *p == '\n') continue;

        char *eq = strchr(p, '=');
        if (!eq) continue;
        *eq = '\0';
        char *btn_name = trim_str(p);
        char *key_name = trim_str(eq + 1);

        /* Check for display resolution configuration */
        if (strcasecmp(btn_name, "width") == 0 || strcasecmp(btn_name, "screen_width") == 0) {
            int val = atoi(key_name);
            if (val > 0 && val <= 512) {
                s_screen_width = val;
                xlog("[PSPKVM-INPUT]   Configured screen_width = %d\n", s_screen_width);
            }
            continue;
        }
        if (strcasecmp(btn_name, "height") == 0 || strcasecmp(btn_name, "screen_height") == 0) {
            int val = atoi(key_name);
            if (val > 0 && val <= 512) {
                s_screen_height = val;
                xlog("[PSPKVM-INPUT]   Configured screen_height = %d\n", s_screen_height);
            }
            continue;
        }
        if (strcasecmp(btn_name, "resolution") == 0 || strcasecmp(btn_name, "screen_size") == 0) {
            int w = 0, h = 0;
            if (sscanf(key_name, "%dx%d", &w, &h) == 2 || sscanf(key_name, "%d,%d", &w, &h) == 2) {
                if (w > 0 && h > 0 && w <= 512 && h <= 512) {
                    s_screen_width = w;
                    s_screen_height = h;
                    xlog("[PSPKVM-INPUT]   Configured resolution = %dx%d\n", w, h);
                }
            }
            continue;
        }
        if (strcasecmp(btn_name, "heap") == 0 || strcasecmp(btn_name, "heap_size") == 0) {
            long val = 0;
            char unit = 0;
            if (sscanf(key_name, "%ld%c", &val, &unit) >= 1) {
                if (unit == 'M' || unit == 'm') {
                    val *= 1024 * 1024;
                } else if (unit == 'K' || unit == 'k') {
                    val *= 1024;
                } else if (val <= 64) {
                    val *= 1024 * 1024;
                }
                if (val >= 2 * 1024 * 1024 && val <= 56 * 1024 * 1024) {
                    g_custom_heap_size = (int)val;
                    xlog("[PSPKVM-INPUT]   Configured custom heap = %d bytes (%.2f MB)\n",
                         g_custom_heap_size, (double)g_custom_heap_size / (1024.0 * 1024.0));
                }
            }
            continue;
        }

        if (strcasecmp(btn_name, "enable_combos") == 0 || strcasecmp(btn_name, "combos") == 0 ||
            strcasecmp(btn_name, "chord_keys") == 0) {
            s_enable_combos = (atoi(key_name) != 0 || strcasecmp(key_name, "true") == 0 || strcasecmp(key_name, "yes") == 0);
            xlog("[PSPKVM-INPUT]   Configured enable_combos = %d\n", s_enable_combos);
            continue;
        }
        if (strcasecmp(btn_name, "select_up") == 0 || strcasecmp(btn_name, "select+up") == 0) {
            s_combo_select_up = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped SELECT+UP -> %d (%s)\n", (int)s_combo_select_up, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "select_down") == 0 || strcasecmp(btn_name, "select+down") == 0) {
            s_combo_select_down = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped SELECT+DOWN -> %d (%s)\n", (int)s_combo_select_down, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "select_left") == 0 || strcasecmp(btn_name, "select+left") == 0) {
            s_combo_select_left = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped SELECT+LEFT -> %d (%s)\n", (int)s_combo_select_left, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "select_right") == 0 || strcasecmp(btn_name, "select+right") == 0) {
            s_combo_select_right = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped SELECT+RIGHT -> %d (%s)\n", (int)s_combo_select_right, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "start_y") == 0 || strcasecmp(btn_name, "start+y") == 0) {
            s_combo_start_y = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped START+Y -> %d (%s)\n", (int)s_combo_start_y, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "start_x") == 0 || strcasecmp(btn_name, "start+x") == 0) {
            s_combo_start_x = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped START+X -> %d (%s)\n", (int)s_combo_start_x, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "start_b") == 0 || strcasecmp(btn_name, "start+b") == 0) {
            s_combo_start_b = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped START+B -> %d (%s)\n", (int)s_combo_start_b, key_name);
            continue;
        }
        if (strcasecmp(btn_name, "start_a") == 0 || strcasecmp(btn_name, "start+a") == 0) {
            s_combo_start_a = parse_key_value(key_name);
            xlog("[PSPKVM-INPUT]   Mapped START+A -> %d (%s)\n", (int)s_combo_start_a, key_name);
            continue;
        }

        javacall_key parsed_k = parse_key_value(key_name);

        /* Match button */
        bool found = false;
        for (size_t i = 0; i < NUM_BUTTON_CONFIGS; i++) {
            if (strcasecmp(btn_name, btn_configs[i].name) == 0) {
                btn_configs[i].current_jkey = parsed_k;
                xlog("[PSPKVM-INPUT]   Mapped %s -> key code %d (%s)\n", btn_configs[i].name, (int)parsed_k, key_name);
                found = true;
                break;
            }
        }
        if (!found) {
            xlog("[PSPKVM-INPUT] Unknown button name '%s' in keymap file\n", btn_name);
        }
    }

    fclose(fp);
    return true;
}

void gb300_input_load_config(const char *jar_path) {
    /* First reset to defaults */
    gb300_input_init();

    s_screen_width = 320;
    s_screen_height = 240;

    if (!jar_path || !*jar_path) {
        javacall_lcd_set_resolution(s_screen_width, s_screen_height);
        return;
    }

    int dw = 320, dh = 240;
    if (gb300_detect_screen_size(jar_path, &dw, &dh)) {
        s_screen_width = dw;
        s_screen_height = dh;
        xlog("[PSPKVM] Auto-detected screen size: %dx%d\n", dw, dh);
    }

    char cfg_path[512];
    bool loaded = false;

    /* Extract basename (e.g. "Anno1503" from "/mnt/sda1/ROMS/j2me/Anno1503.jar") */
    char basename[128] = "";
    char dir[256] = "";
    const char *last_slash = strrchr(jar_path, '/');
    if (last_slash) {
        size_t dirlen = (size_t)(last_slash - jar_path);
        if (dirlen >= sizeof(dir)) dirlen = sizeof(dir) - 1;
        strncpy(dir, jar_path, dirlen);
        dir[dirlen] = '\0';

        strncpy(basename, last_slash + 1, sizeof(basename) - 1);
        basename[sizeof(basename) - 1] = '\0';
    } else {
        strncpy(basename, jar_path, sizeof(basename) - 1);
        basename[sizeof(basename) - 1] = '\0';
    }
    char *bdot = strrchr(basename, '.');
    if (bdot && (strcasecmp(bdot, ".jar") == 0 || strcasecmp(bdot, ".jad") == 0)) {
        *bdot = '\0';
    }

    /* 1. Check game-specific configs in /config/j2me and /configs/j2me */
    const char *config_dirs[] = {
        "/mnt/sda1/config/j2me",
        "/mnt/sda1/configs/j2me",
        "config/j2me",
        "configs/j2me",
        "/media/Sajnaps/GB300/config/j2me",
        "/media/Sajnaps/GB300/configs/j2me"
    };
    const size_t num_dirs = sizeof(config_dirs) / sizeof(config_dirs[0]);

    if (*basename) {
        for (size_t i = 0; i < num_dirs && !loaded; i++) {
            snprintf(cfg_path, sizeof(cfg_path), "%s/%s.cfg", config_dirs[i], basename);
            if (parse_keymap_file(cfg_path)) loaded = true;
        }
    }

    /* 2. Try <jar_path_without_ext>.cfg */
    if (!loaded) {
        strncpy(cfg_path, jar_path, sizeof(cfg_path) - 5);
        cfg_path[sizeof(cfg_path) - 5] = '\0';
        char *dot = strrchr(cfg_path, '.');
        if (dot && (strcasecmp(dot, ".jar") == 0 || strcasecmp(dot, ".jad") == 0)) {
            strcpy(dot, ".cfg");
            if (parse_keymap_file(cfg_path)) loaded = true;
        }
    }

    /* 3. Try <jar_path>.cfg */
    if (!loaded) {
        snprintf(cfg_path, sizeof(cfg_path), "%s.cfg", jar_path);
        if (parse_keymap_file(cfg_path)) loaded = true;
    }

    /* 4. Try <dir>/keymaps/<game_name>.cfg */
    if (!loaded && *dir && *basename) {
        snprintf(cfg_path, sizeof(cfg_path), "%s/keymaps/%s.cfg", dir, basename);
        if (parse_keymap_file(cfg_path)) loaded = true;
    }

    /* 5. Fallback: default.cfg in /config/j2me and /configs/j2me */
    if (!loaded) {
        for (size_t i = 0; i < num_dirs && !loaded; i++) {
            snprintf(cfg_path, sizeof(cfg_path), "%s/default.cfg", config_dirs[i]);
            if (parse_keymap_file(cfg_path)) loaded = true;
        }
    }

    /* 6. Fallback: <dir>/keymaps/default.cfg */
    if (!loaded && *dir) {
        snprintf(cfg_path, sizeof(cfg_path), "%s/keymaps/default.cfg", dir);
        if (parse_keymap_file(cfg_path)) loaded = true;
    }

    if (!loaded) {
        xlog("[PSPKVM-INPUT] No custom keymap found for '%s'. Using hardcoded defaults.\n", jar_path);
    }

    javacall_lcd_set_resolution(s_screen_width, s_screen_height);
    xlog("[PSPKVM] Screen resolution set to %dx%d\n", s_screen_width, s_screen_height);
}

void gb300_input_poll(uint32_t current_buttons) {
    uint32_t changed = current_buttons ^ last_buttons;
    if (!changed) return;

    if (!s_enable_combos) {
        for (size_t i = 0; i < NUM_BUTTON_CONFIGS; i++) {
            uint32_t bit = btn_configs[i].psp_bit;
            if (changed & bit) {
                javacall_key jk = btn_configs[i].current_jkey;
                if (jk != JAVACALL_KEY_INVALID) {
                    if (current_buttons & bit) {
                        xlog("[PSPKVM-INPUT] %s PRESSED -> jkey %d\n", btn_configs[i].name, jk);
                        javanotify_key_event(jk, JAVACALL_KEYPRESSED);
                    } else {
                        xlog("[PSPKVM-INPUT] %s RELEASED -> jkey %d\n", btn_configs[i].name, jk);
                        javanotify_key_event(jk, JAVACALL_KEYRELEASED);
                    }
                }
            }
        }
        last_buttons = current_buttons;
        return;
    }

    /* 1. Track modifier state changes first */
    if (changed & PSP_CTRL_SELECT) {
        if (current_buttons & PSP_CTRL_SELECT) {
            s_select_held = true;
            s_select_combo_used = false;
            xlog("[PSPKVM-INPUT] SELECT HELD (combo modifier active)\n");
        } else {
            s_select_held = false;
            if (!s_select_combo_used) {
                javacall_key jk = get_configured_key(PSP_CTRL_SELECT);
                if (jk != JAVACALL_KEY_INVALID) {
                    xlog("[PSPKVM-INPUT] SELECT TAP -> jkey %d\n", jk);
                    javanotify_key_event(jk, JAVACALL_KEYPRESSED);
                    javanotify_key_event(jk, JAVACALL_KEYRELEASED);
                }
            }
            s_select_combo_used = false;
        }
    }

    if (changed & PSP_CTRL_START) {
        if (current_buttons & PSP_CTRL_START) {
            s_start_held = true;
            s_start_combo_used = false;
            xlog("[PSPKVM-INPUT] START HELD (combo modifier active)\n");
        } else {
            s_start_held = false;
            if (!s_start_combo_used) {
                javacall_key jk = get_configured_key(PSP_CTRL_START);
                if (jk != JAVACALL_KEY_INVALID) {
                    xlog("[PSPKVM-INPUT] START TAP -> jkey %d\n", jk);
                    javanotify_key_event(jk, JAVACALL_KEYPRESSED);
                    javanotify_key_event(jk, JAVACALL_KEYRELEASED);
                }
            }
            s_start_combo_used = false;
        }
    }

    /* 2. Process all non-modifier buttons */
    for (size_t i = 0; i < NUM_BUTTON_CONFIGS; i++) {
        uint32_t bit = btn_configs[i].psp_bit;
        if (bit == PSP_CTRL_SELECT || bit == PSP_CTRL_START) {
            continue;
        }

        if (changed & bit) {
            if (current_buttons & bit) {
                /* Button PRESSED */
                javacall_key jk = JAVACALL_KEY_INVALID;
                bool is_combo = false;

                if (s_select_held) {
                    if (bit == PSP_CTRL_UP)         { jk = s_combo_select_up;    s_select_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_DOWN)  { jk = s_combo_select_down;  s_select_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_LEFT)  { jk = s_combo_select_left;  s_select_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_RIGHT) { jk = s_combo_select_right; s_select_combo_used = true; is_combo = true; }
                }

                if (!is_combo && s_start_held) {
                    if (bit == PSP_CTRL_TRIANGLE)   { jk = s_combo_start_y; s_start_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_SQUARE){ jk = s_combo_start_x; s_start_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_CIRCLE){ jk = s_combo_start_b; s_start_combo_used = true; is_combo = true; }
                    else if (bit == PSP_CTRL_CROSS) { jk = s_combo_start_a; s_start_combo_used = true; is_combo = true; }
                }

                if (!is_combo) {
                    jk = btn_configs[i].current_jkey;
                }

                s_active_jkey[i] = jk;
                if (jk != JAVACALL_KEY_INVALID) {
                    xlog("[PSPKVM-INPUT] %s PRESSED -> jkey %d%s\n",
                         btn_configs[i].name, jk, is_combo ? " (COMBO)" : "");
                    javanotify_key_event(jk, JAVACALL_KEYPRESSED);
                }
            } else {
                /* Button RELEASED */
                javacall_key jk = s_active_jkey[i];
                s_active_jkey[i] = JAVACALL_KEY_INVALID;
                if (jk != JAVACALL_KEY_INVALID) {
                    xlog("[PSPKVM-INPUT] %s RELEASED -> jkey %d\n", btn_configs[i].name, jk);
                    javanotify_key_event(jk, JAVACALL_KEYRELEASED);
                }
            }
        }
    }

    last_buttons = current_buttons;
}

/* J2ME KeyConverter Native Implementations */
KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_lcdui_KeyConverter_getKeyCode(void) {
    int gameAction = KNI_GetParameterAsInt(1);
    int keyCode = 0;
    switch (gameAction) {
        case 1:  keyCode = -1; break; /* Canvas.UP */
        case 2:  keyCode = -3; break; /* Canvas.LEFT */
        case 5:  keyCode = -4; break; /* Canvas.RIGHT */
        case 6:  keyCode = -2; break; /* Canvas.DOWN */
        case 8:  keyCode = -5; break; /* Canvas.FIRE */
        case 9:  keyCode = '5'; break; /* Canvas.GAME_A */
        case 10: keyCode = '7'; break; /* Canvas.GAME_B */
        case 11: keyCode = '1'; break; /* Canvas.GAME_C */
        case 12: keyCode = '3'; break; /* Canvas.GAME_D */
        default: keyCode = 0; break;
    }
    KNI_ReturnInt(keyCode);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_lcdui_KeyConverter_getGameAction(void) {
    int keyCode = KNI_GetParameterAsInt(1);
    int action = 0;
    switch (keyCode) {
        case -1:
        case '2': action = 1; break; /* UP */
        case -2:
        case '8': action = 6; break; /* DOWN */
        case -3:
        case '4': action = 2; break; /* LEFT */
        case -4:
        case '6': action = 5; break; /* RIGHT */
        case -5:
        case '5': action = 8; break; /* FIRE */
        case '7': action = 10; break; /* GAME_B */
        case '1': action = 11; break; /* GAME_C */
        case '3': action = 12; break; /* GAME_D */
        default:  action = 0; break;
    }
    xlog("[KEY-ACTION] getGameAction(%d) -> %d\n", keyCode, action);
    KNI_ReturnInt(action);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_javax_microedition_lcdui_KeyConverter_getSystemKey(void) {
    int keyCode = KNI_GetParameterAsInt(1);
    int sysKey = 0;
    switch (keyCode) {
        case -6: sysKey = 1; break; /* Soft1 */
        case -7: sysKey = 2; break; /* Soft2 */
        default: sysKey = 0; break;
    }
    KNI_ReturnInt(sysKey);
}

KNIEXPORT KNI_RETURNTYPE_OBJECT
Java_javax_microedition_lcdui_KeyConverter_getKeyName(void) {
    int keyCode = KNI_GetParameterAsInt(1);
    const char *name = NULL;
    switch (keyCode) {
        case -1: name = "Up"; break;
        case -2: name = "Down"; break;
        case -3: name = "Left"; break;
        case -4: name = "Right"; break;
        case -5: name = "Select"; break;
        case -6: name = "Soft1"; break;
        case -7: name = "Soft2"; break;
        case '0': name = "0"; break;
        case '1': name = "1"; break;
        case '2': name = "2"; break;
        case '3': name = "3"; break;
        case '4': name = "4"; break;
        case '5': name = "5"; break;
        case '6': name = "6"; break;
        case '7': name = "7"; break;
        case '8': name = "8"; break;
        case '9': name = "9"; break;
        case '*': name = "*"; break;
        case '#': name = "#"; break;
        default: break;
    }
    KNI_StartHandles(1);
    KNI_DeclareHandle(str);
    if (name) {
        KNI_NewStringUTF(name, str);
    } else if (keyCode >= 32 && keyCode <= 126) {
        jchar c = (jchar)keyCode;
        KNI_NewString(&c, 1, str);
    } else {
        KNI_ReleaseHandle(str);
    }
    KNI_EndHandlesAndReturnObject(str);
}
