#!/bin/sh
# export_skin_files.sh <out_dir>
#
# Exports the MIDP Chameleon skin images that the J2ME core serves through
# ResourceHandler (see loadRomizedResource0 in platform_gb300/vm_stubs.c).
# The Java side asks for names such as "screen_image_wash_png" (the dots of
# the original file name are replaced with underscores), so the files are
# exported under those names and are meant to be copied to
# /mnt/sda1/system/skin/ on the device.
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BASE="$ROOT/pspkvm/midp/src/highlevelui/lcdlf/lfjava/resource/skin"
OPT="$ROOT/pspkvm/midp/src/highlevelui/lcdlf/lfjava/resource_optional/skin"
OUT="${1:?usage: export_skin_files.sh <out_dir>}"

mkdir -p "$OUT"

# base skin first, optional skin overrides it (same order as the MIDP build)
for f in "$BASE"/*.png "$OPT"/*.png; do
    [ -e "$f" ] || continue
    name="$(basename "$f" | tr '.' '_')"
    cp "$f" "$OUT/$name"
done

echo "Skin files exported to $OUT ($(ls "$OUT" | wc -l) files)"
