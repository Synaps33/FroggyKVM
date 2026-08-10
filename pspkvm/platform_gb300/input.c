#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "psp_compat.h"
#include "javacall_keypress.h"

static uint32_t last_buttons = 0;

typedef struct {
    uint32_t psp_bit;
    javacall_key jkey;
} button_map_t;

/* Standard GB300 mapping */
static const button_map_t btn_maps[] = {
    { PSP_CTRL_UP,       JAVACALL_KEY_UP },
    { PSP_CTRL_DOWN,     JAVACALL_KEY_DOWN },
    { PSP_CTRL_LEFT,     JAVACALL_KEY_LEFT },
    { PSP_CTRL_RIGHT,    JAVACALL_KEY_RIGHT },
    { PSP_CTRL_CROSS,    JAVACALL_KEY_SELECT },   /* A button -> Fire */
    { PSP_CTRL_CIRCLE,   JAVACALL_KEY_SOFT1 },    /* B button -> Soft Left */
    { PSP_CTRL_SQUARE,   JAVACALL_KEY_SOFT2 },    /* X button -> Soft Right */
    { PSP_CTRL_TRIANGLE, JAVACALL_KEY_ASTERISK }, /* Y button -> * */
    { PSP_CTRL_LTRIGGER, JAVACALL_KEY_1 },        /* L button -> 1 */
    { PSP_CTRL_RTRIGGER, JAVACALL_KEY_3 },        /* R button -> 3 */
    { PSP_CTRL_SELECT,   JAVACALL_KEY_POUND },    /* Select -> # */
    { PSP_CTRL_START,    JAVACALL_KEY_SOFT1 }     /* Start -> Menu */
};

#define NUM_BUTTON_MAPS (sizeof(btn_maps) / sizeof(btn_maps[0]))

void gb300_input_init(void) {
    last_buttons = 0;
}

void gb300_input_poll(uint32_t current_buttons) {
    uint32_t changed = current_buttons ^ last_buttons;
    if (!changed) return;

    for (size_t i = 0; i < NUM_BUTTON_MAPS; i++) {
        uint32_t bit = btn_maps[i].psp_bit;
        if (changed & bit) {
            if (current_buttons & bit) {
                javanotify_key_event(btn_maps[i].jkey, JAVACALL_KEYPRESSED);
            } else {
                javanotify_key_event(btn_maps[i].jkey, JAVACALL_KEYRELEASED);
            }
        }
    }
    last_buttons = current_buttons;
}
