#ifndef PSP_COMPAT_H
#define PSP_COMPAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PSP Basic Types */
typedef int SceUID;
typedef unsigned int SceSize;
typedef unsigned int SceMode;
typedef unsigned int SceUInt;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef struct SceKernelSysClock {
    u32 low;
    u32 high;
} SceKernelSysClock;

typedef struct ScePspDateTime {
    u16 year;
    u16 month;
    u16 day;
    u16 hour;
    u16 minute;
    u16 second;
    u32 microsecond;
} ScePspDateTime;

typedef struct SceCtrlData {
    u32 TimeStamp;
    u32 Buttons;
    u8  Lx;
    u8  Ly;
    u8  Rsrv[6];
} SceCtrlData;

/* PSP Buttons constants */
#define PSP_CTRL_SELECT    0x000001
#define PSP_CTRL_START     0x000008
#define PSP_CTRL_UP        0x000010
#define PSP_CTRL_RIGHT     0x000020
#define PSP_CTRL_DOWN      0x000040
#define PSP_CTRL_LEFT      0x000080
#define PSP_CTRL_LTRIGGER  0x000100
#define PSP_CTRL_RTRIGGER  0x000200
#define PSP_CTRL_TRIANGLE  0x001000
#define PSP_CTRL_CIRCLE    0x002000
#define PSP_CTRL_CROSS     0x004000
#define PSP_CTRL_SQUARE    0x008000

/* Stubs for PSP Kernel / System calls */
static inline int sceKernelCreateCallback(const char *name, void *func, void *arg) { (void)name; (void)func; (void)arg; return 1; }
static inline int sceKernelRegisterExitCallback(int cbid) { (void)cbid; return 0; }
static inline int sceKernelCreateThread(const char *name, void *func, int initPriority, int stackSize, unsigned int attr, void *option) {
    (void)name; (void)func; (void)initPriority; (void)stackSize; (void)attr; (void)option; return 1;
}
static inline int sceKernelStartThread(int thid, SceSize arglen, void *argp) { (void)thid; (void)arglen; (void)argp; return 0; }
static inline int sceKernelWaitThreadEnd(int thid, void *timeout) { (void)thid; (void)timeout; return 0; }
static inline int sceKernelExitDeleteThread(int status) { (void)status; return 0; }
static inline int sceKernelDelayThread(unsigned int uSecs) { (void)uSecs; return 0; }
static inline int sceKernelSetAlarm(unsigned int uSecs, void *handler, void *common) { (void)uSecs; (void)handler; (void)common; return 1; }
static inline int sceKernelCancelAlarm(int alarmid) { (void)alarmid; return 0; }
static inline uint64_t sceKernelGetSystemTimeWide(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000LL + tv.tv_usec;
}
static inline uint32_t sceKernelGetSystemTimeLow(void) {
    return (uint32_t)sceKernelGetSystemTimeWide();
}
static inline uint32_t sceKernelTotalFreeMemSize(void) { return 12 * 1024 * 1024; }
static inline uint32_t sceKernelMaxFreeMemSize(void) { return 12 * 1024 * 1024; }
static inline void sceKernelDcacheWritebackAll(void) {}

/* Power Stubs */
static inline int scePowerRegisterCallback(int slot, int cbid) { (void)slot; (void)cbid; return 0; }

/* Display / GU Stubs */
static inline void sceDisplayWaitVblankStart(void) {}
static inline void sceGuInit(void) {}
static inline void sceGuTerm(void) {}
static inline void sceGuStart(int type, void *list) { (void)type; (void)list; }
static inline void sceGuFinish(void) {}
static inline void sceGuSync(int mode, int what) { (void)mode; (void)what; }
static inline void sceGuSwapBuffers(void) {}

/* Net / Utility Stubs */
static inline int sceUtilityLoadNetModule(int module) { (void)module; return 0; }
static inline int sceUtilityLoadModule(int module) { (void)module; return 0; }

/* These are implemented in logging.c, not stubs */
void javacall_printf(const char *fmt, ...);
void javacall_print(const char *str);
static inline void pspDebugScreenPrintf(const char *fmt, ...) { (void)fmt; }
static inline void pspkvm_screen_log(const char *str) { (void)str; }

#define PSP_MAX_MBX_QUEUES 16
#define PSP_MBX_QUEUE_SIZE 32

typedef struct {
    void *msgs[PSP_MBX_QUEUE_SIZE];
    int head;
    int tail;
    int count;
} psp_mbx_queue_t;

static psp_mbx_queue_t g_psp_mbx_queues[PSP_MAX_MBX_QUEUES];

static inline int sceKernelCreateMbx(const char *name, int attr, void *opt) {
    (void)name; (void)attr; (void)opt;
    static int next_id = 1;
    if (next_id >= PSP_MAX_MBX_QUEUES) next_id = 1;
    int id = next_id++;
    g_psp_mbx_queues[id].head = 0;
    g_psp_mbx_queues[id].tail = 0;
    g_psp_mbx_queues[id].count = 0;
    return id;
}

static inline int sceKernelSendMbx(int mbx, void *msg) {
    if (mbx > 0 && mbx < PSP_MAX_MBX_QUEUES) {
        psp_mbx_queue_t *q = &g_psp_mbx_queues[mbx];
        if (q->count < PSP_MBX_QUEUE_SIZE) {
            q->msgs[q->tail] = msg;
            q->tail = (q->tail + 1) % PSP_MBX_QUEUE_SIZE;
            q->count++;
            return 0;
        }
    }
    return -1;
}

static inline int sceKernelReceiveMbxCB(int mbx, void *msg_ptr, void *timeout) {
    (void)timeout;
    if (mbx > 0 && mbx < PSP_MAX_MBX_QUEUES && msg_ptr) {
        psp_mbx_queue_t *q = &g_psp_mbx_queues[mbx];
        if (q->count > 0) {
            *(void**)msg_ptr = q->msgs[q->head];
            q->head = (q->head + 1) % PSP_MBX_QUEUE_SIZE;
            q->count--;
            return 0;
        }
    }
    if (msg_ptr) *(void**)msg_ptr = NULL;
    return -1;
}

static inline int sceKernelPollMbx(int mbx, void *msg_ptr) {
    return sceKernelReceiveMbxCB(mbx, msg_ptr, NULL);
}

static inline int sceKernelDeleteMbx(int mbx) {
    if (mbx > 0 && mbx < PSP_MAX_MBX_QUEUES) {
        g_psp_mbx_queues[mbx].count = 0;
        g_psp_mbx_queues[mbx].head = 0;
        g_psp_mbx_queues[mbx].tail = 0;
    }
    return 0;
}

static inline int sceKernelWaitSemaCB(int sema, int val, void *timeout) { (void)sema; (void)val; (void)timeout; return 0; }
static inline int sceKernelSignalSema(int sema, int val) { (void)sema; (void)val; return 0; }
static inline int sceKernelCreateSema(const char *name, int attr, int init, int max, void *opt) { (void)name; (void)attr; (void)init; (void)max; (void)opt; return 1; }
static inline int sceKernelDeleteSema(int sema) { (void)sema; return 0; }

#define PSP_CTRL_MODE_ANALOG 1
#define PSP_THREAD_ATTR_USER 0x80000000

static inline int sceCtrlSetSamplingCycle(int cycle) { (void)cycle; return 0; }
static inline int sceCtrlSetSamplingMode(int mode) { (void)mode; return 0; }
static inline int sceCtrlReadBufferPositive(SceCtrlData *pad, int count) { (void)pad; (void)count; return 0; }

static inline int sceKernelWaitThreadEndCB(int thid, void *timeout) { (void)thid; (void)timeout; return 0; }
static inline int sceRtcGetCurrentTick(uint64_t *tick) { if (tick) *tick = sceKernelGetSystemTimeWide(); return 0; }
static inline uint32_t sceRtcGetTickResolution(void) { return 1000000; }

void xlog(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* PSP_COMPAT_H */
