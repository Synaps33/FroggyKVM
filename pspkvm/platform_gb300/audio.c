#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "psp_compat.h"
#include <kni.h>

#define AUDIO_SAMPLE_RATE 22050
#define AUDIO_BUFFER_SIZE 4096
#define AUDIO_RING_MASK ((AUDIO_BUFFER_SIZE * 2) - 1)

/* Ring buffer for streaming PCM */
static int16_t audio_ring_buffer[AUDIO_BUFFER_SIZE * 2];
static int audio_buffer_head = 0;
static int audio_buffer_tail = 0;

/* Tone Synthesizer State */
typedef struct {
    int active;
    uint32_t phase;
    uint32_t phase_step;
    int samples_remaining;
    int16_t amplitude;
} ToneState;

static ToneState g_tone = {0};

/* 16.16 fixed-point phase increment per sample at 22050 Hz for MIDI notes 0..127 */
static const uint32_t midi_note_phase_step[128] = {
    24, 26, 27, 29, 31, 32, 34, 36,
    39, 41, 43, 46, 49, 51, 55, 58,
    61, 65, 69, 73, 77, 82, 87, 92,
    97, 103, 109, 116, 122, 130, 137, 146,
    154, 163, 173, 183, 194, 206, 218, 231,
    245, 259, 275, 291, 309, 327, 346, 367,
    389, 412, 436, 462, 490, 519, 550, 583,
    617, 654, 693, 734, 778, 824, 873, 925,
    980, 1038, 1100, 1165, 1234, 1308, 1386, 1468,
    1555, 1648, 1746, 1849, 1959, 2076, 2199, 2330,
    2469, 2615, 2771, 2936, 3110, 3295, 3491, 3699,
    3919, 4152, 4399, 4660, 4937, 5231, 5542, 5872,
    6221, 6591, 6983, 7398, 7838, 8304, 8797, 9321,
    9875, 10462, 11084, 11743, 12441, 13181, 13965, 14795,
    15675, 16607, 17595, 18641, 19750, 20924, 22168, 23486,
    24883, 26363, 27930, 29591, 31351, 33215, 35190, 37282,
};

void gb300_audio_init(void) {
    audio_buffer_head = 0;
    audio_buffer_tail = 0;
    memset(audio_ring_buffer, 0, sizeof(audio_ring_buffer));
    memset(&g_tone, 0, sizeof(g_tone));
}

void gb300_audio_deinit(void) {
    audio_buffer_head = 0;
    audio_buffer_tail = 0;
    memset(&g_tone, 0, sizeof(g_tone));
}

void gb300_audio_play_tone(int note, int duration_ms, int volume) {
    if (note < 0 || note > 127) return;
    if (duration_ms <= 0) duration_ms = 100;
    if (volume <= 0) {
        g_tone.active = 0;
        return;
    }
    if (volume > 100) volume = 100;

    g_tone.phase = 0;
    g_tone.phase_step = midi_note_phase_step[note];
    g_tone.samples_remaining = (duration_ms * AUDIO_SAMPLE_RATE) / 1000;
    // Scale amplitude: volume 100 -> ~16000 (safe headroom for mixing)
    g_tone.amplitude = (int16_t)((volume * 16000) / 100);
    g_tone.active = 1;
}

void gb300_audio_stop_tone(void) {
    g_tone.active = 0;
}

int gb300_audio_write(const int16_t *samples, int num_frames) {
    if (!samples || num_frames <= 0) return 0;
    for (int i = 0; i < num_frames * 2; i++) {
        audio_ring_buffer[audio_buffer_head] = samples[i];
        audio_buffer_head = (audio_buffer_head + 1) & AUDIO_RING_MASK;
    }
    return num_frames;
}

int gb300_audio_read(int16_t *dst, int num_frames) {
    if (!dst || num_frames <= 0) return 0;

    for (int i = 0; i < num_frames; i++) {
        int16_t sample_l = 0;
        int16_t sample_r = 0;

        // Take from ring buffer if available
        if (audio_buffer_tail != audio_buffer_head) {
            sample_l = audio_ring_buffer[audio_buffer_tail];
            audio_buffer_tail = (audio_buffer_tail + 1) & AUDIO_RING_MASK;
            sample_r = audio_ring_buffer[audio_buffer_tail];
            audio_buffer_tail = (audio_buffer_tail + 1) & AUDIO_RING_MASK;
        }

        // Mix synthesized tone (smooth triangle wave)
        if (g_tone.active && g_tone.samples_remaining > 0) {
            uint16_t frac = (uint16_t)(g_tone.phase & 0xFFFF);
            int16_t tone_sample;
            if (frac < 32768) {
                tone_sample = -g_tone.amplitude + (int16_t)(((int32_t)g_tone.amplitude * frac) >> 14);
            } else {
                tone_sample = g_tone.amplitude - (int16_t)(((int32_t)g_tone.amplitude * (frac - 32768)) >> 14);
            }

            int32_t mix_l = sample_l + tone_sample;
            int32_t mix_r = sample_r + tone_sample;
            if (mix_l > 32767) mix_l = 32767; else if (mix_l < -32768) mix_l = -32768;
            if (mix_r > 32767) mix_r = 32767; else if (mix_r < -32768) mix_r = -32768;
            sample_l = (int16_t)mix_l;
            sample_r = (int16_t)mix_r;

            g_tone.phase += g_tone.phase_step;
            g_tone.samples_remaining--;
            if (g_tone.samples_remaining <= 0) {
                g_tone.active = 0;
            }
        }

        dst[i * 2] = sample_l;
        dst[i * 2 + 1] = sample_r;
    }
    return num_frames;
}

/* Javacall C bindings */
int javacall_media_play_tone(long note, long duration, long volume) {
    if (note >= 0 && note <= 127) {
        gb300_audio_play_tone((int)note, (int)duration, (int)volume);
        return 0; // JAVACALL_OK
    }
    return -1; // JAVACALL_FAIL
}

int javacall_media_stop_tone(void) {
    gb300_audio_stop_tone();
    return 0; // JAVACALL_OK
}

/* KNI TonePlayer Natives */
KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_NativeTonePlayer_nPlayTone(void) {
    jint note = KNI_GetParameterAsInt(1);
    jint dur  = KNI_GetParameterAsInt(2);
    jint vol  = KNI_GetParameterAsInt(3);

    if (vol < 0) vol = 0;
    if (vol > 100) vol = 100;
    if (dur <= 0) dur = 100;

    if (note >= 0 && note <= 127) {
        gb300_audio_play_tone((int)note, (int)dur, (int)vol);
    }
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_NativeTonePlayer_nStopTone(void) {
    gb300_audio_stop_tone();
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_NativeTonePlayer_finalize(void) {
    gb300_audio_stop_tone();
    KNI_ReturnVoid();
}

/* KNI DirectPlayer Natives */
KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nInit(void) {
    KNI_ReturnInt(1);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nAcquireDevice(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_DirectPlayer_nReleaseDevice(void) {
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nIsNeedBuffering(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nBuffering(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nFlushBuffer(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nStart(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nStop(void) {
    gb300_audio_stop_tone();
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nPause(void) {
    gb300_audio_stop_tone();
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nResume(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nGetDuration(void) {
    KNI_ReturnInt(10000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nGetMediaTime(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nSetMediaTime(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nSwitchToBackground(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nSwitchToForeground(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nTerm(void) {
    gb300_audio_stop_tone();
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nIsSupportRecording(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_DirectPlayer_finalize(void) {
    KNI_ReturnVoid();
}

/* KNI DirectVolume Natives */
KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectVolume_nGetVolume(void) {
    KNI_ReturnInt(100);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectVolume_nSetVolume(void) {
    jint vol = KNI_GetParameterAsInt(2);
    KNI_ReturnInt(vol);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectVolume_nIsMuted(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectVolume_nSetMute(void) {
    KNI_ReturnBoolean(KNI_TRUE);
}

/* KNI DirectMIDIControl Natives */
KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_DirectMIDIControl_nShortMidiEvent(void) {
    jint status = KNI_GetParameterAsInt(2);
    jint data1  = KNI_GetParameterAsInt(3);
    jint data2  = KNI_GetParameterAsInt(4);

    int cmd = status & 0xF0;
    if (cmd == 0x90) { // Note On
        if (data2 > 0) {
            gb300_audio_play_tone((int)data1, 300, (int)((data2 * 100) / 127));
        } else {
            gb300_audio_stop_tone();
        }
    } else if (cmd == 0x80) { // Note Off
        gb300_audio_stop_tone();
    }
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nLongMidiEvent(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectMIDIControl_nIsBankQuerySupported(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetBankList(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetProgramList(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetProgram(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetProgramName(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_DirectMIDIControl_nSetProgram(void) {
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetChannelVolume(void) {
    KNI_ReturnInt(100);
}

KNIEXPORT KNI_RETURNTYPE_VOID
Java_com_sun_mmedia_DirectMIDIControl_nSetChannelVolume(void) {
    KNI_ReturnVoid();
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetPitch(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nSetPitch(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetMaxPitch(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetMinPitch(void) {
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetRate(void) {
    KNI_ReturnInt(100000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nSetRate(void) {
    KNI_ReturnInt(100000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetMaxRate(void) {
    KNI_ReturnInt(100000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetMinRate(void) {
    KNI_ReturnInt(100000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetTempo(void) {
    KNI_ReturnInt(120000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nSetTempo(void) {
    KNI_ReturnInt(120000);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectMIDIControl_nGetKeyName(void) {
    KNI_ReturnInt(0);
}

/* KNI DefaultConfiguration Natives */
KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DefaultConfiguration_nIsAmrSupported(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DefaultConfiguration_nIsJtsSupported(void) {
    KNI_ReturnBoolean(KNI_FALSE);
}
