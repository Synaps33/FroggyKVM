#ifndef HACKER_LOADER_H
#define HACKER_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

void gb300_hacker_init(const char *rom_path);
void gb300_hacker_set_main_class(const char *main_class);
void gb300_hacker_log(const char *tag, const char *msg, int pct);
void gb300_hacker_log_class(const char *class_name);
void gb300_hacker_stop(void);
void gb300_hacker_exit(int exit_code);

#ifdef __cplusplus
}
#endif

#endif /* HACKER_LOADER_H */
