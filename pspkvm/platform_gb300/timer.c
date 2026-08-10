#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "psp_compat.h"

uint64_t gb300_timer_get_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000LL + tv.tv_usec;
}

uint32_t gb300_timer_get_ms(void) {
    return (uint32_t)(gb300_timer_get_us() / 1000LL);
}

void gb300_timer_delay_ms(uint32_t ms) {
    uint64_t start = gb300_timer_get_us();
    uint64_t target = start + (uint64_t)ms * 1000LL;
    while (gb300_timer_get_us() < target) {
        /* Spin / idle yield */
    }
}
