#!/bin/sh
# build_mascotme.sh [classes.zip]
#
# Compiles the vendored MascotME (Mascot Capsule Micro3D v3 for J2ME) sources
# and adds the resulting classes into classes.zip, the MIDP class library that
# the core loads from /mnt/sda1/bios/classes.zip on the device.
#
# MascotME is built for CLDC: Java 1.4 class files (max supported by the KVM
# is 52, but 1.4 also keeps StringBuffer instead of the unavailable
# StringBuilder) and with -inlineJSR because the CLDC VM rejects jsr/ret
# bytecode.
#
# Requires: java (JRE 11+), python3, curl (only on the first run to fetch ECJ).
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC_DIR="$ROOT/pspkvm/third_party/mascotme/src"
OUT_DIR="$(mktemp -d)"
ECJ_JAR="${ECJ_JAR:-/tmp/ecj.jar}"
CLASSES_ZIP="${1:-$ROOT/classes.zip}"

if ! command -v java >/dev/null 2>&1; then
    echo "error: java not found" >&2
    exit 1
fi

if [ ! -f "$CLASSES_ZIP" ]; then
    echo "error: $CLASSES_ZIP not found (pass classes.zip as the first argument)" >&2
    exit 1
fi

if [ ! -f "$ECJ_JAR" ]; then
    echo "Downloading Eclipse Compiler for Java (ECJ)..."
    curl -sL -o "$ECJ_JAR" \
        "https://repo1.maven.org/maven2/org/eclipse/jdt/ecj/3.38.0/ecj-3.38.0.jar"
fi

echo "Compiling MascotME..."
java -jar "$ECJ_JAR" \
    -source 1.4 -target 1.4 -inlineJSR \
    -bootclasspath "$CLASSES_ZIP" \
    -d "$OUT_DIR" $(find "$SRC_DIR" -name "*.java")

python3 - "$CLASSES_ZIP" "$OUT_DIR" <<'PY'
import sys, os, zipfile

zip_path, out_dir = sys.argv[1], sys.argv[2]
tmp_path = zip_path + ".tmp"

with zipfile.ZipFile(zip_path) as zin, \
     zipfile.ZipFile(tmp_path, "w", zipfile.ZIP_DEFLATED) as zout:
    # keep everything except a previous MascotME install
    for item in zin.infolist():
        if not item.filename.startswith("com/mascotcapsule/"):
            zout.writestr(item, zin.read(item.filename))
    # add the freshly compiled classes
    count = 0
    for root, _, files in os.walk(out_dir):
        for f in files:
            if f.endswith(".class"):
                p = os.path.join(root, f)
                zout.write(p, os.path.relpath(p, out_dir).replace(os.sep, "/"))
                count += 1

os.replace(tmp_path, zip_path)
print("Added %d MascotME classes to %s" % (count, zip_path))
PY
