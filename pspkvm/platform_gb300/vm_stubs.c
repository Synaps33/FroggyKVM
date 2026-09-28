#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "psp_compat.h"
#include <kni.h>
#include <sni.h>
#include <midpUtilKni.h>
#include "midlet_meta.h"

int gForegroundIsolateId = 1;
int gForegroundDisplayId = 1;

void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch);
const char* gb300_fs_get_jar(void);

/* Properties / Config stubs */
int javacall_initialize_configurations(void) {
    xlog("[GB300-STUB] javacall_initialize_configurations()\n");
    return 0;
}
void javacall_finalize_configurations(void) {
    xlog("[GB300-STUB] javacall_finalize_configurations()\n");
}

/* MIDP AMS stubs */
void* midpRemoveOptionFlag(const char* flag, char** argv, int* argc) {
    (void)flag; (void)argv; (void)argc;
    return NULL;
}
char* midpRemoveCommandOption(const char* opt, char** argv, int* argc) {
    (void)opt; (void)argv; (void)argc;
    return NULL;
}
char* midpFixMidpHome(const char* prog) {
    (void)prog;
    xlog("[GB300-STUB] midpFixMidpHome returning '/mnt/sda1/ROMS/J2ME'\n");
    return "/mnt/sda1/ROMS/J2ME";
}

void midpMIDletProxyListReset(void) {
    gForegroundDisplayId = 1;
}

const char* midp_suite_get_suite_storage(int id) {
    (void)id;
    return "/mnt/sda1/SAVES/J2ME";
}
const char* midp_suite_get_class_path(int id) { (void)id; return NULL; }
char storageGetPathSeparator(void) { return ':'; }
int javacall_get_property(const char* key, int type, char** value) {
    (void)key; (void)type;
    if (value) *value = NULL;
    return -1;
}
char* getLocalTimeZone(void) { return "GMT"; }
int javacall_devemu_get_current_device(void) { return 0; }
int javacall_devemu_get_rotation(int dev) { (void)dev; return 0; }
char* javacall_devemu_get_device_pid(int dev) { (void)dev; return "Default"; }

KNIEXPORT KNI_RETURNTYPE_INT
KNIDECL(com_sun_midp_installer_DeviceDesc_getDeviceKeyCode0) {
    int index = KNI_GetParameterAsInt(1);
    int key = KNI_GetParameterAsInt(2);
    xlog("[DEVEDESC] getDeviceKeyCode0(dev=%d, key=%d) -> %d\n", index, key, key);
    KNI_ReturnInt(key);
}

KNIEXPORT KNI_RETURNTYPE_INT
KNIDECL(com_sun_midp_installer_DeviceDesc_devIdToDispId) {
    int id = KNI_GetParameterAsInt(1);
    KNI_ReturnInt(id);
}

KNIEXPORT KNI_RETURNTYPE_INT
KNIDECL(com_sun_midp_installer_DeviceDesc_dispIdToDevId) {
    int id = KNI_GetParameterAsInt(1);
    KNI_ReturnInt(id);
}
int unicodeToNative(const unsigned short *ustr, int ulen, unsigned char *bstr, int blen) {
    int i = 0;
    if (ustr && bstr && blen > 0) {
        for (i = 0; i < ulen && i < blen - 1; i++) {
            bstr[i] = (unsigned char)(ustr[i] & 0xFF);
        }
        bstr[i] = '\0';
    }
    return i;
}
void* pcsl_mem_malloc_impl0(size_t sz) { return malloc(sz); }
void* pcsl_mem_calloc_impl0(size_t n, size_t sz) { return calloc(n, sz); }
void* pcsl_mem_realloc_impl0(void* p, size_t sz) { return realloc(p, sz); }
void pcsl_mem_free_chunk(void* p) { if (p) free(p); }
void pcsl_mem_free_impl0(void* p) { if (p) free(p); }
void pss(void) {}

int midp_get_suite_ids(void **ids, int *num) {
    if (ids) *ids = NULL;
    if (num) *num = 0;
    return 0;
}
void midp_free_suite_ids(void *ids, int num) { (void)ids; (void)num; }

int find_midlet_class(int id, int num, void** res) {
    (void)id; (void)num;
    const char *rom = gb300_fs_get_jar();
    const char *cls = gb300_get_main_class(rom);
    if (cls && res) {
        if (pcsl_string_from_chars(cls, (pcsl_string*)res) == PCSL_STRING_OK) {
            xlog("[GB300-STUB] find_midlet_class resolved main class: %s\n", cls);
            return 1;
        }
    }
    if (res) *res = NULL;
    return 0; /* 0 = error / not found */
}

void midp_suite_storage_cleanup(void) {}
void sr_finalizeSystem(void) {}
void finalizeConfig(void) {}
void storageFinalize(void) {}
void sr_initSystem(void) {}
int storageInitialize(void) { return 0; }
int midp_suite_storage_init(void) { return 0; }
static const char* gb300_lookup_system_property(const char *key) {
    if (!key) return NULL;
    if (strcmp(key, "microedition.platform") == 0) return "Nokia3650";
    if (strcmp(key, "microedition.configuration") == 0) return "CLDC-1.1";
    if (strcmp(key, "microedition.profiles") == 0) return "MIDP-2.0";
    if (strcmp(key, "microedition.locale") == 0) return "en-US";
    if (strcmp(key, "microedition.encoding") == 0) return "ISO-8859-1";
    if (strcmp(key, "microedition.commports") == 0) return "";
    if (strcmp(key, "microedition.hostname") == 0) return "localhost";
    if (strcmp(key, "microedition.jtwi.version") == 0) return "1.0";
    if (strncmp(key, "fileconn.dir.", 13) == 0) return "file:///root/";
    if (strstr(key, "WIDTH") || strstr(key, "width")) return "320";
    if (strstr(key, "HEIGHT") || strstr(key, "height")) return "240";

    const char *jar = gb300_fs_get_jar();
    if (jar) {
        const char *val = gb300_get_manifest_property(jar, key);
        if (val) return val;
    }
    return NULL;
}

int initializeConfig(void) { return 0; }
const char* getInternalProperty(const char* k) { return gb300_lookup_system_property(k); }
void storageSetTotalSpace(int sz) { (void)sz; }

int pushopen(void) { return 0; }
int pushclose(void) { return 0; }
int pushcheckinLeftOvers(void) { return 0; }
int pushcheckinall(void) { return 0; }
void sr_repairSystem(void) {}
const char* getSystemProperty(const char* key) {
    const char *val = gb300_lookup_system_property(key);
    xlog("[SYSTEM-PROP] getSystemProperty('%s') -> '%s'\n", key ? key : "(null)", val ? val : "(null)");
    return val;
}

/* Event subsystem stubs */
KNIEXPORT KNI_RETURNTYPE_VOID
KNIDECL(com_sun_midp_main_MIDletProxyList_setForegroundInNativeState) {
    gForegroundIsolateId = KNI_GetParameterAsInt(1);
    gForegroundDisplayId = KNI_GetParameterAsInt(2);
    xlog("[GB300-MIDP] setForegroundInNativeState: isolate=%d display=%d\n",
         gForegroundIsolateId, gForegroundDisplayId);
    KNI_ReturnVoid();
}
int getCurrentIsolateId(void) { return 0; }
jboolean midp_waitWhileSuspended(void) { return KNI_FALSE; }
int findPushBlockedHandle(int h) { (void)h; return 0; }
int findPushTimerBlockedHandle(int h) { (void)h; return 0; }
void midp_createEventQueueLock(void) {}
void midp_destroyEventQueueLock(void) {}
void midp_waitAndLockEventQueue(void) {}
void midp_unlockEventQueue(void) {}
void midp_logThreadId(const char* s) { (void)s; }
void midp_exitVM(int code) { (void)code; }

void storageFreeError(char* pszError) { (void)pszError; }
int storage_open(char** ppszError, const void* filename, int ioMode) {
    (void)filename; (void)ioMode;
    if (ppszError) *ppszError = "not implemented";
    return -1;
}
void storageClose(char** ppszError, int handle) {
    (void)ppszError; (void)handle;
}
long storageRead(char** ppszError, int handle, char* buffer, long length) {
    (void)handle; (void)buffer; (void)length;
    if (ppszError) *ppszError = "not implemented";
    return -1;
}
long storageSizeOf(char** ppszError, int handle) {
    (void)handle;
    if (ppszError) *ppszError = "not implemented";
    return -1;
}

#ifdef __cplusplus
extern "C" {
#endif
// const void* jvm_natives_table[1] = { 0 }; -- now in NativesTable.cpp
void setInternalProperty(const char* k, const char* v) { (void)k; (void)v; }
void setSystemProperty(const char* k, const char* v) { (void)k; (void)v; }
#include <malloc.h>
void* pcsl_mem_allocate_chunk(unsigned int initial_size, unsigned int max_size, unsigned int alignment) {
    if (alignment == 0) alignment = 4;
    void *ptr = memalign(alignment, max_size);
    xlog("[PSPKVM] pcsl_mem_allocate_chunk(init=%u, max=%u, align=%u) -> %p\n",
         initial_size, max_size, alignment, ptr);
    return ptr;
}
void* pcsl_mem_adjust_chunk(void* p, unsigned int sz) { return p; }
void* setup_stack_asm(void* xsp) {
    void** sp = (void**)xsp;
    *--sp = NULL; /* frame pointer */
    *--sp = NULL; /* dummy */
    *--sp = NULL; /* return_point */
    return (void*)sp;
}

/* File and Config stubs for MIDP */
KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_io_j2me_storage_File_initConfigRoot) {
    KNI_StartHandles(1);
    KNI_DeclareHandle(string);
    KNI_NewStringUTF("/tmp/", string);
    KNI_EndHandlesAndReturnObject(string);
}

KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_io_j2me_storage_File_initStorageRoot) {
    KNI_StartHandles(1);
    KNI_DeclareHandle(string);
    KNI_NewStringUTF("/tmp/", string);
    KNI_EndHandlesAndReturnObject(string);
}

KNIEXPORT KNI_RETURNTYPE_LONG
KNIDECL(com_sun_midp_io_j2me_storage_File_availableStorage) {
    KNI_ReturnLong(64LL * 1024 * 1024);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
KNIDECL(com_sun_midp_io_j2me_storage_File_storageExists) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_VOID
KNIDECL(com_sun_midp_io_j2me_storage_File_deleteStorage) {
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_VOID
KNIDECL(com_sun_midp_io_j2me_storage_File_renameStorage) {
    KNI_ReturnVoid();
}

/*
 * Serves MIDP Chameleon skin image resources to ResourceHandler.
 * The Java side asks for names like "screen_image_wash_png" (dots from the
 * original file name are replaced by underscores). We load them from the SD
 * card (or /tmp for desktop testing) and hand them back as a byte[].
 * Also acts as a proper NULL return for anything not found - returning a
 * garbage KNI handle here used to break the skin loader with
 * ArrayIndexOutOfBoundsException.
 */
KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_util_ResourceHandler_loadRomizedResource0) {
    KNI_StartHandles(2);
    KNI_DeclareHandle(hName);
    KNI_DeclareHandle(hReturnArray);
    KNI_GetParameterAsObject(1, hName);
    KNI_ReleaseHandle(hReturnArray);

    if (!KNI_IsNullHandle(hName)) {
        jsize len = KNI_GetStringLength(hName);
        if (len > 0 && len < 200) {
            char name[224];
            jchar ubuf[224];
            KNI_GetStringRegion(hName, 0, len, ubuf);
            for (int i = 0; i < len; i++) name[i] = (char)(ubuf[i] & 0x7F);
            name[len] = '\0';

            static const char *dirs[] = {
                "/mnt/sda1/system/skin/",
                "/tmp/",
                NULL
            };
            for (int d = 0; dirs[d] != NULL; d++) {
                char path[400];
                snprintf(path, sizeof(path), "%s%s", dirs[d], name);
                FILE *f = fopen(path, "rb");
                if (!f) continue;
                fseek(f, 0, SEEK_END);
                long sz = ftell(f);
                fseek(f, 0, SEEK_SET);
                if (sz > 0 && sz < (1024 * 1024)) {
                    unsigned char *buf = (unsigned char *)malloc((size_t)sz);
                    if (buf != NULL) {
                        size_t rd = fread(buf, 1, (size_t)sz, f);
                        if (rd == (size_t)sz) {
                            SNI_NewArray(SNI_BYTE_ARRAY, (jsize)sz, hReturnArray);
                            if (!KNI_IsNullHandle(hReturnArray)) {
                                KNI_SetRawArrayRegion(hReturnArray, 0, (jsize)sz, (jbyte *)buf);
                            }
                        }
                        free(buf);
                    }
                }
                fclose(f);
                break;
            }
        }
    }
    KNI_EndHandlesAndReturnObject(hReturnArray);
}

KNIEXPORT KNI_RETURNTYPE_VOID
KNIDECL(com_sun_midp_log_LoggingBase_report) {
    jint severity = KNI_GetParameterAsInt(1);
    jint channelID = KNI_GetParameterAsInt(2);
    KNI_StartHandles(1);
    KNI_DeclareHandle(msg);
    KNI_GetParameterAsObject(3, msg);
    if (!KNI_IsNullHandle(msg)) {
        jsize len = KNI_GetStringLength(msg);
        char buf[512] = {0};
        jsize copylen = (len < 511) ? len : 511;
        jchar ubuf[512];
        KNI_GetStringRegion(msg, 0, copylen, ubuf);
        for (int i = 0; i < copylen; i++) buf[i] = (char)(ubuf[i] & 0xFF);
        buf[copylen] = '\0';
        xlog("[MIDP-LOG sev=%d ch=%d] %s\n", severity, channelID, buf);
    }
    KNI_EndHandles();
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_main_Configuration_getProperty0) {
    KNI_StartHandles(2);
    KNI_DeclareHandle(key_h);
    KNI_DeclareHandle(res_h);
    KNI_GetParameterAsObject(1, key_h);
    const char* val = NULL;
    if (!KNI_IsNullHandle(key_h)) {
        jsize len = KNI_GetStringLength(key_h);
        char key[128] = {0};
        jsize copylen = (len < 127) ? len : 127;
        jchar ubuf[128];
        KNI_GetStringRegion(key_h, 0, copylen, ubuf);
        for (int i = 0; i < copylen; i++) key[i] = (char)(ubuf[i] & 0xFF);
        key[copylen] = '\0';

        val = gb300_lookup_system_property(key);
        xlog("[CONFIG] getProperty0('%s') -> '%s'\n", key, val ? val : "(null)");
    }
    if (val != NULL) {
        KNI_NewStringUTF(val, res_h);
    } else {
        KNI_ReleaseHandle(res_h);
    }
    KNI_EndHandlesAndReturnObject(res_h);
}

extern unsigned char* gb300_read_jar_entry(const char *jar_path, const char *entry_name, size_t *out_size);

KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_jarutil_JarReader_readJarEntry0) {
    KNI_StartHandles(3);
    KNI_DeclareHandle(jar_h);
    KNI_DeclareHandle(entry_h);
    KNI_DeclareHandle(res_h);

    KNI_GetParameterAsObject(1, jar_h);
    KNI_GetParameterAsObject(2, entry_h);

    char jar_path[512] = {0};
    char entry_name[256] = {0};

    if (!KNI_IsNullHandle(jar_h)) {
        jsize len = KNI_GetStringLength(jar_h);
        jsize copylen = (len < 511) ? len : 511;
        jchar ubuf[512];
        KNI_GetStringRegion(jar_h, 0, copylen, ubuf);
        for (int i = 0; i < copylen; i++) jar_path[i] = (char)(ubuf[i] & 0xFF);
        jar_path[copylen] = '\0';
    }

    if (!KNI_IsNullHandle(entry_h)) {
        jsize len = KNI_GetStringLength(entry_h);
        jsize copylen = (len < 255) ? len : 255;
        jchar ubuf[256];
        KNI_GetStringRegion(entry_h, 0, copylen, ubuf);
        for (int i = 0; i < copylen; i++) entry_name[i] = (char)(ubuf[i] & 0xFF);
        entry_name[copylen] = '\0';
    }

    if (jar_path[0] == '\0') {
        const char *def_jar = gb300_fs_get_jar();
        if (def_jar) {
            strncpy(jar_path, def_jar, sizeof(jar_path) - 1);
        }
    }

    size_t out_sz = 0;
    unsigned char *data = gb300_read_jar_entry(jar_path, entry_name, &out_sz);
    xlog("[JarReader] readJarEntry0: jar='%s', entry='%s' -> %zu bytes\n", jar_path, entry_name, out_sz);
    if (data && out_sz > 0) {
        SNI_NewArray(SNI_BYTE_ARRAY, out_sz, res_h);
        if (!KNI_IsNullHandle(res_h)) {
            KNI_SetRawArrayRegion(res_h, 0, out_sz, (jbyte*)data);
        }
        free(data);
    }

    KNI_EndHandlesAndReturnObject(res_h);
}

/* Native method String[] load() of com.sun.midp.midletsuite.SuiteProperties */
KNIEXPORT KNI_RETURNTYPE_OBJECT
KNIDECL(com_sun_midp_midletsuite_SuiteProperties_load) {
    KNI_StartHandles(2);
    KNI_DeclareHandle(properties);
    KNI_DeclareHandle(tempStr);

    const char *jar_path = gb300_fs_get_jar();
    size_t sz = 0;
    unsigned char *buf = jar_path ? gb300_read_jar_entry(jar_path, "META-INF/MANIFEST.MF", &sz) : NULL;

    if (!buf || sz == 0) {
        if (buf) free(buf);
        xlog("[SuiteProperties] load: no manifest found, returning empty array\n");
        SNI_NewArray(SNI_STRING_ARRAY, 0, properties);
    } else {
        typedef struct {
            char key[128];
            char val[512];
        } PropPair;

        PropPair *pairs = (PropPair*)calloc(256, sizeof(PropPair));
        int count = 0;

        if (pairs) {
            const char *p = (const char*)buf;
            const char *end = p + sz;

            while (p < end && count < 256) {
                /* Skip empty lines */
                while (p < end && (*p == '\r' || *p == '\n')) p++;
                if (p >= end) break;

                /* Check continuation line (starts with space or tab) */
                if ((*p == ' ' || *p == '\t') && count > 0) {
                    p++;
                    const char *val_start = p;
                    while (p < end && *p != '\r' && *p != '\n') p++;
                    size_t vlen = p - val_start;
                    size_t cur_len = strlen(pairs[count - 1].val);
                    if (cur_len + vlen < sizeof(pairs[count - 1].val) - 1) {
                        memcpy(pairs[count - 1].val + cur_len, val_start, vlen);
                        pairs[count - 1].val[cur_len + vlen] = '\0';
                    }
                    continue;
                }

                /* Regular line: Name: Value */
                const char *colon = p;
                while (colon < end && *colon != ':' && *colon != '\r' && *colon != '\n') colon++;
                if (colon >= end || *colon != ':') {
                    /* Skip non-header line */
                    while (p < end && *p != '\r' && *p != '\n') p++;
                    continue;
                }

                size_t klen = colon - p;
                if (klen >= sizeof(pairs[count].key)) klen = sizeof(pairs[count].key) - 1;
                memcpy(pairs[count].key, p, klen);
                pairs[count].key[klen] = '\0';

                /* Skip colon and leading spaces */
                colon++;
                while (colon < end && (*colon == ' ' || *colon == '\t')) colon++;

                const char *val_start = colon;
                while (colon < end && *colon != '\r' && *colon != '\n') colon++;
                size_t vlen = colon - val_start;
                if (vlen >= sizeof(pairs[count].val)) vlen = sizeof(pairs[count].val) - 1;
                memcpy(pairs[count].val, val_start, vlen);
                pairs[count].val[vlen] = '\0';

                count++;
                p = colon;
            }

            free(buf);

            xlog("[SuiteProperties] load: parsed %d manifest properties from '%s'\n", count, jar_path);
            SNI_NewArray(SNI_STRING_ARRAY, count * 2, properties);
            if (!KNI_IsNullHandle(properties)) {
                for (int i = 0; i < count; i++) {
                    KNI_NewStringUTF(pairs[i].key, tempStr);
                    KNI_SetObjectArrayElement(properties, (jint)(i * 2), tempStr);
                    KNI_NewStringUTF(pairs[i].val, tempStr);
                    KNI_SetObjectArrayElement(properties, (jint)(i * 2 + 1), tempStr);
                }
            }
            free(pairs);
        } else {
            free(buf);
            SNI_NewArray(SNI_STRING_ARRAY, 0, properties);
        }
    }

    KNI_EndHandlesAndReturnObject(properties);
}

#ifdef __cplusplus
}
#endif
