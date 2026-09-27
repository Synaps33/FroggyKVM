#ifndef _EVENTQUEUE_COMPAT_H_
#define _EVENTQUEUE_COMPAT_H_

#ifndef MAX_EVENTS
#define MAX_EVENTS 20
#endif

struct Java_com_sun_midp_events_EventQueue {
    int nativeEventQueueHandle;
};

#ifdef __cplusplus
extern "C" {
#endif
void pss(void);
int javacall_initialize_configurations(void);
void javacall_finalize_configurations(void);
int javacall_total_heap_size(void);
void javacall_keymap_init(void);
void javanotify_network_connect(void);
void javanotify_shutdown_current(void);
void javanotify_switch_to_ams(void);
#ifdef __cplusplus
}
#endif

#endif /* _EVENTQUEUE_COMPAT_H_ */
