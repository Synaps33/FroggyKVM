#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libretro.h"
#include "psp_compat.h"
#include "midlet_meta.h"
#include "hacker_loader.h"

/* Declarations for GB300 platform sub-modules */
void gb300_video_init(void);
uint16_t* gb300_video_get_framebuffer(void);
void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch);
void gb300_video_draw_splash(const char *title, const char *rom_name, const char *status);

void gb300_audio_init(void);
void gb300_audio_deinit(void);
int gb300_audio_read(int16_t *dst, int num_frames);

void gb300_input_init(void);
void gb300_input_load_config(const char *jar_path);
void gb300_input_poll(uint32_t current_buttons);

void gb300_fs_init(void);
void gb300_fs_set_rom(const char *path);
const char* gb300_fs_get_jar(void);

void JavaTask(void);
void javanotify_start_java_with_arbitrary_args(int argc, char* argv[]);

#if defined(SF2000) || defined(GB300)
extern void dly_tsk(int ms);
#else
#include <unistd.h>
#define dly_tsk(ms) usleep((ms) * 1000)
#endif

#include <stdarg.h>
__attribute__((weak)) void xlog(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    fflush(stdout);
}

/* Libretro callback function pointers */
#include <setjmp.h>

static jmp_buf s_exit_jmp;
static volatile int s_exit_requested = 0;
static volatile int s_can_exit_jmp = 0;

static retro_video_refresh_t video_cb = NULL;
static retro_audio_sample_t audio_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_input_poll_t input_poll_cb = NULL;
static retro_input_state_t input_state_cb = NULL;
static retro_environment_t environ_cb = NULL;

static void request_libretro_shutdown(void) {
    if (environ_cb)
        environ_cb(RETRO_ENVIRONMENT_SHUTDOWN, NULL);
}

static bool game_loaded = false;
static bool jvm_started = false;

static bool j2me_classes_available(void) {
    static const char *paths[] = {
        FROGGY_SD_ROOT "/cubegm/bios/classes.zip",
        FROGGY_SD_ROOT "/bios/classes.zip",
        FROGGY_SD_ROOT "/BIOS/classes.zip",
        FROGGY_SD_ROOT "/cubegm/cores/j2me/classes.zip",
        FROGGY_SD_ROOT "/roms/j2me/classes.zip",
        /* Local build/test trees: the desktop runner has no /mnt/sda1. */
        "/home/Sajnaps/gb300/bios/classes.zip",
        "/home/Sajnaps/gb300/froggykvm/classes.zip",
        "classes.zip",
        NULL
    };
    for (int i = 0; paths[i]; i++) {
        FILE *f = fopen(paths[i], "rb");
        if (f) {
            fclose(f);
            return true;
        }
    }
    return false;
}

/* Declared weak: only the SF2000 multicore firmware provides this symbol.
 * The desktop test runner does not, and a hard dependency made the whole
 * library fail to dlopen. With a weak reference the call is simply skipped
 * and we fall back to the old "exit to FrogUI" behaviour. */
extern int frontend_open_pause_menu(void) __attribute__((weak));

#if defined(SF2000) || defined(GB300)
extern volatile uint32_t g_joy_task_state;
extern volatile uint32_t g_joy_state;
extern void frontend_check_hotkeys(void);
extern void shutdown_game(void);

/* Firmware pause menu (Resume / Reboot / Save / Load), including the
 * framebuffer snapshot the save-state path needs. Lives in the frontend so
 * that this core and the firmware's own dummy_run_emulator_menu() share one
 * implementation. Returns 0 to resume. */
#define SF2000_HW_UP       0x0010
#define SF2000_HW_DOWN     0x0040
#define SF2000_HW_LEFT     0x0080
#define SF2000_HW_RIGHT    0x0020
#define SF2000_HW_SELECT   0x0001
#define SF2000_HW_START    0x0008
#define SF2000_HW_A        0x2000
#define SF2000_HW_B        0x4000
#define SF2000_HW_X        0x0400
#define SF2000_HW_Y        0x0800
#define SF2000_HW_L        0x1000
#define SF2000_HW_R        0x8000

static uint32_t poll_gb300_buttons(void) {
    uint32_t raw = g_joy_task_state;
    g_joy_state = raw;

    uint32_t mask = 0;
    if (raw & SF2000_HW_UP)     mask |= PSP_CTRL_UP;
    if (raw & SF2000_HW_DOWN)   mask |= PSP_CTRL_DOWN;
    if (raw & SF2000_HW_LEFT)   mask |= PSP_CTRL_LEFT;
    if (raw & SF2000_HW_RIGHT)  mask |= PSP_CTRL_RIGHT;
    if (raw & SF2000_HW_A)      mask |= PSP_CTRL_CROSS;     /* RetroPad A -> PSP CROSS -> J2ME 5 */
    if (raw & SF2000_HW_B)      mask |= PSP_CTRL_CIRCLE;    /* RetroPad B -> PSP CIRCLE -> J2ME Soft1 */
    if (raw & SF2000_HW_X)      mask |= PSP_CTRL_SQUARE;    /* RetroPad X -> PSP SQUARE -> J2ME Soft2 */
    if (raw & SF2000_HW_Y)      mask |= PSP_CTRL_TRIANGLE;  /* RetroPad Y -> PSP TRIANGLE -> J2ME 5 */
    if (raw & SF2000_HW_L)      mask |= PSP_CTRL_LTRIGGER;  /* L -> PSP LTRIGGER -> J2ME 1 */
    if (raw & SF2000_HW_R)      mask |= PSP_CTRL_RTRIGGER;  /* R -> PSP RTRIGGER -> J2ME 3 */
    if (raw & SF2000_HW_SELECT) mask |= PSP_CTRL_SELECT;    /* SELECT -> PSP SELECT -> J2ME # */
    if (raw & SF2000_HW_START)  mask |= PSP_CTRL_START;     /* START -> PSP START -> J2ME Soft1 */

    return mask;
}
#else
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
#endif

RETRO_API unsigned retro_api_version(void) {
    return RETRO_API_VERSION;
}

RETRO_API void retro_set_environment(retro_environment_t cb) {
    environ_cb = cb;
    if (environ_cb) {
        enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
        environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);

        struct retro_input_descriptor desc[] = {
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "D-Pad Left" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "D-Pad Up" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "D-Pad Down" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "D-Pad Right" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "Fire / Select" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "Soft 1 (Left Softkey)" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "Soft 2 (Right Softkey)" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,      "Key 5 (Num 5 / Action)" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "Key 1" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "Key 3" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Key #" },
            { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Menu / Soft 1" },
            { 0, 0, 0, 0, NULL }
        };
        environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
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
    jvm_started = false;
    xlog("[PSPKVM-GB300] retro_init: Initialization complete.\n");
}

RETRO_API void retro_deinit(void) {
    xlog("[PSPKVM-GB300] retro_deinit: Deinitializing core...\n");
    gb300_audio_deinit();
    game_loaded = false;
    jvm_started = false;
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
    if (!j2me_classes_available()) {
        xlog("[PSPKVM-GB300] Missing classes.zip; install it in /cubegm/bios/classes.zip\n");
        return false;
    }
    if (game && game->path) {
        xlog("[PSPKVM-GB300] retro_load_game: Loading ROM file '%s'\n", game->path);
        gb300_fs_set_rom(game->path);
        gb300_input_load_config(game->path);
    } else {
        xlog("[PSPKVM-GB300] retro_load_game: Running in standalone/stub mode\n");
        gb300_fs_set_rom(FROGGY_SD_ROOT "/roms/j2me/stub.jar");
        gb300_input_load_config(FROGGY_SD_ROOT "/roms/j2me/stub.jar");
    }
    game_loaded = true;
    jvm_started = false;
    return true;
}

RETRO_API void retro_unload_game(void) {
    xlog("[PSPKVM-GB300] retro_unload_game: Game unloaded\n");
    game_loaded = false;
    jvm_started = false;
}

retro_video_refresh_t gb300_get_video_cb(void) {
    return video_cb;
}

/* Called by the Java VM whenever waiting for events or yielding CPU */
extern void gb300_check_timers(void);

/* Hard flag ensuring multicore skips pause menu entirely for J2ME */
int g_is_j2me_core = 1;

void gb300_poll_events(void) {
    /* 0. Dispatch software timers */
    gb300_check_timers();

    extern uint64_t gb300_timer_get_us(void);
    uint64_t now = gb300_timer_get_us();

    /* 1. Poll input, throttled to 60fps (16.6ms) */
    static uint64_t last_input_time = 0;
    if (last_input_time == 0 || now - last_input_time >= 16666) {
        last_input_time = now;
        
        if (input_poll_cb) input_poll_cb();
#if defined(SF2000) || defined(GB300)
        frontend_check_hotkeys();
#endif
        uint32_t buttons = poll_gb300_buttons();
        gb300_input_poll(buttons);

        /* 2. SELECT + START opens the firmware pause menu
         *    (Resume / Reboot / Save / Load) instead of bailing out of the
         *    Java task, which used to look like a plain restart to the user.
         *    Resume returns 0 and the game carries on; anything else exits. */
        if ((buttons & (PSP_CTRL_SELECT | PSP_CTRL_START)) == (PSP_CTRL_SELECT | PSP_CTRL_START)) {
            int response;
            /* Swallow the combo: run_emulator_menu() polls input itself and a
             * still-held SELECT+START would immediately re-trigger. */
            if (frontend_open_pause_menu == 0) {
                xlog("[PSPKVM-GB300] pause menu unavailable -> exit to FrogUI\n");
                response = 1;
            } else {
                xlog("[PSPKVM-GB300] SELECT+START -> firmware pause menu\n");
                response = frontend_open_pause_menu();
                xlog("[PSPKVM-GB300] pause menu response = %d\n", response);
            }
            if (response != 0) {
                s_exit_requested = 1;
                if (s_can_exit_jmp) {
                    longjmp(s_exit_jmp, 1);
                }
            }
        }
    }

    /* 3. Feed the frontend in stable, frame-sized batches. The VM can poll
     * very frequently; 5ms batches underfill the 48kHz device queue. */
    static uint64_t last_audio_time = 0;
    if (last_audio_time == 0 || now < last_audio_time) last_audio_time = now;
    
    int64_t delta_us = now - last_audio_time;
    if (delta_us > 100000) {
        /* Drop audio debt if game was loading/frozen for >100ms */
        last_audio_time = now;
        delta_us = 0;
    }
    
    if (delta_us >= 20000) {
        int frames = (int)((delta_us * 22050) / 1000000LL);
        if (frames > 0) {
            if (frames > 1024) {
                frames = 1024;
                last_audio_time = now - (1024 * 1000000LL) / 22050;
            }
            static int16_t pcm_samples[2048 * 2];
            int frames_read = gb300_audio_read(pcm_samples, frames);
            if (audio_batch_cb && frames_read > 0) {
                audio_batch_cb(pcm_samples, frames_read);
            }
            last_audio_time += (frames * 1000000LL) / 22050;
        }
    }
}

void gb300_jvm_yield(void) {
    gb300_poll_events();

    /* Delay to let the OS / watchdog breathe when JVM has no active work */
    dly_tsk(1);
}

#if defined(__linux__) && !defined(SF2000)
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

extern void dump_java_stack(void);

static void dump_stack(int sig) {
    (void)sig;
    const char msg[] = "\n=== FROGGY_TIMEOUT ALARM TRIGGERED ===\n";
    write(2, msg, sizeof(msg) - 1);
    dump_java_stack();
    void *bt[64];
    int n = backtrace(bt, 64);
    backtrace_symbols_fd(bt, n, 2);
    _exit(1);
}
#endif

RETRO_API void retro_run(void) {
    if (!game_loaded) return;

    if (!jvm_started) {
        jvm_started = true;

#if defined(__linux__) && !defined(SF2000)
        const char *tmo_env = getenv("FROGGY_TIMEOUT");
        if (tmo_env && atoi(tmo_env) > 0) {
            signal(SIGALRM, dump_stack);
            alarm(atoi(tmo_env));
        }
#endif

        const char *rom = gb300_fs_get_jar();

        /* Initialize Hacker Console Boot Screen */
        gb300_hacker_init(rom);

        gb300_hacker_log("JAR", "PARSING MANIFEST.MF ...", 18);
        const char *main_class = gb300_get_main_class(rom);
        if (main_class && *main_class) {
            gb300_hacker_set_main_class(main_class);
            gb300_hacker_log("META", "ENTRY POINT RESOLVED", 25);
        } else {
            gb300_hacker_log("META", "AUTO-DETECT MIDLET ENTRY", 25);
        }

        gb300_hacker_log("JVM", "INIT CLDC 1.1 / MIDP 2.0", 35);

        char *argv[5];
        int argc = 4;
        argv[0] = "pspkvm";
        argv[1] = (char*)(rom ? rom : "");
        argv[2] = (char*)(main_class && *main_class ? main_class : "internal");
        argv[3] = (char*)(rom ? rom : "");
        argv[4] = NULL;

        extern int JVM_SetUseVerifier(int use_verifier);
        JVM_SetUseVerifier(0);

        gb300_hacker_log("OPT", "VERIFIER BYPASS [OK]", 42);

        xlog("[PSPKVM-GB300] Starting Java VM with argc=%d, argv[1]='%s', argv[2]='%s', argv[3]='%s'\n",
             argc, argv[1], argv[2], argv[3]);
        javanotify_start_java_with_arbitrary_args(argc, argv);

        gb300_hacker_log("KVM", "STARTING JAVATASK THREAD", 50);

        xlog("[PSPKVM-GB300] Entering JavaTask()...\n");
        s_can_exit_jmp = 1;
        if (setjmp(s_exit_jmp) == 0) {
            JavaTask();
            s_can_exit_jmp = 0;
            xlog("[PSPKVM-GB300] JavaTask() finished normally.\n");
        } else {
            s_can_exit_jmp = 0;
            xlog("[PSPKVM-GB300] Returned from JavaTask() via hotkey exit.\n");
        }

        if (s_exit_requested) {
            dly_tsk(16);
            request_libretro_shutdown();
            return;
        }

        /* JVM finished normally (game ended) */
        gb300_hacker_exit(0);
        s_exit_requested = 1;
        dly_tsk(16);
        request_libretro_shutdown();
        return;
    }

    /* JVM has exited -- keep pushing last frame so firmware doesn't freeze */
    {
        uint16_t *fb = gb300_video_get_framebuffer();
        if (video_cb && fb) {
            video_cb(fb, 320, 240, 320 * sizeof(uint16_t));
        }
    }
    gb300_jvm_yield();
}

RETRO_API void retro_reset(void) {
    /* Prevent reset crash from any unexpected menu interaction */
    xlog("[PSPKVM-GB300] retro_reset triggered: IGNORING.\n");
}

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
