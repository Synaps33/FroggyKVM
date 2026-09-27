#ifndef _MIDLET_META_H_
#define _MIDLET_META_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Returns the fully-qualified Java class name of the main MIDlet
 * (from MIDlet-1 in .jad or META-INF/MANIFEST.MF in .jar).
 * Returns NULL if not found or on error.
 */
const char* gb300_get_main_class(const char *rom_path);

/*
 * Returns property value from META-INF/MANIFEST.MF in .jar.
 * Returns NULL if not found or on error.
 */
const char* gb300_get_manifest_property(const char *jar_path, const char *key);

/*
 * Attempts to detect target display resolution (width and height)
 * based on manifest properties and filename conventions.
 * Returns true if a specific resolution was detected, false otherwise.
 */
bool gb300_detect_screen_size(const char *rom_path, int *out_w, int *out_h);

/*
 * Reads and decompresses an entry from a JAR archive.
 * Returns dynamically allocated buffer that caller must free(),
 * and writes entry length into *out_size.
 * Returns NULL on error.
 */
unsigned char* gb300_read_jar_entry(const char *jar_path, const char *entry_name, size_t *out_size);

#ifdef __cplusplus
}
#endif

#endif /* _MIDLET_META_H_ */
