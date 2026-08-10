#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "dirent.h"
#include "psp_compat.h"

#define J2ME_ROM_DIR  "/ROMS/J2ME"
#define J2ME_SAVE_DIR "/SAVES/J2ME"

static char current_rom_path[512] = {0};
static char current_jar_path[512] = {0};

void gb300_fs_init(void) {
#if defined(_WIN32)
    mkdir(J2ME_SAVE_DIR);
#else
    mkdir(J2ME_SAVE_DIR, 0777);
#endif
}

void gb300_fs_set_rom(const char *path) {
    if (!path) return;
    strncpy(current_rom_path, path, sizeof(current_rom_path) - 1);
    current_rom_path[sizeof(current_rom_path) - 1] = '\0';

    /* Check if extension is .jad */
    size_t len = strlen(current_rom_path);
    if (len > 4 && strcasecmp(current_rom_path + len - 4, ".jad") == 0) {
        /* Derive matching .jar path */
        strncpy(current_jar_path, current_rom_path, len - 4);
        current_jar_path[len - 4] = '\0';
        strcat(current_jar_path, ".jar");

        FILE *f = fopen(current_jar_path, "rb");
        if (f) {
            fclose(f);
            printf("[GB300 FS] JAD launched. Found matching JAR: %s\n", current_jar_path);
        } else {
            printf("[GB300 FS] Warning: JAD launched but matching JAR not found: %s\n", current_jar_path);
        }
    } else {
        strncpy(current_jar_path, current_rom_path, sizeof(current_jar_path) - 1);
    }
}

const char* gb300_fs_get_rom(void) {
    return current_rom_path;
}

const char* gb300_fs_get_jar(void) {
    return current_jar_path;
}
