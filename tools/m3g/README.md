# m3g Image2D patch

`Image2D.java` replaces the class of the same name inside `classes.zip`.

## Why

The MIDP `javax.microedition.m3g` layer defers pixel uploads to
`Object3D.applyDeferred()`, but `Object3D.construct()` calls
`createDeferred()` directly for objects that were created successfully and
only queues objects whose `nCreate()` returned 0. `flushDeferred()` therefore
never calls `applyDeferred()` for a working object, `Image2D.nSetImage()` never
runs and every texture in every M3G game samples the renderer's white fallback.

The replacement uploads the pixels *from* `createDeferred()`, right after
`nCreate()` returned a handle.

## Rebuild

```sh
mkdir -p out
javac -source 8 -target 8 -bootclasspath /path/to/classes.zip \
      -d out Image2D.java
cd out && zip /path/to/classes.zip javax/microedition/m3g/Image2D.class
```

The CLDC VM in this port accepts class files up to major version 52.
