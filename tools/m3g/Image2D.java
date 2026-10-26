package javax.microedition.m3g;

import javax.microedition.lcdui.Image;

/**
 * Replacement for the MIDP javax.microedition.m3g.Image2D.
 *
 * The shipped class defers the pixel upload to applyDeferred(), but
 * Object3D.construct() only calls createDeferred() for objects that were
 * created successfully, and flushDeferred() only walks objects whose
 * createDeferred() failed.  applyDeferred() therefore never ran, no
 * nSetImage() ever reached the renderer and every texture stayed white.
 *
 * This version uploads the pixels straight from createDeferred().
 */
public class Image2D extends Object3D {

    public static final int ALPHA           = 96;
    public static final int LUMINANCE       = 97;
    public static final int LUMINANCE_ALPHA = 98;
    public static final int RGB             = 99;
    public static final int RGBA            = 100;

    private static final int FLAG_DYNAMIC  = 2;
    private static final int FLAG_PALETTED = 4;

    private int format;
    private int width;
    private int height;
    private boolean mutable;
    private int createFlags;
    private byte[] pixels;
    private byte[] palette;

    Image2D() {
    }

    public Image2D(int format, int width, int height) {
        this.format = format;
        this.width = width;
        this.height = height;
        this.mutable = true;
        this.createFlags = FLAG_DYNAMIC;
        checkShape(format, width, height);
        construct();
    }

    public Image2D(int format, int width, int height, byte[] pixels) {
        if (pixels == null) {
            throw new NullPointerException();
        }
        this.format = format;
        this.width = width;
        this.height = height;
        this.mutable = true;
        this.createFlags = FLAG_DYNAMIC;
        checkShape(format, width, height);
        if (pixels.length < width * height * bytesPerPixel(format)) {
            throw new IllegalArgumentException("not enough pixel data");
        }
        this.pixels = pixels;
        construct();
    }

    public Image2D(int format, int width, int height, byte[] pixels, byte[] palette) {
        if (pixels == null || palette == null) {
            throw new NullPointerException();
        }
        this.format = format;
        this.width = width;
        this.height = height;
        this.mutable = false;
        this.createFlags = FLAG_DYNAMIC | FLAG_PALETTED;
        checkShape(format, width, height);
        this.pixels = pixels;
        this.palette = palette;
        construct();
    }

    public Image2D(int format, Object image) {
        if (image == null) {
            throw new NullPointerException();
        }
        if (!(image instanceof Image)) {
            throw new IllegalArgumentException("unsupported image object");
        }
        Image src = (Image) image;
        this.format = format;
        this.width = src.getWidth();
        this.height = src.getHeight();
        this.mutable = false;
        this.createFlags = FLAG_DYNAMIC;
        checkShape(format, width, height);
        int[] argb = new int[width * height];
        src.getRGB(argb, 0, width, 0, 0, width, height);
        this.pixels = pack(argb, format);
        construct();
    }

    void createDeferred() {
        handle = nCreate(format, width, height, createFlags);
        register();
        /* The fix: upload the data as soon as the handle exists. */
        if (handle != 0) {
            applyDeferred();
        }
    }

    void applyDeferred() {
        if (palette != null) {
            int entries = palette.length / bytesPerPixel(format);
            nSetPalette(handle, entries, palette);
        }
        if (pixels != null) {
            nSetImage(handle, pixels);
        }
        if (!mutable) {
            nCommit(handle);
        }
    }

    private static int bytesPerPixel(int format) {
        switch (format) {
        case ALPHA:
        case LUMINANCE:
            return 1;
        case LUMINANCE_ALPHA:
            return 2;
        case RGB:
            return 3;
        default:
            return 4;
        }
    }

    private static byte[] pack(int[] argb, int format) {
        int bpp = bytesPerPixel(format);
        byte[] out = new byte[argb.length * bpp];
        int o = 0;
        for (int i = 0; i < argb.length; i++) {
            int c = argb[i];
            int a = (c >>> 24) & 0xFF;
            int r = (c >>> 16) & 0xFF;
            int g = (c >>> 8) & 0xFF;
            int b = c & 0xFF;
            switch (format) {
            case ALPHA:
                out[o++] = (byte) a;
                break;
            case LUMINANCE:
                out[o++] = (byte) ((77 * r + 151 * g + 28 * b) >> 8);
                break;
            case LUMINANCE_ALPHA:
                out[o++] = (byte) ((77 * r + 151 * g + 28 * b) >> 8);
                out[o++] = (byte) a;
                break;
            case RGB:
                out[o++] = (byte) r;
                out[o++] = (byte) g;
                out[o++] = (byte) b;
                break;
            default:
                out[o++] = (byte) r;
                out[o++] = (byte) g;
                out[o++] = (byte) b;
                out[o++] = (byte) a;
                break;
            }
        }
        return out;
    }

    private static void checkShape(int format, int width, int height) {
        if (format < ALPHA || format > RGBA) {
            throw new IllegalArgumentException("invalid image format");
        }
        if (width <= 0 || height <= 0) {
            throw new IllegalArgumentException("image must be non-empty");
        }
    }

    public int getFormat() {
        return format;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public boolean isMutable() {
        return mutable;
    }

    public void set(int x, int y, int width, int height, byte[] pixels) {
        if (pixels == null) {
            throw new NullPointerException();
        }
        if (!mutable) {
            throw new IllegalStateException("image is immutable");
        }
        if (handle != 0) {
            nSetSubImage(handle, x, y, width, height, pixels);
        }
    }

    private static native int nCreate(int format, int width, int height, int flags);
    private static native void nSetImage(int handle, byte[] pixels);
    private static native void nSetPalette(int handle, int entries, byte[] palette);
    private static native void nSetSubImage(int handle, int x, int y, int w, int h, byte[] pixels);
    private static native void nCommit(int handle);
    private static native int nGetWidth(int handle);
    private static native int nGetHeight(int handle);
    private static native int nGetFormat(int handle);
}
