#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libretro.h"
#include "psp_compat.h"

/* Declarations for GB300 platform sub-modules */
void gb300_video_init(void);
uint16_t* gb300_video_get_framebuffer(void);
void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch);

void gb300_audio_init(void);
void gb300_audio_deinit(void);
int gb300_audio_read(int16_t *dst, int num_frames);

void gb300_input_init(void);
void gb300_input_poll(uint32_t current_buttons);

void gb300_fs_init(void);
void gb300_fs_set_rom(const char *path);

void JavaTask(void);

/* Libretro callback function pointers */
static retro_video_refresh_t video_cb = NULL;
static retro_audio_sample_t audio_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_input_poll_t input_poll_cb = NULL;
static retro_input_state_t input_state_cb = NULL;
static retro_environment_t environ_cb = NULL;

static bool game_loaded = false;

/* Convert retro_input_state_cb calls to PSP button mask */
static uint32_t poll_gb300_buttons(void) {
    if (!input_state_cb) return 0;
    uint32_t mask = 0;

    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))     mask |= PSP_CTRL_UP;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))   mask |= PSP_CTRL_DOWN;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))   mask |= PSP_CTRL_LEFT;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))  mask |= PSP_CTRL_RIGHT;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A))      mask |= PSP_CTRL_CROSS;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B))      mask |= PSP_CTRL_CIRCLE;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X))      mask |= PSP_CTRL_SQUARE;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y))      mask |= PSP_CTRL_TRIANGLE;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L))      mask |= PSP_CTRL_LTRIGGER;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R))      mask |= PSP_CTRL_RTRIGGER;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) mask |= PSP_CTRL_SELECT;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START))  mask |= PSP_CTRL_START;

    return mask;
}

RETRO_API unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

RETRO_API void retro_set_environment(retro_environment_t cb) {
    environ_cb = cb;
    if (environ_cb) {
        enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
        environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
    }
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
RETRO_API void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
RETRO_API void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
RETRO_API void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }
RETRO_API void retro_set_controller_port_device(unsigned port, unsigned device) { (void)port; (void)device; }

RETRO_API void retro_init(void) {
    xlog("[PSPKVM-GB300] retro_init: Initializing PSPKVM core...\n");
    gb300_video_init();
    gb300_audio_init();
    gb300_input_init();
    gb300_fs_init();
    xlog("[PSPKVM-GB300] retro_init: Initialization complete.\n");
}

RETRO_API void retro_deinit(void) {
    xlog("[PSPKVM-GB300] retro_deinit: Deinitializing core...\n");
    gb300_audio_deinit();
    game_loaded = false;
}

RETRO_API void retro_get_system_info(struct retro_system_info *info) {
    memset(info, 0, sizeof(*info));
    info->library_name     = "PSPKVM J2ME";
    info->library_version  = "0.5.5";
    info->valid_extensions = "jar|jad";
    info->need_fullpath    = true;
    info->block_extract    = false;
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
    memset(info, 0, sizeof(*info));
    info->timing.fps            = 60.0;
    info->timing.sample_rate    = 22050.0;
    info->geometry.base_width   = 320;
    info->geometry.base_height  = 240;
    info->geometry.max_width    = 320;
    info->geometry.max_height   = 240;
    info->geometry.aspect_ratio = 4.0 / 3.0;
}

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
    if (game && game->path) {
        xlog("[PSPKVM-GB300] retro_load_game: Loading ROM file '%s'\n", game->path);
        gb300_fs_set_rom(game->path);
    } else {
        xlog("[PSPKVM-GB300] retro_load_game: Running in standalone/stub mode\n");
        gb300_fs_set_rom("/ROMS/J2ME/stub.jar");
    }
    game_loaded = true;
    return true;
}

RETRO_API void retro_unload_game(void) {
    xlog("[PSPKVM-GB300] retro_unload_game: Game unloaded\n");
    game_loaded = false;
}

retro_video_refresh_t gb300_get_video_cb(void) {
    return video_cb;
}

void javanotify_start_java_with_arbitrary_args(int argc, char* argv[]);
void JavaTask(void);
const char* gb300_fs_get_jar(void);

static bool jvm_started = false;
void j2me_canvas_render_frame(uint32_t buttons);

RETRO_API void retro_run(void) {
    if (!game_loaded) return;

    if (!jvm_started) {
        jvm_started = true;
        const char *rom = gb300_fs_get_jar();
        xlog("[PSPKVM-GB300] Starting Java VM for: %s\n", rom ? rom : "NULL");

        char *argv[5];
        argv[0] = "pspkvm";
        argv[1] = "-heapsize";
        argv[2] = "4M";
        if (rom && (strstr(rom, ".jad") || strstr(rom, ".JAD"))) {
            argv[3] = "-Xdescriptor";
        } else {
            argv[3] = "-jar";
        }
        argv[4] = (char*)(rom ? rom : "");

        xlog("[PSPKVM-GB300] Invoking javanotify_start_java_with_arbitrary_args(argc=5, argv[4]='%s')...\n", argv[4]);
        javanotify_start_java_with_arbitrary_args(5, argv);
        xlog("[PSPKVM-GB300] javanotify returned. Now calling JavaTask()...\n");
        JavaTask();
        xlog("[PSPKVM-GB300] JavaTask() initialized.\n");
    }

    if (input_poll_cb) input_poll_cb();
    uint32_t buttons = poll_gb300_buttons();
    gb300_input_poll(buttons);

    /* Render current framebuffer to Libretro video callback */
    uint16_t *fb = gb300_video_get_framebuffer();
    if (video_cb && fb) {
        video_cb(fb, 320, 240, 320 * sizeof(uint16_t));
    }

    /* Output audio frame samples */
    int16_t pcm_samples[256 * 2];
    int frames_read = gb300_audio_read(pcm_samples, 256);
    if (audio_batch_cb && frames_read > 0) {
        audio_batch_cb(pcm_samples, frames_read);
    }
}

RETRO_API void retro_reset(void) {}

void reportToLog(int severity, int channelID, char* message, ...) {
    (void)severity; (void)channelID; (void)message;
}

int vmsettings_key_equals(const char* k, const char* cmp) {
    (void)k; (void)cmp;
    return 0;
}

RETRO_API size_t retro_serialize_size(void) { return 0; }
RETRO_API bool retro_serialize(void *data, size_t size) { (void)data; (void)size; return false; }
RETRO_API bool retro_unserialize(const void *data, size_t size) { (void)data; (void)size; return false; }
RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char *code) { (void)index; (void)enabled; (void)code; }
RETRO_API bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) { (void)game_type; (void)info; (void)num_info; return false; }
RETRO_API unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
RETRO_API void *retro_get_memory_data(unsigned id) { (void)id; return NULL; }
RETRO_API size_t retro_get_memory_size(unsigned id) { (void)id; return 0; }
