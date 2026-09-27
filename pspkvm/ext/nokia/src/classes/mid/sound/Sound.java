package com.nokia.mid.sound;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import javax.microedition.media.Manager;
import javax.microedition.media.Player;
import javax.microedition.media.PlayerListener;
import javax.microedition.media.control.VolumeControl;

public class Sound {
    public static final int SOUND_PLAYING = 0;
    public static final int SOUND_STOPPED = 1;
    public static final int SOUND_UNINITIALIZED = 3;

    public static final int FORMAT_TONE = 1;
    public static final int FORMAT_WAV = 5;

    private static final int[] BPM_TABLE = {
        25, 28, 31, 35, 40, 45, 50, 56, 63, 70, 80, 90, 100, 112, 125, 140,
        160, 180, 200, 225, 250, 285, 320, 355, 400, 450, 500, 565, 635, 715, 800, 900
    };

    private static final int[] NOTE_TO_SEMITONE = {
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    };

    private static final short[] FREQ_TABLE = {
        0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 26, 27, 29,
        30, 32, 34, 36, 38, 41, 43, 45, 48, 51, 54, 57, 60, 64, 68, 72, 76, 81, 85, 90, 96,
        101, 107, 114, 120, 128, 135, 143, 152, 161, 170, 180, 191, 202, 214, 227, 240, 255,
        270, 286, 303, 321, 340, 360, 381, 404, 428, 453, 480, 509, 539, 571, 605, 641, 679,
        719, 762, 807, 855, 906, 960, 1017, 1078, 1142, 1210, 1282, 1358, 1438, 1524, 1614,
        1710, 1812, 1920, 2034, 2155, 2283, 2419, 2563, 2715, 2876, 3047, 3228, 3420, 3624,
        3839, 4067, 4309, 4565, 4837, 5125, 5429, 5752, 6094, 6456, 6840, 7247, 7678, 8134,
        8618, 9130, 9673, 10249, 10858, 11504, 12188, 12912
    };

    public Player m_player;
    private Sound m_instance;
    private SoundListener listener;

    private int status = SOUND_UNINITIALIZED;
    private int type;
    public int dataLen;
    private int gain = 255;

    private short[] toneNotes;
    private int[] toneDurations;
    private TonePlayerThread activeToneThread;

    public Sound(byte data[], int _type) {
        type = _type;
        dataLen = (data != null) ? data.length : 0;
        m_instance = this;
        init(data, _type);
    }

    public Sound(int freq, long duration) {
        type = FORMAT_TONE;
        m_instance = this;
        init(freq, duration);
    }

    public String getType() {
        if (type == FORMAT_TONE) {
            return "FORMAT_TONE";
        } else if (type == FORMAT_WAV) {
            return "FORMAT_WAV";
        } else {
            return null;
        }
    }

    public static int getConcurrentSoundCount(int type) {
        return 1;
    }

    public int getGain() {
        if (m_player != null) {
            try {
                VolumeControl vc = (VolumeControl)m_player.getControl("VolumeControl");
                if (vc != null) return vc.getLevel();
            } catch (Exception e) {}
        }
        return gain;
    }

    public int getState() {
        return status;
    }

    public static int[] getSupportedFormats() {
        return new int[] { FORMAT_TONE, FORMAT_WAV };
    }

    public void init(byte data[], int _type) {
        if (data == null) throw new NullPointerException();

        stop();
        if (m_player != null) {
            m_player.deallocate();
            m_player = null;
        }
        toneNotes = null;
        toneDurations = null;
        type = _type;
        dataLen = data.length;

        String mimeType = "audio/midi";
        if (data.length >= 12 && data[0] == 'R' && data[1] == 'I' && data[2] == 'F' && data[3] == 'F') {
            if (data[8] == 'W' && data[9] == 'A' && data[10] == 'V' && data[11] == 'E') {
                _type = FORMAT_WAV;
                mimeType = "audio/x-wav";
            } else if (data[8] == 'R' && data[9] == 'M' && data[10] == 'I' && data[11] == 'D') {
                mimeType = "audio/midi";
            }
        } else if (data.length >= 4 && data[0] == 'F' && data[1] == 'O' && data[2] == 'R' && data[3] == 'M') {
            _type = FORMAT_WAV;
            mimeType = "audio/x-wav";
        } else if (data.length >= 4 && data[0] == 'M' && data[1] == 'T' && data[2] == 'h' && data[3] == 'd') {
            mimeType = "audio/midi";
        }

        if (_type == FORMAT_TONE) {
            if (data.length > 0 && data[0] == 0x02) {
                parseOtt(data);
                status = SOUND_STOPPED;
                return;
            }
        }

        try {
            InputStream is = new ByteArrayInputStream(data);
            m_player = Manager.createPlayer(is, mimeType);
            m_player.addPlayerListener(new PlayerListener() {
                public void playerUpdate(Player player, String event, Object eventData) {
                    if (event.equals(PlayerListener.STOPPED) || event.equals(PlayerListener.END_OF_MEDIA)) {
                        status = SOUND_STOPPED;
                        if (listener != null) {
                            try {
                                listener.soundStateChanged(m_instance, SOUND_STOPPED);
                            } catch (Exception e) {}
                        }
                    }
                }
            });
            is.close();
            status = SOUND_STOPPED;
        } catch (Exception e) {
            status = SOUND_UNINITIALIZED;
        }
    }

    public void init(int freq, long duration) {
        stop();
        if (m_player != null) {
            m_player.deallocate();
            m_player = null;
        }
        if (duration <= 0) duration = 100;
        int note = convertFreqToNote(freq);
        toneNotes = new short[] { (short)note };
        toneDurations = new int[] { (int)duration };
        type = FORMAT_TONE;
        status = SOUND_STOPPED;
    }

    public void play(int loop) {
        if (status == SOUND_PLAYING) {
            stop();
        }
        if (m_player != null) {
            try {
                m_player.setLoopCount(loop == 0 ? -1 : loop);
            } catch (Exception e) {}
            resume();
            return;
        }
        if (toneNotes != null && toneNotes.length > 0) {
            status = SOUND_PLAYING;
            if (listener != null) {
                try {
                    listener.soundStateChanged(this, SOUND_PLAYING);
                } catch (Exception e) {}
            }
            synchronized (this) {
                activeToneThread = new TonePlayerThread(toneNotes, toneDurations, loop);
                activeToneThread.start();
            }
        }
    }

    public void release() {
        stop();
        if (m_player != null) {
            try {
                m_player.deallocate();
            } catch (Exception e) {}
            m_player = null;
        }
        toneNotes = null;
        toneDurations = null;
        status = SOUND_UNINITIALIZED;
        if (listener != null) {
            try {
                listener.soundStateChanged(this, SOUND_UNINITIALIZED);
            } catch (Exception e) {}
        }
    }

    public void resume() {
        if (m_player != null) {
            try {
                m_player.start();
            } catch (Exception e) {}
            status = SOUND_PLAYING;
            if (listener != null) {
                try {
                    listener.soundStateChanged(this, SOUND_PLAYING);
                } catch (Exception e) {}
            }
            return;
        }
        if (toneNotes != null && toneNotes.length > 0 && status != SOUND_PLAYING) {
            play(1);
        }
    }

    public void setGain(int level) {
        this.gain = level;
        if (m_player != null) {
            try {
                VolumeControl vc = (VolumeControl)m_player.getControl("VolumeControl");
                if (vc != null) {
                    int vol = (level * 100) / 255;
                    if (vol > 100) vol = 100;
                    if (vol < 0) vol = 0;
                    vc.setLevel(vol);
                }
            } catch (Exception e) {}
        }
    }

    public void setSoundListener(SoundListener soundlistener) {
        listener = soundlistener;
    }

    public void stop() {
        if (m_player != null) {
            try {
                m_player.stop();
            } catch (Exception e) {}
        }
        synchronized (this) {
            if (activeToneThread != null) {
                activeToneThread.cancel();
                activeToneThread = null;
            }
        }
        try {
            com.sun.mmedia.TonePlayer tp = com.sun.mmedia.Configuration.getConfiguration().getTonePlayer();
            if (tp != null) tp.stopTone();
        } catch (Exception e) {}

        status = SOUND_STOPPED;
        if (listener != null) {
            try {
                listener.soundStateChanged(this, SOUND_STOPPED);
            } catch (Exception e) {}
        }
    }

    public static int convertFreqToNote(int freq) {
        if (freq <= 0) return 0;
        int low = 0;
        int high = FREQ_TABLE.length - 1;
        if (freq >= FREQ_TABLE[high]) return high;

        while (low <= high) {
            int mid = (low + high) >>> 1;
            int midVal = FREQ_TABLE[mid];
            if (midVal < freq) {
                low = mid + 1;
            } else if (midVal > freq) {
                high = mid - 1;
            } else {
                return mid;
            }
        }
        if (low > 0 && (freq - FREQ_TABLE[low - 1]) < (FREQ_TABLE[low] - freq)) {
            return low - 1;
        } else {
            return low;
        }
    }

    private void parseOtt(byte[] data) {
        try {
            BitReader br = new BitReader(data);
            br.skip(8); // command length
            br.skip(8); // ringing-tone-programming
            int partId = br.read(7);
            int charWidth = (partId == 0x22) ? 16 : 8; // 0b0100010 = 0x22
            if (partId == 0x22) {
                br.read(1);
                br.skip(7);
            }
            int songType = br.read(3);
            if (songType == 1) {
                int titleLen = br.read(4);
                for (int i = 0; i < titleLen; i++) {
                    br.skip(charWidth);
                }
            } else if (songType != 2) {
                return;
            }

            int seqLen = br.read(8);
            short[] tempNotes = new short[64];
            int[] tempDur = new int[64];
            int noteCount = 0;

            int currScale = 1; // default scale 1 (octave 5)
            int currBpm = 125;  // default 125 bpm

            for (int j = 0; j < seqLen && br.hasMore(17); j++) {
                br.skip(3); // pattern header id
                int patId = br.read(2);
                int loopVal = br.read(4);
                int patLen = br.read(8);

                for (int k = 0; k < patLen && br.hasMore(3); k++) {
                    int instId = br.read(3);
                    if (instId == 1) { // Note
                        if (!br.hasMore(9)) break;
                        int noteVal = br.read(4);
                        int noteDur = br.read(3);
                        int durSpec = br.read(2);

                        int durMs = (int)((240000L / currBpm) >> noteDur);
                        if (durSpec == 1) {
                            durMs = (durMs * 3) / 2;
                        } else if (durSpec == 2) {
                            durMs = (durMs * 7) / 4;
                        } else if (durSpec == 3) {
                            durMs = (durMs * 2) / 3;
                        }
                        if (durMs <= 0) durMs = 10;

                        short midiNote;
                        if (noteVal <= 0 || noteVal > 12) {
                            midiNote = -1; // pause / rest
                        } else {
                            midiNote = (short)(60 + currScale * 12 + NOTE_TO_SEMITONE[noteVal]);
                            if (midiNote < 0) midiNote = 0;
                            if (midiNote > 127) midiNote = 127;
                        }

                        if (noteCount >= tempNotes.length) {
                            short[] n2 = new short[tempNotes.length * 2];
                            System.arraycopy(tempNotes, 0, n2, 0, tempNotes.length);
                            tempNotes = n2;
                            int[] d2 = new int[tempDur.length * 2];
                            System.arraycopy(tempDur, 0, d2, 0, tempDur.length);
                            tempDur = d2;
                        }
                        tempNotes[noteCount] = midiNote;
                        tempDur[noteCount] = durMs;
                        noteCount++;
                    } else if (instId == 2) { // Scale
                        if (!br.hasMore(2)) break;
                        currScale = br.read(2);
                    } else if (instId == 3) { // Style
                        if (!br.hasMore(2)) break;
                        br.skip(2);
                    } else if (instId == 4) { // Tempo
                        if (!br.hasMore(5)) break;
                        int bpmIdx = br.read(5);
                        if (bpmIdx >= 0 && bpmIdx < BPM_TABLE.length) {
                            currBpm = BPM_TABLE[bpmIdx];
                        }
                    } else if (instId == 5) { // Volume
                        if (!br.hasMore(4)) break;
                        br.skip(4);
                    } else {
                        break;
                    }
                }
            }

            if (noteCount > 0) {
                toneNotes = new short[noteCount];
                System.arraycopy(tempNotes, 0, toneNotes, 0, noteCount);
                toneDurations = new int[noteCount];
                System.arraycopy(tempDur, 0, toneDurations, 0, noteCount);
            }
        } catch (Exception e) {}
    }

    private class TonePlayerThread extends Thread {
        private final short[] notes;
        private final int[] durations;
        private final int loopCount;
        private volatile boolean stopped = false;

        public TonePlayerThread(short[] notes, int[] durations, int loopCount) {
            this.notes = notes;
            this.durations = durations;
            this.loopCount = loopCount;
        }

        public void cancel() {
            stopped = true;
            interrupt();
        }

        public void run() {
            int loopsDone = 0;
            int targetLoops = (loopCount == 0) ? -1 : loopCount;

            try {
                while (!stopped && (targetLoops == -1 || loopsDone < targetLoops)) {
                    for (int i = 0; i < notes.length && !stopped; i++) {
                        short note = notes[i];
                        int dur = durations[i];
                        int vol = (gain * 100) / 255;
                        if (vol > 100) vol = 100;
                        if (vol < 0) vol = 0;

                        if (note >= 0) {
                            try {
                                Manager.playTone(note, dur, vol);
                            } catch (Exception e) {}
                        }
                        try {
                            Thread.sleep(dur);
                        } catch (InterruptedException ie) {
                            stopped = true;
                            break;
                        }
                    }
                    loopsDone++;
                }
            } finally {
                if (!stopped) {
                    status = SOUND_STOPPED;
                    if (listener != null) {
                        try {
                            listener.soundStateChanged(Sound.this, SOUND_STOPPED);
                        } catch (Exception e) {}
                    }
                }
                synchronized (Sound.this) {
                    if (activeToneThread == this) {
                        activeToneThread = null;
                    }
                }
            }
        }
    }

    private static class BitReader {
        private final byte[] data;
        private int pos;

        public BitReader(byte[] data) {
            this.data = data;
            this.pos = 0;
        }

        public int read(int length) {
            int p = pos / 8;
            int bit = pos % 8;
            if (p >= data.length) return 0;
            int d = (data[p] & 0xFF) << 8;
            if (bit + length > 8 && (p + 1) < data.length) {
                d |= (data[p + 1] & 0xFF);
            }
            pos += length;
            return (d >> (16 - bit - length)) & ((1 << length) - 1);
        }

        public void skip(int length) {
            pos += length;
        }

        public boolean hasMore(int length) {
            return (pos + length) <= (data.length * 8);
        }
    }
}
