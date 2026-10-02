#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "psp_compat.h"
#include <kni.h>
#include "fluidlite.h"

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
#define WAV_MAX_PLAYERS 64
#define WAV_PCM_RATE 22050
#define WAV_MAX_RAW_PER_PLAYER (4 * 1024 * 1024)
#define WAV_MAX_RAW_TOTAL (8 * 1024 * 1024)
#define WAV_MAX_PCM_TOTAL (12 * 1024 * 1024)

/* ---- FluidLite soundfont synth -------------------------------------
 *
 * GB300: the built-in square-wave synth below sounds nothing like a real
 * instrument. FluidLite is FluidSynth's embeddable core, so we keep our own
 * SMF parser and mixer and only replace the oscillator bank: MIDI events go
 * straight to the soundfont, and fluid_synth_write_s16() fills the same
 * int16 buffer the rest of the mixer already expects.
 *
 * One synth is shared by every player - loading a 3 MB soundfont per track
 * would blow the memory budget. If the soundfont cannot be loaded we fall
 * back to the original synth so games still make noise.
 */
static fluid_settings_t *g_fl_settings = NULL;
static fluid_synth_t     *g_fl_synth = NULL;
static int                g_fl_probed = 0;

static const char *g_fl_soundfonts[] = {
    "/mnt/sda1/bios/Roland_SC-55.sf2",
    "/mnt/sda1/system/Roland_SC-55.sf2",
    NULL
};

static fluid_synth_t *midi_fluid_synth(void) {
    int i;
    if (g_fl_probed) return g_fl_synth;
    g_fl_probed = 1;

    g_fl_settings = new_fluid_settings();
    if (!g_fl_settings) return NULL;
    /* 22050 Hz, mono source rendered to our stereo frame buffer. */
    fluid_settings_setnum(g_fl_settings, "synth.sample-rate", (double)WAV_PCM_RATE);
    g_fl_synth = new_fluid_synth(g_fl_settings);
    if (!g_fl_synth) return NULL;

    for (i = 0; g_fl_soundfonts[i]; i++) {
        if (fluid_synth_sfload(g_fl_synth, g_fl_soundfonts[i], 1) >= 0) {
            xlog("[AUDIO] FluidLite: soundfont %s wczytany\n", g_fl_soundfonts[i]);
            return g_fl_synth;
        }
    }
    xlog("[AUDIO] FluidLite: brak soundfontu, uzywam wlasnego syntezatora\n");
    delete_fluid_synth(g_fl_synth);  g_fl_synth = NULL;
    delete_fluid_settings(g_fl_settings); g_fl_settings = NULL;
    return NULL;
}

/* Max frames synthesised per mixer call, so no single call stalls
 * the emulator for long. 2048 frames is ~93 ms of audio. */
#define MIDI_TOPUP_MAX 4096

/* Synthesis budget shared by every player for one gb300_audio_read() call. */
#define MIDI_BUDGET_PER_READ 8192
static int g_midi_budget = 0;

/* All players share a single FluidLite synth, and every midi_stream_render()
 * call advances that shared synth. With 22 tracks open (Doom RPG) the work was
 * therefore multiplied by the number of tracks and the CPU pegged at 100%,
 * freezing the device. Only one track may synthesise per gb300_audio_read();
 * the others keep playing whatever is already buffered. */
static int g_midi_synth_used = 0;

typedef struct midi_stream_s midi_stream_t;

typedef struct {
    int used;
    char mime[32];
    unsigned char *raw;      /* accumulated source data */
    int rawLen;
    int rawTruncated;
    int16_t *pcm;            /* decoded: WAV_PCM_RATE Hz, interleaved */
    int channels;            /* channels of pcm (1 or 2) */
    int pcmFrames;
    int pos;
    int playing;
    int paused;
    int volume;              /* 0..100 */
    int muted;
    int durationMs;
    void *midi;              /* midi_stream_t while streaming a SMF track */
    int loopMidi;            /* loop the rendered prefix instead of stopping */
} wav_player_t;

static void midi_stream_free(midi_stream_t *ms);

static wav_player_t g_wav[WAV_MAX_PLAYERS];
static int g_wav_raw_total = 0;
static int g_wav_pcm_total = 0;

static wav_player_t *wav_get(int handle) {
    if (handle <= 0 || handle > WAV_MAX_PLAYERS) return NULL;
    return &g_wav[handle - 1];
}

static void wav_unload(wav_player_t *p) {
    if (p->midi) { midi_stream_free((midi_stream_t *)p->midi); p->midi = NULL; }
    p->loopMidi = 0;
    if (p->raw) { g_wav_raw_total -= p->rawLen; free(p->raw); p->raw = NULL; }
    if (p->pcm) { g_wav_pcm_total -= p->pcmFrames * p->channels * 2; free(p->pcm); p->pcm = NULL; }
    p->channels = 2;
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
            src[(b * samplesPerBlock + 0) * channels + ch] = pred[ch];
        }

        for (i = 1; i < samplesPerBlock; i++) {
            for (ch = 0; ch < channels; ch++) {
                int nibIdx = i - 1;
                int byteIdx;
                if (channels == 1) {
                    byteIdx = sp + nibIdx / 2;
                } else {
                    byteIdx = sp + (nibIdx / 8) * 8 + ch * 4 + (nibIdx % 8) / 2;
                }
                int nib, step, diff;

                if (byteIdx >= blockAlign) {
                    src[(b * samplesPerBlock + i) * channels + ch] = (int16_t)pred[ch];
                    continue;
                }
                nib = (nibIdx & 1) ? (blk[byteIdx] >> 4) : (blk[byteIdx] & 0x0F);
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
    p->channels = 2;
    g_wav_pcm_total += outFrames * 4;
    p->durationMs = (int)(((int64_t)totalFrames * 1000) / rate);
    return 1;
}

/* ===================== MIDI (Standard MIDI File) playback =====================
 * Games using "audio/midi" players send us an SMF. We parse the events and
 * render the whole song once to 22050 Hz mono PCM with a small software synth
 * (16 voices, 4 waveforms, simple envelopes). The rendered buffer is played
 * through the same player/mixer path as WAV sounds.
 */
#define MIDI_MAX_VOICES 16
#define MIDI_MAX_EVENTS 65536

typedef struct {
    int tick;
    unsigned char type;     /* 0x80 note off, 0x90 note on, 0xB0 cc, 0xC0 prog, 0xFF meta */
    unsigned char ch;
    unsigned char d1, d2;
    int value;              /* tempo for 0xFF 0x51 */
} midi_event_t;

typedef struct {
    int note, ch, prog;
    int gain;               /* 0..127 */
    int active;
    int release;
    uint32_t phase;
    uint32_t inc;
    int32_t env;            /* 0..32767 */
    int wave;
    int order;
} midi_voice_t;

static int8_t midi_wave[4][256];
static int midi_wave_ready = 0;

static void midi_init_waves(void) {
    int i;
    if (midi_wave_ready) return;
    midi_wave_ready = 1;
    for (i = 0; i < 256; i++) {
        /* sine (fast parabola approximation, no libm needed) */
        double x = (double)i / 256.0;
        double v;
        if (x < 0.5) {
            v = 4.0 * x * (1.0 - x);
            v = 0.775 * v + 0.225 * v * v;
        } else {
            double y = 2.0 * x - 1.0;
            double w = 1.0 - y;
            double q = 4.0 * w * (1.0 - w);
            q = 0.775 * q + 0.225 * q * q;
            v = -q;
        }
        midi_wave[0][i] = (int8_t)(v * 120.0);
        /* triangle */
        if (i < 128) midi_wave[1][i] = (int8_t)((i * 2 - 128) * 120 / 128);
        else midi_wave[1][i] = (int8_t)((384 - i * 2) * 120 / 128);
        /* square (softened edge at transitions to reduce harsh buzz) */
        if (i == 0 || i == 128) midi_wave[2][i] = 0;
        else if (i < 128) midi_wave[2][i] = 120;
        else midi_wave[2][i] = -120;
        /* saw */
        midi_wave[3][i] = (int8_t)(((i - 128) * 120) / 128);
    }
}

static int midi_prog_wave(int prog) {
    if (prog < 8) return 1;      /* piano */
    if (prog < 16) return 0;     /* chromatic percussion */
    if (prog < 24) return 2;     /* organ */
    if (prog < 32) return 3;     /* guitar */
    if (prog < 40) return 0;     /* bass */
    if (prog < 56) return 3;     /* strings */
    if (prog < 64) return 2;     /* brass */
    if (prog < 72) return 2;     /* reed */
    if (prog < 80) return 0;     /* pipe */
    if (prog < 88) return 3;     /* synth lead */
    if (prog < 96) return 1;     /* synth pad */
    return 0;
}

static int midi_vlq(unsigned char *d, int *pos, int end) {
    int value = 0;
    while (*pos < end) {
        int b = d[(*pos)++];
        value = (value << 7) | (b & 0x7F);
        if (!(b & 0x80)) break;
    }
    return value;
}

static int midi_cmp(const void *a, const void *b) {
    int ta = ((const midi_event_t *)a)->tick;
    int tb = ((const midi_event_t *)b)->tick;
    return (ta > tb) - (ta < tb);
}

static void midi_add_event(midi_event_t **pev, int *pnev, int *pcap, int tick,
                           int type, int ch, int d1, int d2, int value) {
    midi_event_t *ev = *pev;
    if (*pnev >= *pcap) {
        int newcap = (*pcap < MIDI_MAX_EVENTS) ? (*pcap * 2) : *pcap;
        midi_event_t *np;
        if (newcap > MIDI_MAX_EVENTS) newcap = MIDI_MAX_EVENTS;
        if (newcap == *pcap) return;   /* full: drop */
        np = (midi_event_t *)realloc(ev, sizeof(midi_event_t) * newcap);
        if (!np) return;
        *pev = ev = np;
        *pcap = newcap;
    }
    ev[*pnev].tick = tick;
    ev[*pnev].type = (unsigned char)type;
    ev[*pnev].ch = (unsigned char)ch;
    ev[*pnev].d1 = (unsigned char)d1;
    ev[*pnev].d2 = (unsigned char)d2;
    ev[*pnev].value = value;
    (*pnev)++;
}

static void midi_synth(midi_voice_t *voices, int16_t *out, int frames) {
    int i, j;
    int n_active = 0;
    for (j = 0; j < MIDI_MAX_VOICES; j++) {
        if (voices[j].active) n_active++;
    }
    if (n_active == 0) {
        memset(out, 0, (size_t)frames * sizeof(int16_t));
        return;
    }

    /* Dynamic shift based on polyphony to prevent clipping/crackling */
    int shift = 7;
    if (n_active > 8) shift = 10;
    else if (n_active > 4) shift = 9;
    else if (n_active > 2) shift = 8;

    for (i = 0; i < frames; i++) {
        int32_t acc = 0;
        for (j = 0; j < MIDI_MAX_VOICES; j++) {
            midi_voice_t *v = &voices[j];
            if (!v->active) continue;

            /* Linear interpolation in wavetable:
             * v->phase is 16.16 fixed-point (65536 = 1 full cycle of 256 samples).
             * Bits [15:8] select sample index (0..255).
             * Bits [7:0] select fraction (0..255) between index and index+1. */
            uint8_t idx = (uint8_t)((v->phase >> 8) & 0xFF);
            uint8_t frac = (uint8_t)(v->phase & 0xFF);
            int32_t s0 = midi_wave[v->wave][idx];
            int32_t s1 = midi_wave[v->wave][(idx + 1) & 0xFF];
            int32_t sample = (s0 * (256 - frac) + s1 * frac) >> 8;

            acc += sample * (v->env >> 8) * v->gain;

            if (!v->release) {
                if (v->env < 32767) {
                    /* Smooth attack (~10ms) to eliminate note-on clicks */
                    v->env += (v->ch == 9) ? 400 : 150;
                    if (v->env > 32767) v->env = 32767;
                }
                if (v->ch == 9) {
                    /* Percussion naturally decays without waiting for note-off */
                    v->release = 1;
                }
            } else {
                /* Smooth release to eliminate note-off clicks */
                int decay = (v->ch == 9) ? 120 : 50;
                v->env -= decay;
                if (v->env <= 0) {
                    v->env = 0;
                    v->active = 0;
                }
            }
            v->phase += v->inc;
        }
        acc >>= shift;
        if (acc > 32767) acc = 32767;
        else if (acc < -32768) acc = -32768;
        out[i] = (int16_t)acc;
    }
}

/* Render an SMF to 22050 Hz mono PCM. Returns 1 on success. */
/* ---- Streaming SMF synthesis ----------------------------------------
 *
 * GB300 fix: midi_render() used to synthesise the whole track in one go from
 * nBuffering(), i.e. while the MIDlet thread is inside
 * Manager.createPlayer(...,"audio/midi"). A 124 s track meant 2.7 M frames of
 * soft-float synthesis blocking the game thread - Galaxy on Fire 2 never got
 * past its loading screen because of it.
 *
 * Parsing the SMF is cheap, so we do that up front and keep the event list.
 * The actual synthesis now happens in small chunks from the mixer
 * (midi_stream_render via wav_mix_frame), so the game thread is never blocked
 * and audio is produced in real time while it plays.
 */
struct midi_stream_s {
    midi_event_t *ev;
    int nev;
    int evcap;
    int ev_i;                /* next event to process */
    int division;            /* PPQN */
    int64_t curTick;
    int64_t tempo;           /* microseconds per quarter note */
    int totalTicks;
    midi_voice_t voices[MIDI_MAX_VOICES];
    int chanProg[16], chanVol[16], chanExpr[16];
    int order;
    int done;                /* all events consumed */
    int looped;              /* one loop of the track has been rendered */
};
static void midi_stream_free(midi_stream_t *ms) {
    if (!ms) return;
    if (ms->ev) free(ms->ev);
    free(ms);
}

static int midi_stream_render(wav_player_t *p, int frames);

/* Parse the SMF in p->raw into a new stream. Returns 1 on success. */
static int midi_stream_open(wav_player_t *p) {
    unsigned char *d = p->raw;
    int n = p->rawLen;
    int hlen, ntrks, division, pos, i, j;
    int nev = 0, evcap = 4096;
    midi_event_t *ev;
    midi_stream_t *ms;
    int64_t maxTick = 0;

    if (!d || n < 14 || memcmp(d, "MThd", 4) != 0) return 0;
    hlen = (d[4] << 24) | (d[5] << 16) | (d[6] << 8) | d[7];
    if (hlen < 6 || 8 + hlen > n) return 0;
    ntrks = (d[10] << 8) | d[11];
    division = (d[12] << 8) | d[13];
    if (division <= 0 || (division & 0x8000)) return 0;   /* PPQN only */

    ev = (midi_event_t *)malloc(sizeof(midi_event_t) * evcap);
    if (!ev) return 0;

    pos = 8 + hlen;
    for (i = 0; i < ntrks && pos + 8 <= n; i++) {
        int tlen, tend, tick = 0, running = 0;
        if (memcmp(d + pos, "MTrk", 4) != 0) break;
        tlen = (d[pos + 4] << 24) | (d[pos + 5] << 16) | (d[pos + 6] << 8) | d[pos + 7];
        pos += 8;
        tend = pos + tlen;
        if (tend > n) tend = n;
        while (pos < tend) {
            int delta = midi_vlq(d, &pos, tend);
            int status;
            tick += delta;
            if (pos >= tend) break;
            status = d[pos];
            if (status & 0x80) { pos++; running = status; } else { status = running; }
            if (status == 0xFF) {
                int mtype, mlen;
                if (pos >= tend) break;
                mtype = d[pos++];
                mlen = midi_vlq(d, &pos, tend);
                if (mtype == 0x51 && mlen == 3 && pos + 3 <= tend) {
                    int tempoVal = (d[pos] << 16) | (d[pos + 1] << 8) | d[pos + 2];
                    midi_add_event(&ev, &nev, &evcap, tick, 0xFF, 0, 0x51, 0, tempoVal);
                }
                pos += mlen;
            } else if (status == 0xF0 || status == 0xF7) {
                int slen = midi_vlq(d, &pos, tend);
                pos += slen;
            } else {
                int type = status & 0xF0;
                int ch = status & 0x0F;
                if (type == 0xC0 || type == 0xD0) {
                    int d1 = (pos < tend) ? d[pos++] : 0;
                    if (type == 0xC0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0xC0, ch, d1, 0, 0);
                    }
                } else {
                    int d1 = (pos < tend) ? d[pos++] : 0;
                    int d2 = (pos < tend) ? d[pos++] : 0;
                    if (type == 0x90 && d2 > 0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0x90, ch, d1, d2, 0);
                    } else if (type == 0x80 || (type == 0x90 && d2 == 0)) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0x80, ch, d1, 0, 0);
                    } else if (type == 0xB0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0xB0, ch, d1, d2, 0);
                    }
                }
            }
        }
        pos = tend;
    }

    if (nev <= 0) { free(ev); return 0; }
    qsort(ev, nev, sizeof(midi_event_t), midi_cmp);

    for (i = 0; i < nev; i++) {
        if (ev[i].tick > maxTick) maxTick = ev[i].tick;
    }

    ms = (midi_stream_t *)calloc(1, sizeof(midi_stream_t));
    if (!ms) { free(ev); return 0; }
    ms->ev = ev;
    ms->nev = nev;
    ms->evcap = evcap;
    ms->ev_i = 0;
    ms->division = division;
    ms->curTick = 0;
    ms->tempo = 500000;
    ms->totalTicks = (int)maxTick;
    midi_init_waves();
    for (j = 0; j < MIDI_MAX_VOICES; j++) memset(&ms->voices[j], 0, sizeof(midi_voice_t));
    for (j = 0; j < 16; j++) { ms->chanProg[j] = 0; ms->chanVol[j] = 100; ms->chanExpr[j] = 127; }

    if (p->midi) midi_stream_free((midi_stream_t *)p->midi);
    p->midi = ms;
    p->channels = 1;

    /* GB300: make sure the soundfont synth is up before we start a track. */
    midi_fluid_synth();

    /* GB300: render a bounded prefix once, at load time.
     *
     * Synthesising the whole track here blocked the MIDlet thread for
     * minutes (a 64 s track is 1.4 M frames of soft-float synthesis), and
     * synthesising on demand from the mixer instead made the CPU cost
     * continuous, which hung games in their menu on GB300. A short prefix
     * keeps the one-off load cost small and leaves the mixer free: the
     * prefix simply loops, and DirectPlayer restarts the player on stop. */
    {
        int64_t totalFrames = (((int64_t)ms->totalTicks * ms->tempo) /
                               (1000000LL * ms->division)) * WAV_PCM_RATE;
        int prefix = WAV_PCM_RATE * 10;
        if (prefix > WAV_MAX_PCM_TOTAL / 2) prefix = WAV_MAX_PCM_TOTAL / 2;
        if (totalFrames > 0 && prefix > (int)totalFrames) prefix = (int)totalFrames;
        if (prefix > 0) {
            /* Only a small head start here. Rendering the whole 10 s prefix
             * in one call blocked the MIDlet thread for the full synthesis
             * (Asphalt froze to 0 FPS when its music started, Doom stuttered
             * on every new track). The mixer tops the buffer up incrementally
             * instead, so no single call is expensive. */
            int head = g_fl_synth ? (WAV_PCM_RATE / 4) : (WAV_PCM_RATE / 2);      /* 250ms / 500ms */
            if (prefix > head) prefix = head;
            midi_stream_render(p, prefix);
            p->loopMidi = (totalFrames > 0 && (int64_t)ms->totalTicks * ms->tempo /
                           (1000000LL * ms->division) * WAV_PCM_RATE > p->pcmFrames);
            xlog("[AUDIO] MIDI head: %d klatek (bufor uzupelniany na biezaco)\n",
                 p->pcmFrames);
        }
    }

    /* Duration of the track, so Player.getDuration() still works.
     * seconds = ticks * tempo / (1e6 * PPQN), milliseconds = seconds * 1000. */
    p->durationMs = (int)(((int64_t)maxTick * ms->tempo) / (1000000LL * division) * 1000);
    {
        int64_t totalFrames = (((int64_t)maxTick * ms->tempo) / (1000000LL * division)) * WAV_PCM_RATE;
        xlog("[AUDIO] MIDI opened: %d events, %d ticks (~%d ms, %d frames), streaming\n",
             nev, (int)maxTick, p->durationMs, (int)totalFrames);
    }
    return 1;
}

/* Synthesise the next `frames` frames of the stream into p->pcm. */
static int midi_stream_render(wav_player_t *p, int frames) {
    midi_stream_t *ms = (midi_stream_t *)p->midi;
    int budget, outPos, i, j, want;

    if (!ms || frames <= 0) return 0;

    budget = WAV_MAX_PCM_TOTAL - g_wav_pcm_total + p->pcmFrames * p->channels * 2;
    want = p->pcmFrames + frames;
    if (want * 2 > budget) want = budget / 2;
    if (want <= p->pcmFrames) return 0;                    /* out of PCM budget */
    if (want > WAV_PCM_RATE * 300) want = WAV_PCM_RATE * 300;   /* 5 min cap */

    {
        int16_t *np = (int16_t *)realloc(p->pcm, (size_t)want * 2);
        if (!np) return 0;
        p->pcm = np;
    }
    memset(p->pcm + p->pcmFrames, 0, (size_t)(want - p->pcmFrames) * 2);
    outPos = p->pcmFrames;
    p->pcmFrames = want;
    g_wav_pcm_total += (want - outPos) * 2;

    while (ms->ev_i < ms->nev && outPos < p->pcmFrames) {
        midi_event_t *e = &ms->ev[ms->ev_i];
        int64_t target = outPos + (int64_t)(e->tick - ms->curTick) * ms->tempo * WAV_PCM_RATE /
                                   (1000000LL * ms->division);
        if (target > p->pcmFrames) target = p->pcmFrames;
        if (target > outPos) {
            int n = (int)(target - outPos);
            if (g_fl_synth) {
                /* FluidLite renders straight into our int16 frame buffer. */
                fluid_synth_write_s16(g_fl_synth, n,
                                      p->pcm + outPos, 0, p->channels,
                                      p->pcm + outPos, 0, p->channels);
            } else {
                midi_synth(ms->voices, p->pcm + outPos, n);
            }
            outPos = (int)target;
        }
        ms->curTick = e->tick;
        ms->ev_i++;

        if (e->type == 0xFF) {
            if (e->d1 == 0x51 && e->value > 0) ms->tempo = e->value;
        } else if (e->type == 0x90) {
            int ch = e->ch, note = e->d1 & 0x7F, vel = e->d2 & 0x7F;
            if (g_fl_synth) {
                /* channel 9 is the drum kit in General MIDI */
                fluid_synth_noteon(g_fl_synth, ch, note, vel);
                ms->curTick = e->tick;
                ms->ev_i++;
                continue;
            }
            midi_voice_t *v = NULL;
            for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (ms->voices[j].active && ms->voices[j].ch == ch && ms->voices[j].note == note) { v = &ms->voices[j]; break; }
            }
            if (!v) for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (!ms->voices[j].active) { v = &ms->voices[j]; break; }
            }
            if (!v) {
                int best = 0;
                for (j = 1; j < MIDI_MAX_VOICES; j++) {
                    if (ms->voices[j].order < ms->voices[best].order) best = j;
                }
                v = &ms->voices[best];
            }
            v->gain = (vel * ms->chanVol[ch] * ms->chanExpr[ch]) / (127 * 127);
            if (v->gain > 127) v->gain = 127;
            if (v->gain < 0) v->gain = 0;
            v->note = note;
            v->ch = ch;
            v->prog = ms->chanProg[ch];
            if (ch == 9) {
                v->wave = (note <= 36) ? 0 : 3;
            } else {
                v->wave = midi_prog_wave(ms->chanProg[ch]);
            }
            v->phase = 0;
            v->inc = midi_note_phase_step[note];
            v->env = 0;
            v->release = 0;
            v->active = 1;
            v->order = ms->order++;
        } else if (e->type == 0x80) {
            int ch = e->ch, note = e->d1 & 0x7F;
            if (g_fl_synth) {
                fluid_synth_noteoff(g_fl_synth, ch, note);
                ms->curTick = e->tick;
                ms->ev_i++;
                continue;
            }
            for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (ms->voices[j].active && ms->voices[j].ch == ch && ms->voices[j].note == note) {
                    ms->voices[j].release = 1;
                }
            }
        } else if (e->type == 0xC0) {
            ms->chanProg[e->ch & 0x0F] = e->d1 & 0x7F;
            if (g_fl_synth) {
                fluid_synth_program_change(g_fl_synth, e->ch & 0x0F, e->d1 & 0x7F);
            }
        } else if (e->type == 0xB0) {
            int cc = e->d1 & 0x7F, val = e->d2 & 0x7F;
            if (cc == 7) ms->chanVol[e->ch & 0x0F] = val;
            else if (cc == 11) ms->chanExpr[e->ch & 0x0F] = val;
            if (g_fl_synth) {
                fluid_synth_cc(g_fl_synth, e->ch & 0x0F, cc, val);
            }
        }
    }
    if (ms->ev_i >= ms->nev) ms->done = 1;
    return 1;
}

/* Restart a finished stream so looping players can play again. */
static void midi_stream_rewind(wav_player_t *p) {
    midi_stream_t *ms = (midi_stream_t *)p->midi;
    int j;
    if (!ms) return;
    ms->ev_i = 0;
    ms->curTick = 0;
    ms->tempo = 500000;
    ms->order = 0;
    ms->done = 0;
    for (j = 0; j < MIDI_MAX_VOICES; j++) memset(&ms->voices[j], 0, sizeof(midi_voice_t));
    for (j = 0; j < 16; j++) { ms->chanProg[j] = 0; ms->chanVol[j] = 100; ms->chanExpr[j] = 127; }
    p->pcmFrames = 0;
    p->pos = 0;
}

static int midi_render(wav_player_t *p) {
    unsigned char *d = p->raw;
    int n = p->rawLen;
    int hlen, ntrks, division, pos, i, j;
    int nev = 0, evcap = 4096;
    midi_event_t *ev;
    midi_voice_t voices[MIDI_MAX_VOICES];
    int chanProg[16], chanVol[16], chanExpr[16];
    int16_t *out;
    int budget, outCap, outPos = 0, order = 0;
    int64_t curTick = 0, tempo = 500000;

    if (!d || n < 14 || memcmp(d, "MThd", 4) != 0) return 0;
    hlen = (d[4] << 24) | (d[5] << 16) | (d[6] << 8) | d[7];
    if (hlen < 6 || 8 + hlen > n) return 0;
    ntrks = (d[10] << 8) | d[11];
    division = (d[12] << 8) | d[13];
    if (division <= 0 || (division & 0x8000)) return 0;   /* PPQN only */

    ev = (midi_event_t *)malloc(sizeof(midi_event_t) * evcap);
    if (!ev) return 0;

    pos = 8 + hlen;
    for (i = 0; i < ntrks && pos + 8 <= n; i++) {
        int tlen, tend, tick = 0, running = 0;
        if (memcmp(d + pos, "MTrk", 4) != 0) break;
        tlen = (d[pos + 4] << 24) | (d[pos + 5] << 16) | (d[pos + 6] << 8) | d[pos + 7];
        pos += 8;
        tend = pos + tlen;
        if (tend > n) tend = n;
        while (pos < tend) {
            int delta = midi_vlq(d, &pos, tend);
            int status;
            tick += delta;
            if (pos >= tend) break;
            status = d[pos];
            if (status & 0x80) { pos++; running = status; } else { status = running; }
            if (status == 0xFF) {
                int mtype, mlen;
                if (pos >= tend) break;
                mtype = d[pos++];
                mlen = midi_vlq(d, &pos, tend);
                if (mtype == 0x51 && mlen == 3 && pos + 3 <= tend) {
                    int tempoVal = (d[pos] << 16) | (d[pos + 1] << 8) | d[pos + 2];
                    midi_add_event(&ev, &nev, &evcap, tick, 0xFF, 0, 0x51, 0, tempoVal);
                }
                pos += mlen;
            } else if (status == 0xF0 || status == 0xF7) {
                int slen = midi_vlq(d, &pos, tend);
                pos += slen;
            } else {
                int type = status & 0xF0;
                int ch = status & 0x0F;
                if (type == 0xC0 || type == 0xD0) {
                    int d1 = (pos < tend) ? d[pos++] : 0;
                    if (type == 0xC0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0xC0, ch, d1, 0, 0);
                    }
                } else {
                    int d1 = (pos < tend) ? d[pos++] : 0;
                    int d2 = (pos < tend) ? d[pos++] : 0;
                    if (type == 0x90 && d2 > 0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0x90, ch, d1, d2, 0);
                    } else if (type == 0x80 || (type == 0x90 && d2 == 0)) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0x80, ch, d1, 0, 0);
                    } else if (type == 0xB0) {
                        midi_add_event(&ev, &nev, &evcap, tick, 0xB0, ch, d1, d2, 0);
                    }
                }
            }
        }
        pos = tend;
    }

    if (nev <= 0) { free(ev); return 0; }
    qsort(ev, nev, sizeof(midi_event_t), midi_cmp);

    midi_init_waves();
    for (i = 0; i < MIDI_MAX_VOICES; i++) memset(&voices[i], 0, sizeof(midi_voice_t));
    for (i = 0; i < 16; i++) { chanProg[i] = 0; chanVol[i] = 100; chanExpr[i] = 127; }

    budget = WAV_MAX_PCM_TOTAL - g_wav_pcm_total;
    outCap = budget / 2;                                  /* mono s16 */
    if (outCap > WAV_PCM_RATE * 300) outCap = WAV_PCM_RATE * 300;   /* 5 min cap */
    if (outCap <= 0) { free(ev); return 0; }
    out = (int16_t *)malloc((size_t)outCap * 2);
    if (!out) { free(ev); return 0; }

    for (i = 0; i < nev && outPos < outCap; i++) {
        midi_event_t *e = &ev[i];
        int64_t target = outPos + (int64_t)(e->tick - curTick) * tempo * WAV_PCM_RATE /
                                   (1000000LL * division);
        if (target > outCap) target = outCap;
        if (target > outPos) {
            midi_synth(voices, out + outPos, (int)(target - outPos));
            outPos = (int)target;
        }
        curTick = e->tick;

        if (e->type == 0xFF) {
            if (e->d1 == 0x51 && e->value > 0) tempo = e->value;
        } else if (e->type == 0x90) {
            int ch = e->ch, note = e->d1 & 0x7F, vel = e->d2 & 0x7F;
            midi_voice_t *v = NULL;
            for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (voices[j].active && voices[j].ch == ch && voices[j].note == note) { v = &voices[j]; break; }
            }
            if (!v) for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (!voices[j].active) { v = &voices[j]; break; }
            }
            if (!v) {
                int best = 0;
                for (j = 1; j < MIDI_MAX_VOICES; j++) {
                    if (voices[j].order < voices[best].order) best = j;
                }
                v = &voices[best];
            }
            v->gain = (vel * chanVol[ch] * chanExpr[ch]) / (127 * 127);
            if (v->gain > 127) v->gain = 127;
            if (v->gain < 0) v->gain = 0;
            v->note = note;
            v->ch = ch;
            v->prog = chanProg[ch];
            if (ch == 9) {
                v->wave = (note <= 36) ? 0 : 3;
            } else {
                v->wave = midi_prog_wave(chanProg[ch]);
            }
            v->phase = 0;
            v->inc = midi_note_phase_step[note];
            v->env = 0;
            v->release = 0;
            v->active = 1;
            v->order = order++;
        } else if (e->type == 0x80) {
            int ch = e->ch, note = e->d1 & 0x7F;
            for (j = 0; j < MIDI_MAX_VOICES; j++) {
                if (voices[j].active && voices[j].ch == ch && voices[j].note == note) {
                    voices[j].release = 1;
                }
            }
        } else if (e->type == 0xC0) {
            chanProg[e->ch & 0x0F] = e->d1 & 0x7F;
        } else if (e->type == 0xB0) {
            int cc = e->d1 & 0x7F, val = e->d2 & 0x7F;
            if (cc == 7) chanVol[e->ch & 0x0F] = val;
            else if (cc == 11) chanExpr[e->ch & 0x0F] = val;
        }
    }

    free(ev);
    if (outPos <= 0) { free(out); return 0; }
    p->pcm = out;
    p->pcmFrames = outPos;
    p->channels = 1;
    g_wav_pcm_total += outPos * 2;
    p->durationMs = (int)(((int64_t)outPos * 1000) / WAV_PCM_RATE);
    xlog("[AUDIO] MIDI rendered: %d frames (%d ms, %d events)\n", outPos, p->durationMs, nev);
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
        p->channels = 2;
        g_wav_pcm_total += outFrames * 4;
        p->durationMs = (int)(((int64_t)srcFrames * 1000) / rate);
        return 1;
    }
}

/* Top up MIDI buffers for active players. Called once per gb300_audio_read(). */
static void wav_topup_midi(void) {
    int i;
    for (i = 0; i < WAV_MAX_PLAYERS; i++) {
        wav_player_t *p = &g_wav[i];
        if (!p->used || !p->playing || p->paused || !p->midi || !p->pcm) continue;

        midi_stream_t *ms = (midi_stream_t *)p->midi;
        int remaining = p->pcmFrames - p->pos;

        /* The track ran out of events and we already buffered all of
         * it: rewind if looping. */
        if (ms && ms->done && remaining > 0 && remaining < WAV_PCM_RATE / 4) {
            if (p->loopMidi) {
                ms->ev_i = 0;
                ms->curTick = 0;
                ms->done = 0;
                ms->looped = 0;
                p->pcmFrames = 0;
                p->pos = 0;
                remaining = 0;
            }
        }

        /* Only synthesise while there are events left.
         * If using the fast built-in synth (g_fl_synth == NULL), each player has
         * its own voices and synthesis is integer-only, so we don't gate it. */
        if (ms && !ms->done && g_midi_budget > 0 && (!g_fl_synth || !g_midi_synth_used)) {
            if (remaining < WAV_PCM_RATE * 2) {
                int deficit = WAV_PCM_RATE * 2 - remaining;
                int chunk = (deficit > MIDI_TOPUP_MAX) ? MIDI_TOPUP_MAX : deficit;
                if (chunk > g_midi_budget) chunk = g_midi_budget;
                if (midi_stream_render(p, chunk)) {
                    g_midi_budget -= chunk;
                    g_midi_synth_used = 1;
                }
            }
        }
    }
}

/* Mix one output frame (22050 Hz stereo) of all active WAV players */
static void wav_mix_frame(int32_t *l, int32_t *r) {
    int i;
    for (i = 0; i < WAV_MAX_PLAYERS; i++) {
        wav_player_t *p = &g_wav[i];
        if (!p->used || !p->playing || p->paused || !p->pcm) continue;

        if (p->pos >= p->pcmFrames) {
            if (p->midi && p->loopMidi) {
                p->pos = 0;              /* seamless loop */
            } else {
                p->playing = 0;
                p->pos = 0;
                continue;
            }
        }

        if (p->pos < p->pcmFrames) {
            int vol = p->muted ? 0 : p->volume;
            int sl = p->pcm[p->pos * p->channels];
            int sr = (p->channels > 1) ? p->pcm[p->pos * p->channels + 1] : sl;
            *l += (sl * vol) / 100;
            *r += (sr * vol) / 100;
            p->pos++;
            if (!p->midi && p->pos >= p->pcmFrames) {
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

    g_midi_budget = MIDI_BUDGET_PER_READ;
    g_midi_synth_used = 0;

    wav_topup_midi();

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
            if (p->rawLen >= 4 && memcmp(p->raw, "MThd", 4) == 0) {
                /* Only parse here; the samples are synthesised on demand by
                 * wav_mix_frame so that createPlayer() returns immediately. */
                if (!midi_stream_open(p)) {
                    xlog("[AUDIO] handle %d: MIDI parse failed (%d bytes)\n", handle, p->rawLen);
                }
            } else if (!wav_decode(p)) {
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
