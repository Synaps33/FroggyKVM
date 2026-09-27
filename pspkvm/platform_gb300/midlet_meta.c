#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "midlet_meta.h"
#include "puff.h"

extern void xlog(const char *fmt, ...);

static char s_main_class[256] = {0};

/* Extract class name from a line starting with MIDlet-1: */
static const char* parse_midlet1_line(const char *line) {
    /* Format: MIDlet-1: <name>, <icon>, <class> */
    const char *p = strchr(line, ':');
    if (!p) return NULL;
    p++; /* skip ':' */

    /* Skip to first comma */
    p = strchr(p, ',');
    if (!p) return NULL;
    p++; /* skip first comma */

    /* Skip to second comma */
    p = strchr(p, ',');
    if (!p) return NULL;
    p++; /* skip second comma */

    /* Skip leading whitespace */
    while (*p == ' ' || *p == '\t') p++;

    /* Copy class name until whitespace, comma, or newline */
    int idx = 0;
    while (*p && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' && *p != ',' && idx < (int)(sizeof(s_main_class) - 1)) {
        s_main_class[idx++] = *p++;
    }
    s_main_class[idx] = '\0';

    if (idx > 0) {
        return s_main_class;
    }
    return NULL;
}

/* Parse MIDlet-1 from a text buffer (JAD or MANIFEST.MF) */
static const char* parse_midlet1_from_buffer(const char *buf, size_t len) {
    const char *cur = buf;
    const char *end = buf + len;

    while (cur < end) {
        /* Find line end */
        const char *line_end = cur;
        while (line_end < end && *line_end != '\r' && *line_end != '\n') {
            line_end++;
        }

        /* Check if line starts with MIDlet-1: (case-insensitive) */
        size_t line_len = line_end - cur;
        if (line_len >= 9 && strncasecmp(cur, "MIDlet-1:", 9) == 0) {
            char temp_line[512];
            size_t copy_len = (line_len < sizeof(temp_line) - 1) ? line_len : sizeof(temp_line) - 1;
            memcpy(temp_line, cur, copy_len);
            temp_line[copy_len] = '\0';

            const char *cls = parse_midlet1_line(temp_line);
            if (cls) {
                xlog("[GB300 META] Found main class: '%s'\n", cls);
                return cls;
            }
        }

        /* Advance to next line */
        cur = line_end;
        while (cur < end && (*cur == '\r' || *cur == '\n')) {
            cur++;
        }
    }
    return NULL;
}

/* Try reading from a .jad file on disk */
static const char* try_parse_jad(const char *jad_path) {
    FILE *f = fopen(jad_path, "r");
    if (!f) return NULL;

    char line[512];
    const char *found = NULL;
    while (fgets(line, sizeof(line), f)) {
        if (strncasecmp(line, "MIDlet-1:", 9) == 0) {
            found = parse_midlet1_line(line);
            if (found) {
                xlog("[GB300 META] Found class in JAD '%s': '%s'\n", jad_path, found);
                break;
            }
        }
    }
    fclose(f);
    return found;
}

static inline uint16_t read_u16_le(const unsigned char *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static inline uint32_t read_u32_le(const unsigned char *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

/* Try reading META-INF/MANIFEST.MF from a .jar (ZIP archive) */
unsigned char* gb300_read_jar_entry(const char *jar_path, const char *entry_name, size_t *out_size) {
    if (out_size) *out_size = 0;
    if (!jar_path || !entry_name) return NULL;

    const char *target = entry_name;
    while (*target == '/') target++;

    FILE *f = fopen(jar_path, "rb");
    if (!f) return NULL;

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long file_size = ftell(f);
    if (file_size < 22) {
        fclose(f);
        return NULL;
    }

    /* Search backwards for ZIP End of Central Directory (EOCD: 0x06054b50) */
    /* Most ZIPs have no comment, so EOCD is within the last 2KB. Try 2KB first. */
    long search_len = (file_size < 2048) ? file_size : 2048;
    long search_pos = file_size - search_len;
    if (fseek(f, search_pos, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }

    unsigned char *search_buf = (unsigned char*)malloc(search_len);
    if (!search_buf) {
        fclose(f);
        return NULL;
    }

    if (fread(search_buf, 1, search_len, f) != (size_t)search_len) {
        free(search_buf);
        fclose(f);
        return NULL;
    }

    long eocd_offset = -1;
    for (long i = search_len - 22; i >= 0; i--) {
        if (search_buf[i] == 0x50 && search_buf[i+1] == 0x4b &&
            search_buf[i+2] == 0x05 && search_buf[i+3] == 0x06) {
            eocd_offset = i;
            break;
        }
    }

    /* Fallback: if not found in last 2KB, search full 65KB window */
    if (eocd_offset < 0 && file_size > search_len) {
        free(search_buf);
        search_len = (file_size < 65558) ? file_size : 65558;
        search_pos = file_size - search_len;
        if (fseek(f, search_pos, SEEK_SET) != 0) {
            fclose(f);
            return NULL;
        }
        search_buf = (unsigned char*)malloc(search_len);
        if (!search_buf) {
            fclose(f);
            return NULL;
        }
        if (fread(search_buf, 1, search_len, f) != (size_t)search_len) {
            free(search_buf);
            fclose(f);
            return NULL;
        }
        for (long i = search_len - 22; i >= 0; i--) {
            if (search_buf[i] == 0x50 && search_buf[i+1] == 0x4b &&
                search_buf[i+2] == 0x05 && search_buf[i+3] == 0x06) {
                eocd_offset = i;
                break;
            }
        }
    }

    if (eocd_offset < 0) {
        free(search_buf);
        fclose(f);
        return NULL;
    }

    uint16_t total_entries = read_u16_le(search_buf + eocd_offset + 10);
    uint32_t cd_size = read_u32_le(search_buf + eocd_offset + 12);
    uint32_t cd_offset = read_u32_le(search_buf + eocd_offset + 16);
    free(search_buf);

    if (cd_offset + cd_size > (uint32_t)file_size || cd_size == 0) {
        fclose(f);
        return NULL;
    }

    unsigned char *cd_buf = (unsigned char*)malloc(cd_size);
    if (!cd_buf) {
        fclose(f);
        return NULL;
    }

    if (fseek(f, cd_offset, SEEK_SET) != 0 ||
        fread(cd_buf, 1, cd_size, f) != cd_size) {
        free(cd_buf);
        fclose(f);
        return NULL;
    }

    unsigned char *result_data = NULL;
    size_t result_size = 0;
    uint32_t pos = 0;
    for (uint16_t entry = 0; entry < total_entries && pos + 46 <= cd_size; entry++) {
        if (cd_buf[pos] != 0x50 || cd_buf[pos+1] != 0x4b ||
            cd_buf[pos+2] != 0x01 || cd_buf[pos+3] != 0x02) {
            break;
        }

        uint16_t method = read_u16_le(cd_buf + pos + 10);
        uint32_t comp_size = read_u32_le(cd_buf + pos + 20);
        uint32_t uncomp_size = read_u32_le(cd_buf + pos + 24);
        uint16_t name_len = read_u16_le(cd_buf + pos + 28);
        uint16_t extra_len = read_u16_le(cd_buf + pos + 30);
        uint16_t comment_len = read_u16_le(cd_buf + pos + 32);
        uint32_t local_header_offset = read_u32_le(cd_buf + pos + 42);

        if (pos + 46 + name_len > cd_size) break;

        char filename[256];
        size_t ncopy = (name_len < sizeof(filename) - 1) ? name_len : sizeof(filename) - 1;
        memcpy(filename, cd_buf + pos + 46, ncopy);
        filename[ncopy] = '\0';

        bool match = (strcmp(filename, target) == 0);
        if (!match && strcasecmp(target, "META-INF/MANIFEST.MF") == 0) {
            match = (strcasecmp(filename, "META-INF/MANIFEST.MF") == 0);
        }

        if (match) {
            if (fseek(f, local_header_offset, SEEK_SET) == 0) {
                unsigned char loc_hdr[30];
                if (fread(loc_hdr, 1, 30, f) == 30 &&
                    loc_hdr[0] == 0x50 && loc_hdr[1] == 0x4b &&
                    loc_hdr[2] == 0x03 && loc_hdr[3] == 0x04) {
                    uint16_t loc_name_len = read_u16_le(loc_hdr + 26);
                    uint16_t loc_extra_len = read_u16_le(loc_hdr + 28);
                    long data_offset = local_header_offset + 30 + loc_name_len + loc_extra_len;

                    if (fseek(f, data_offset, SEEK_SET) == 0) {
                        if (method == 0) {
                            result_data = (unsigned char*)malloc(comp_size + 128);
                            if (result_data && fread(result_data, 1, comp_size, f) == comp_size) {
                                result_size = comp_size;
                                result_data[result_size] = '\0';
                            } else if (result_data) {
                                free(result_data);
                                result_data = NULL;
                            }
                        } else if (method == 8) {
                            unsigned char *comp_buf = (unsigned char*)malloc(comp_size);
                            unsigned long max_uncomp = (uncomp_size > 0 && uncomp_size < 1048576) ? uncomp_size : 65536;
                            result_data = (unsigned char*)malloc(max_uncomp + 128);

                            if (comp_buf && result_data && fread(comp_buf, 1, comp_size, f) == comp_size) {
                                unsigned long dst_len = max_uncomp;
                                unsigned long src_len = comp_size;
                                int puff_err = puff(result_data, &dst_len, comp_buf, &src_len);
                                if (puff_err == 0 && dst_len > 0) {
                                    result_size = dst_len;
                                    result_data[result_size] = '\0';
                                } else {
                                    free(result_data);
                                    result_data = NULL;
                                }
                            } else if (result_data) {
                                free(result_data);
                                result_data = NULL;
                            }
                            if (comp_buf) free(comp_buf);
                        }
                    }
                }
            }
            break;
        }

        pos += 46 + name_len + extra_len + comment_len;
    }

    free(cd_buf);
    fclose(f);

    if (out_size) *out_size = result_size;
    return result_data;
}

static const char* try_parse_jar_manifest(const char *jar_path) {
    size_t sz = 0;
    unsigned char *buf = gb300_read_jar_entry(jar_path, "META-INF/MANIFEST.MF", &sz);
    if (!buf) return NULL;
    const char *res = parse_midlet1_from_buffer((char*)buf, sz);
    free(buf);
    return res;
}

const char* gb300_get_manifest_property(const char *jar_path, const char *key) {
    if (!jar_path || !key) return NULL;
    static char s_val[256];
    size_t sz = 0;
    unsigned char *buf = gb300_read_jar_entry(jar_path, "META-INF/MANIFEST.MF", &sz);
    if (!buf) return NULL;

    const char *p = (const char*)buf;
    const char *end = p + sz;
    size_t klen = strlen(key);

    while (p < end) {
        while (p < end && (*p == '\r' || *p == '\n')) p++;
        if (p >= end) break;

        const char *line_start = p;
        while (p < end && *p != '\r' && *p != '\n') p++;
        const char *line_end = p;

        if ((size_t)(line_end - line_start) > klen + 1 &&
            strncasecmp(line_start, key, klen) == 0 &&
            line_start[klen] == ':') {
            const char *val_start = line_start + klen + 1;
            while (val_start < line_end && (*val_start == ' ' || *val_start == '\t')) val_start++;
            size_t val_len = line_end - val_start;
            if (val_len >= sizeof(s_val)) val_len = sizeof(s_val) - 1;
            memcpy(s_val, val_start, val_len);
            s_val[val_len] = '\0';
            free(buf);
            return s_val;
        }
    }

    free(buf);
    return NULL;
}

const char* gb300_get_main_class(const char *rom_path) {
    if (!rom_path || strlen(rom_path) == 0) return NULL;

    /* 1. If rom_path is .jad, try it directly */
    size_t len = strlen(rom_path);
    if (len > 4 && strcasecmp(rom_path + len - 4, ".jad") == 0) {
        const char *cls = try_parse_jad(rom_path);
        if (cls) return cls;
    }

    /* 2. Check if a sibling .jad exists next to .jar */
    if (len > 4 && strcasecmp(rom_path + len - 4, ".jar") == 0) {
        char jad_sibling[512];
        strncpy(jad_sibling, rom_path, len - 4);
        jad_sibling[len - 4] = '\0';
        strcat(jad_sibling, ".jad");

        const char *cls = try_parse_jad(jad_sibling);
        if (cls) return cls;

        /* 3. Parse manifest inside the .jar */
        cls = try_parse_jar_manifest(rom_path);
        if (cls) return cls;
    }

    /* 4. If rom_path was .jad and didn't have class, try sibling .jar */
    if (len > 4 && strcasecmp(rom_path + len - 4, ".jad") == 0) {
        char jar_sibling[512];
        strncpy(jar_sibling, rom_path, len - 4);
        jar_sibling[len - 4] = '\0';
        strcat(jar_sibling, ".jar");

        const char *cls = try_parse_jar_manifest(jar_sibling);
        if (cls) return cls;
    }

    return NULL;
}

bool gb300_detect_screen_size(const char *rom_path, int *out_w, int *out_h) {
    if (!rom_path || !out_w || !out_h) return false;

    /* 1. Check Nokia-MIDlet-Original-Display-Size / Target / Screen-Size */
    const char *keys[] = {
        "Nokia-MIDlet-Original-Display-Size",
        "Nokia-MIDlet-Target-Display-Size",
        "MIDlet-Screen-Size",
        "MIDlet-Display-Size",
        NULL
    };
    for (int i = 0; keys[i]; i++) {
        const char *val = gb300_get_manifest_property(rom_path, keys[i]);
        if (val) {
            int w = 0, h = 0;
            if (sscanf(val, "%d,%d", &w, &h) == 2 || sscanf(val, "%dx%d", &w, &h) == 2) {
                if (w > 0 && h > 0 && w <= 512 && h <= 512) {
                    *out_w = w;
                    *out_h = h;
                    return true;
                }
            }
        }
    }

    /* 2. Check filename heuristics */
    const char *p = rom_path;
    const char *last_slash = strrchr(rom_path, '/');
    if (last_slash) p = last_slash + 1;

    char lower_name[256];
    size_t nlen = strlen(p);
    if (nlen >= sizeof(lower_name)) nlen = sizeof(lower_name) - 1;
    for (size_t i = 0; i < nlen; i++) lower_name[i] = (char)tolower((unsigned char)p[i]);
    lower_name[nlen] = '\0';

    if (strstr(lower_name, "shepherdess")) {
        *out_w = 128;
        *out_h = 128;
        return true;
    }
    if (strstr(lower_name, "s60") || strstr(lower_name, "3650") || strstr(lower_name, "7650") || strstr(lower_name, "6600")) {
        *out_w = 176;
        *out_h = 208;
        return true;
    }
    if (strstr(lower_name, "s40") || strstr(lower_name, "7210") || strstr(lower_name, "6610") || 
        strstr(lower_name, "6100") || strstr(lower_name, "6230") || strstr(lower_name, "3100")) {
        *out_w = 128;
        *out_h = 128;
        return true;
    }

    /* 3. Check MIDlet-Name in manifest */
    const char *mname = gb300_get_manifest_property(rom_path, "MIDlet-Name");
    if (mname) {
        char lower_mname[256];
        size_t mlen = strlen(mname);
        if (mlen >= sizeof(lower_mname)) mlen = sizeof(lower_mname) - 1;
        for (size_t i = 0; i < mlen; i++) lower_mname[i] = (char)tolower((unsigned char)mname[i]);
        lower_mname[mlen] = '\0';
        if (strstr(lower_mname, "shepherdess")) {
            *out_w = 128;
            *out_h = 128;
            return true;
        }
    }

    return false;
}
