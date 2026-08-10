#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "psp_compat.h"

#define AUDIO_SAMPLE_RATE 22050
#define AUDIO_BUFFER_SIZE 2048

static int16_t audio_ring_buffer[AUDIO_BUFFER_SIZE * 2];
static int audio_buffer_head = 0;
static int audio_buffer_tail = 0;

void gb300_audio_init(void) {
    audio_buffer_head = 0;
    audio_buffer_tail = 0;
    memset(audio_ring_buffer, 0, sizeof(audio_ring_buffer));
}

void gb300_audio_deinit(void) {
    audio_buffer_head = 0;
    audio_buffer_tail = 0;
}

int gb300_audio_write(const int16_t *samples, int num_frames) {
    if (!samples || num_frames <= 0) return 0;
    for (int i = 0; i < num_frames * 2; i++) {
        audio_ring_buffer[audio_buffer_head] = samples[i];
        audio_buffer_head = (audio_buffer_head + 1) % (AUDIO_BUFFER_SIZE * 2);
    }
    return num_frames;
}

int gb300_audio_read(int16_t *dst, int num_frames) {
    if (!dst || num_frames <= 0) return 0;
    int count = 0;
    for (int i = 0; i < num_frames * 2; i++) {
        if (audio_buffer_tail == audio_buffer_head) {
            dst[i] = 0;
        } else {
            dst[i] = audio_ring_buffer[audio_buffer_tail];
            audio_buffer_tail = (audio_buffer_tail + 1) % (AUDIO_BUFFER_SIZE * 2);
            count++;
        }
    }
    return count / 2;
}
