#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "psp_compat.h"

void gb300_video_flush(const uint16_t *src, int src_w, int src_h, int src_pitch);
const char* gb300_fs_get_jar(void);

/* Properties / Config stubs */
int javacall_initialize_configurations(void) {
    xlog("[GB300-STUB] javacall_initialize_configurations()\n");
    return 0;
}
int javacall_finalize_configurations(void) {
    xlog("[GB300-STUB] javacall_finalize_configurations()\n");
    return 0;
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
    xlog("[GB300-STUB] midpFixMidpHome returning '/ROMS/J2ME'\n");
    return "/ROMS/J2ME";
}
void InitializeEvents(void) {}
void FinalizeEvents(void) {}
void midp_resetEvents(void) {}
void midpMIDletProxyListReset(void) {}
void midp_check_events(void *blocked_threads, int blocked_threads_count, long long timeout) {}
const char* midp_suite_get_suite_storage(int id) { (void)id; return "/ROMS/J2ME"; }
const char* midp_suite_get_class_path(int id) { (void)id; return NULL; }
char storageGetPathSeparator(void) { return '/'; }
void* pcsl_mem_malloc_impl0(size_t sz) { return malloc(sz); }
void pcsl_mem_free_chunk(void* p) { if (p) free(p); }
void pcsl_mem_free_impl0(void* p) { if (p) free(p); }

static int dummy_str = 1;
int pcsl_string_from_chars(const char* chars, void** str) {
    (void)chars;
    if (str) *str = &dummy_str;
    return 0;
}
int midp_get_suite_ids(void **ids, int *num) {
    if (ids) *ids = NULL;
    if (num) *num = 0;
    return 0;
}
void midp_free_suite_ids(void *ids, int num) { (void)ids; (void)num; }
int find_midlet_class(int id, int num, void** res) {
    (void)id; (void)num;
    if (res) *res = &dummy_str;
    return 1;
}

void midp_suite_storage_cleanup(void) {}
void sr_finalizeSystem(void) {}
void finalizeConfig(void) {}
void storageFinalize(void) {}
void sr_initSystem(void) {}
int storageInitialize(void) { return 0; }
int midp_suite_storage_init(void) { return 0; }
int initializeConfig(void) { return 0; }
const char* getInternalProperty(const char* k) { (void)k; return NULL; }
void storageSetTotalSpace(int sz) { (void)sz; }

void lcdlf_ui_init(void) {}
void lcdlf_ui_finalize(void) {}
int lcdlf_reverse_orientation(void) { return 0; }
int pushopen(void) { return 0; }
int pushclose(void) { return 0; }
int pushcheckinLeftOvers(void) { return 0; }
int pushcheckinall(void) { return 0; }
void sr_repairSystem(void) {}
void midpUnregisterAmsIsolateId(int id) { (void)id; }
const char* getSystemProperty(const char* key) { (void)key; return NULL; }

extern int JVM_Start(char* classPath, char* mainClass, int argc, char** argv);
int midpRunVm(char* classPath, char* mainClass, int argc, char** argv) {
    xlog("[PSPKVM-GB300] midpRunVm starting class '%s' with cp '%s'\n", mainClass ? mainClass : "NULL", classPath ? classPath : "NULL");
    return JVM_Start(classPath, mainClass, argc, argv);
}

#ifdef __cplusplus
extern "C" {
#endif
const void* jvm_natives_table[1] = { 0 };
void setInternalProperty(const char* k, const char* v) { (void)k; (void)v; }
void setSystemProperty(const char* k, const char* v) { (void)k; (void)v; }
#include <malloc.h>
void* pcsl_mem_allocate_chunk(unsigned int initial_size, unsigned int max_size, unsigned int alignment) {
    if (alignment == 0) alignment = 4;
    return memalign(alignment, max_size);
}
void* pcsl_mem_adjust_chunk(void* p, unsigned int sz) { return p; }
void setup_stack_asm(void) {}
#ifdef __cplusplus
}
#endif
