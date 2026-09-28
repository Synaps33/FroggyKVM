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

/* ===================== WAV (PCM) playback =====================
 * The JSR-135 DirectPlayer hands the audio data of every player to the
 * native side via nBuffering(). We accumulate it, parse RIFF/WAVE on
 * end-of-stream and decode to a fixed 22050 Hz stereo stream, which is then
 * mixed into the core's audio output (gb300_audio_read).
 */
#define WAV_MAX_PLAYERS 32
#define WAV_PCM_RATE 22050
#define WAV_MAX_RAW_PER_PLAYER (4 * 1024 * 1024)
#define WAV_MAX_RAW_TOTAL (6 * 1024 * 1024)
#define WAV_MAX_PCM_TOTAL (8 * 1024 * 1024)

typedef struct {
    int used;
    char mime[32];
    unsigned char *raw;      /* accumulated source data */
    int rawLen;
    int rawTruncated;
    int16_t *pcm;            /* decoded: WAV_PCM_RATE Hz, stereo interleaved */
    int pcmFrames;
    int pos;
    int playing;
    int paused;
    int volume;              /* 0..100 */
    int muted;
    int durationMs;
} wav_player_t;

static wav_player_t g_wav[WAV_MAX_PLAYERS];
static int g_wav_raw_total = 0;
static int g_wav_pcm_total = 0;

static wav_player_t *wav_get(int handle) {
    if (handle <= 0 || handle > WAV_MAX_PLAYERS) return NULL;
    return &g_wav[handle - 1];
}

static void wav_unload(wav_player_t *p) {
    if (p->raw) { g_wav_raw_total -= p->rawLen; free(p->raw); p->raw = NULL; }
    if (p->pcm) { g_wav_pcm_total -= p->pcmFrames * 4; free(p->pcm); p->pcm = NULL; }
    p->rawLen = 0;
    p->rawTruncated = 0;
    p->pcmFrames = 0;
    p->pos = 0;
    p->playing = 0;
    p->paused = 0;
    p->durationMs = 0;
}

static void wav_read_src(const unsigned char *d, int channels, int bits,
                         int frame, int *l, int *r) {
    if (bits == 8) {
        const unsigned char *s = d + frame * channels;
        int lv = ((int)s[0] - 128) << 8;
        *l = lv;
        *r = (channels >= 2) ? (((int)s[1] - 128) << 8) : lv;
    } else {
        const unsigned char *s = d + frame * channels * 2;
        int lv = (int)(int16_t)(s[0] | (s[1] << 8));
        *l = lv;
        *r = (channels >= 2) ? (int)(int16_t)(s[2] | (s[3] << 8)) : lv;
    }
}

/* IMA ADPCM (WAVE format 0x11) - used by many J2ME games */
static const int ima_index_table[8] = { -1, -1, -1, -1, 2, 4, 6, 8 };
static const int ima_step_table[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41,
    45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190,
    209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724,
    796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272,
    2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132,
    7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500,
    20350, 22385, 24623, 27086, 29794, 32767
};

static int wav_decode_adpcm(wav_player_t *p, unsigned char *data, int dataLen,
                            int channels, int rate, int blockAlign, int samplesPerBlock) {
    int blocks, totalFrames, i, b, ch;
    int16_t *src;
    int budget, outFrames;

    if (blockAlign <= 4 * channels) return 0;
    if (samplesPerBlock <= 0 || samplesPerBlock > 4096) {
        samplesPerBlock = ((blockAlign - 4 * channels) * 2 / channels) + 1;
    }
    blocks = dataLen / blockAlign;
    if (blocks <= 0 || samplesPerBlock <= 0) return 0;

    totalFrames = blocks * samplesPerBlock;
    src = (int16_t *)malloc((size_t)totalFrames * channels * sizeof(int16_t));
    if (!src) return 0;

    for (b = 0; b < blocks; b++) {
        unsigned char *blk = data + b * blockAlign;
        int pred[2] = { 0, 0 };
        int stepi[2] = { 0, 0 };
        int sp = 4 * channels;

        for (ch = 0; ch < channels; ch++) {
            pred[ch] = (int16_t)(blk[ch * 4] | (blk[ch * 4 + 1] << 8));
            stepi[ch] = blk[ch * 4 + 2];
            if (stepi[ch] > 88) stepi[ch] = 88;
        }

        for (i = 0; i < samplesPerBlock; i++) {
            for (ch = 0; ch < channels; ch++) {
                int ni = i * channels + ch;
                int byteIdx = sp + ni / 2;
                int nib, step, diff;

                if (byteIdx >= blockAlign) {
                    src[(b * samplesPerBlock + i) * channels + ch] = (int16_t)pred[ch];
                    continue;
                }
                nib = (ni & 1) ? (blk[byteIdx] >> 4) : (blk[byteIdx] & 0x0F);
                step = ima_step_table[stepi[ch]];
                diff = step >> 3;
                if (nib & 4) diff += step;
                if (nib & 2) diff += step >> 1;
                if (nib & 1) diff += step >> 2;
                if (nib & 8) pred[ch] -= diff; else pred[ch] += diff;
                if (pred[ch] > 32767) pred[ch] = 32767;
                else if (pred[ch] < -32768) pred[ch] = -32768;
                stepi[ch] += ima_index_table[nib & 7];
                if (stepi[ch] < 0) stepi[ch] = 0;
                else if (stepi[ch] > 88) stepi[ch] = 88;
                src[(b * samplesPerBlock + i) * channels + ch] = (int16_t)pred[ch];
            }
        }
    }

    budget = WAV_MAX_PCM_TOTAL - g_wav_pcm_total;
    outFrames = (int)(((int64_t)totalFrames * WAV_PCM_RATE) / rate);
    if (outFrames <= 0) { free(src); return 0; }
    if (outFrames * 4 > budget) outFrames = budget / 4;
    if (outFrames <= 0) { free(src); return 0; }

    p->pcm = (int16_t *)malloc((size_t)outFrames * 4);
    if (!p->pcm) { free(src); return 0; }

    for (i = 0; i < outFrames; i++) {
        int64_t fixed = ((int64_t)i * rate * 256) / WAV_PCM_RATE;
        int idx = (int)(fixed >> 8);
        int frac = (int)(fixed & 0xFF);
        int l0, r0, l1, r1;

        if (idx >= totalFrames - 1) { idx = totalFrames - 1; frac = 0; }
        l0 = src[idx * channels];
        r0 = (channels > 1) ? src[idx * channels + 1] : l0;
        if (frac) {
            l1 = src[(idx + 1) * channels];
            r1 = (channels > 1) ? src[(idx + 1) * channels + 1] : l1;
        } else {
            l1 = l0; r1 = r0;
        }
        p->pcm[i * 2]     = (int16_t)((l0 * (256 - frac) + l1 * frac) >> 8);
        p->pcm[i * 2 + 1] = (int16_t)((r0 * (256 - frac) + r1 * frac) >> 8);
    }

    free(src);
    p->pcmFrames = outFrames;
    g_wav_pcm_total += outFrames * 4;
    p->durationMs = (int)(((int64_t)totalFrames * 1000) / rate);
    return 1;
}

/* Parse RIFF/WAVE (PCM) and decode to 22050 Hz stereo s16. Returns 1 on success. */
static int wav_decode(wav_player_t *p) {
    unsigned char *d = p->raw;
    int n = p->rawLen;
    int pos, format = 0, channels = 0, rate = 0, bits = 0;
    int blockAlign = 0, samplesPerBlock = 0;
    unsigned char *data = NULL;
    int dataLen = 0;

    if (!d || n < 44) {
        xlog("[AUDIO] wav: too small (%d bytes)\n", n);
        return 0;
    }
    if (memcmp(d, "RIFF", 4) != 0 || memcmp(d + 8, "WAVE", 4) != 0) {
        xlog("[AUDIO] wav: bad magic %c%c%c%c / %c%c%c%c\n",
             d[0], d[1], d[2], d[3], d[8], d[9], d[10], d[11]);
        return 0;
    }

    for (pos = 12; pos + 8 <= n; ) {
        unsigned char *id = d + pos;
        unsigned int sz = (unsigned int)d[pos + 4] | ((unsigned int)d[pos + 5] << 8) |
                          ((unsigned int)d[pos + 6] << 16) | ((unsigned int)d[pos + 7] << 24);
        pos += 8;
        if ((int)sz > n - pos) sz = (unsigned int)(n - pos);
        if (memcmp(id, "fmt ", 4) == 0 && sz >= 16) {
            format = d[pos] | (d[pos + 1] << 8);
            channels = d[pos + 2] | (d[pos + 3] << 8);
            rate = (int)((unsigned int)d[pos + 4] | ((unsigned int)d[pos + 5] << 8) |
                         ((unsigned int)d[pos + 6] << 16) | ((unsigned int)d[pos + 7] << 24));
            bits = d[pos + 14] | (d[pos + 15] << 8);
            blockAlign = d[pos + 12] | (d[pos + 13] << 8);
            if (sz >= 20) samplesPerBlock = d[pos + 18] | (d[pos + 19] << 8);
            if (format == 0xFFFE && sz >= 40) {
                format = d[pos + 24] | (d[pos + 25] << 8); /* SubFormat GUID, first 2 bytes */
            }
        } else if (memcmp(id, "data", 4) == 0) {
            data = d + pos;
            dataLen = (int)sz;
        }
        pos += (int)((sz + 1) & ~1u);
    }

    if (!data || dataLen <= 0) {
        xlog("[AUDIO] wav: no data chunk (data=%p len=%d)\n", (void*)data, dataLen);
        return 0;
    }
    if (format == 17 && bits == 4 && (channels == 1 || channels == 2) &&
        rate >= 4000 && rate <= 48000 && blockAlign > 0) {
        return wav_decode_adpcm(p, data, dataLen, channels, rate, blockAlign, samplesPerBlock);
    }
    if (format != 1 || (channels != 1 && channels != 2) ||
        (bits != 8 && bits != 16) || rate < 4000 || rate > 48000) {
        xlog("[AUDIO] wav: unsupported fmt=%d ch=%d rate=%d bits=%d dataLen=%d\n",
             format, channels, rate, bits, dataLen);
        return 0;
    }

    {
        int bytesPerFrame = channels * (bits / 8);
        int srcFrames = dataLen / bytesPerFrame;
        int outFrames, i;
        int budget = WAV_MAX_PCM_TOTAL - g_wav_pcm_total;

        if (srcFrames <= 0) return 0;

        outFrames = (int)(((int64_t)srcFrames * WAV_PCM_RATE) / rate);
        if (outFrames <= 0) return 0;
        if (outFrames * 4 > budget) outFrames = budget / 4;
        if (outFrames <= 0) return 0;

        p->pcm = (int16_t *)malloc((size_t)outFrames * 4);
        if (!p->pcm) return 0;

        for (i = 0; i < outFrames; i++) {
            int64_t fixed = ((int64_t)i * rate * 256) / WAV_PCM_RATE;
            int idx = (int)(fixed >> 8);
            int frac = (int)(fixed & 0xFF);
            int l0, r0, l1, r1;

            if (idx >= srcFrames - 1) { idx = srcFrames - 1; frac = 0; }
            wav_read_src(data, channels, bits, idx, &l0, &r0);
            if (frac) {
                wav_read_src(data, channels, bits, idx + 1, &l1, &r1);
            } else {
                l1 = l0; r1 = r0;
            }
            p->pcm[i * 2]     = (int16_t)((l0 * (256 - frac) + l1 * frac) >> 8);
            p->pcm[i * 2 + 1] = (int16_t)((r0 * (256 - frac) + r1 * frac) >> 8);
        }

        p->pcmFrames = outFrames;
        g_wav_pcm_total += outFrames * 4;
        p->durationMs = (int)(((int64_t)srcFrames * 1000) / rate);
        return 1;
    }
}

/* Mix one output frame (22050 Hz stereo) of all active WAV players */
static void wav_mix_frame(int32_t *l, int32_t *r) {
    int i;
    for (i = 0; i < WAV_MAX_PLAYERS; i++) {
        wav_player_t *p = &g_wav[i];
        if (p->used && p->playing && !p->paused && p->pcm && p->pos < p->pcmFrames) {
            int vol = p->muted ? 0 : p->volume;
            *l += ((int)p->pcm[p->pos * 2]     * vol) / 100;
            *r += ((int)p->pcm[p->pos * 2 + 1] * vol) / 100;
            if (++p->pos >= p->pcmFrames) {
                p->playing = 0;
                p->pos = 0;
            }
        }
    }
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
        int32_t mix_l = 0;
        int32_t mix_r = 0;

        // Take from ring buffer if available
        if (audio_buffer_tail != audio_buffer_head) {
            mix_l += audio_ring_buffer[audio_buffer_tail];
            audio_buffer_tail = (audio_buffer_tail + 1) & AUDIO_RING_MASK;
            mix_r += audio_ring_buffer[audio_buffer_tail];
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
            mix_l += tone_sample;
            mix_r += tone_sample;

            g_tone.phase += g_tone.phase_step;
            g_tone.samples_remaining--;
            if (g_tone.samples_remaining <= 0) {
                g_tone.active = 0;
            }
        }

        // Mix WAV players (JSR-135 DirectPlayer)
        wav_mix_frame(&mix_l, &mix_r);

        if (mix_l > 32767) mix_l = 32767; else if (mix_l < -32768) mix_l = -32768;
        if (mix_r > 32767) mix_r = 32767; else if (mix_r < -32768) mix_r = -32768;
        dst[i * 2] = (int16_t)mix_l;
        dst[i * 2 + 1] = (int16_t)mix_r;
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
    jint isolateId = KNI_GetParameterAsInt(1);
    jint pID = KNI_GetParameterAsInt(2);
    int h, result = 0;
    char mimeCopy[32];

    (void)isolateId;
    (void)pID;

    KNI_StartHandles(2);
    KNI_DeclareHandle(hMime);
    KNI_DeclareHandle(hUri);
    KNI_GetParameterAsObject(3, hMime);
    KNI_GetParameterAsObject(4, hUri);

    for (h = 0; h < WAV_MAX_PLAYERS; h++) {
        if (!g_wav[h].used) break;
    }

    mimeCopy[0] = '\0';
    if (h < WAV_MAX_PLAYERS) {
        if (!KNI_IsNullHandle(hMime)) {
            jsize len = KNI_GetStringLength(hMime);
            jchar ubuf[32];
            jsize n = (len < 31) ? len : 31;
            KNI_GetStringRegion(hMime, 0, n, ubuf);
            for (jsize i = 0; i < n; i++) mimeCopy[i] = (char)(ubuf[i] & 0xFF);
            mimeCopy[n] = '\0';
        }

        wav_unload(&g_wav[h]);
        memset(&g_wav[h], 0, sizeof(wav_player_t));
        g_wav[h].used = 1;
        g_wav[h].volume = 100;
        strncpy(g_wav[h].mime, mimeCopy, sizeof(g_wav[h].mime) - 1);
        result = h + 1;
    }

    KNI_EndHandles();

    if (result) {
        xlog("[AUDIO] nInit: handle=%d mime='%s'\n", result, g_wav[result - 1].mime);
    } else {
        xlog("[AUDIO] nInit: no free player slot\n");
    }
    KNI_ReturnInt(result);
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
    /* The native side does not pull the stream itself - the Java layer must
     * push the data to us via nBuffering(). */
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nBuffering(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jlong bytes = KNI_GetParameterAsLong(3);
    wav_player_t *p = wav_get(handle);

    if (!p) {
        KNI_ReturnInt(-1);
    }

    if (bytes < 0) {
        /* end of stream: decode and drop the raw data */
        if (p->raw && !p->pcm) {
            if (!wav_decode(p)) {
                xlog("[AUDIO] handle %d: unsupported audio (%d bytes, truncated=%d, mime='%s')\n",
                     handle, p->rawLen, p->rawTruncated, p->mime);
            }
        }
        if (p->raw) {
            g_wav_raw_total -= p->rawLen;
            free(p->raw);
            p->raw = NULL;
            p->rawLen = 0;
        }
        xlog("[AUDIO] handle %d: %d frames (%d ms) ready, mime='%s'\n",
             handle, p->pcmFrames, p->durationMs, p->mime);
        KNI_ReturnInt(0);
    }

    if (bytes > 0 && !p->pcm) {
        KNI_StartHandles(1);
        KNI_DeclareHandle(hBuf);
        KNI_GetParameterAsObject(2, hBuf);
        if (!KNI_IsNullHandle(hBuf)) {
            jsize len = KNI_GetArrayLength(hBuf);
            if (len > (jsize)bytes) len = (jsize)bytes;
            if (p->rawLen + (int)len <= WAV_MAX_RAW_PER_PLAYER &&
                g_wav_raw_total + (int)len <= WAV_MAX_RAW_TOTAL) {
                unsigned char *nb = (unsigned char *)realloc(p->raw, (size_t)(p->rawLen + len));
                if (nb) {
                    p->raw = nb;
                    KNI_GetRawArrayRegion(hBuf, 0, len, (jbyte *)(p->raw + p->rawLen));
                    p->rawLen += (int)len;
                    g_wav_raw_total += (int)len;
                }
            } else if (!p->rawTruncated) {
                p->rawTruncated = 1;
                xlog("[AUDIO] handle %d: audio data truncated (limit reached)\n", handle);
            }
        }
        KNI_EndHandles();
    }
    KNI_ReturnInt(0);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nFlushBuffer(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    if (p && p->raw) {
        g_wav_raw_total -= p->rawLen;
        free(p->raw);
        p->raw = NULL;
        p->rawLen = 0;
    }
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nStart(void) {
    jint handle = KNI_GetParameterAsInt(1);
    wav_player_t *p = wav_get(handle);
    if (p) {
        if (p->paused) {
            p->paused = 0;
        } else {
            p->pos = 0;
        }
        p->playing = 1;
    }
    xlog("[AUDIO] nStart: handle=%d frames=%d playing=%d\n", handle, p ? p->pcmFrames : -1, p ? p->playing : -1);
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nStop(void) {
    jint handle = KNI_GetParameterAsInt(1);
    wav_player_t *p = wav_get(handle);
    if (p) {
        p->playing = 0;
        p->paused = 0;
        p->pos = 0;
    }
    xlog("[AUDIO] nStop: handle=%d pos=%d\n", handle, p ? p->pos : -1);
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nPause(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    if (p) {
        p->paused = 1;
    }
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectPlayer_nResume(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    if (p) {
        p->paused = 0;
        p->playing = 1;
    }
    KNI_ReturnBoolean(KNI_TRUE);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nGetDuration(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    KNI_ReturnInt(p ? p->durationMs : 0);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nGetMediaTime(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    if (!p) KNI_ReturnInt(0);
    KNI_ReturnInt((jint)(((int64_t)p->pos * 1000) / WAV_PCM_RATE));
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectPlayer_nSetMediaTime(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jlong ms = KNI_GetParameterAsLong(2);
    wav_player_t *p = wav_get(handle);
    if (p) {
        int64_t pos = (ms * WAV_PCM_RATE) / 1000;
        if (pos < 0) pos = 0;
        if (pos > p->pcmFrames) pos = p->pcmFrames;
        p->pos = (int)pos;
    }
    KNI_ReturnInt((jint)ms);
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
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    if (p) {
        wav_unload(p);
        p->used = 0;
    }
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
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    KNI_ReturnInt(p ? p->volume : 100);
}

KNIEXPORT KNI_RETURNTYPE_INT
Java_com_sun_mmedia_DirectVolume_nSetVolume(void) {
    jint handle = KNI_GetParameterAsInt(1);
    jint vol = KNI_GetParameterAsInt(2);
    wav_player_t *p = wav_get(handle);
    if (vol < 0) vol = 0;
    if (vol > 100) vol = 100;
    if (p) p->volume = vol;
    xlog("[AUDIO] nSetVolume: handle=%d vol=%d\n", handle, vol);
    KNI_ReturnInt(vol);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectVolume_nIsMuted(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    KNI_ReturnBoolean((p && p->muted) ? KNI_TRUE : KNI_FALSE);
}

KNIEXPORT KNI_RETURNTYPE_BOOLEAN
Java_com_sun_mmedia_DirectVolume_nSetMute(void) {
    wav_player_t *p = wav_get(KNI_GetParameterAsInt(1));
    jboolean mute = KNI_GetParameterAsBoolean(2);
    if (p) p->muted = mute ? 1 : 0;
    xlog("[AUDIO] nSetMute: handle=%d mute=%d\n", KNI_GetParameterAsInt(1), mute ? 1 : 0);
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
