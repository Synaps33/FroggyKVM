#!/bin/bash
# FroggyKVM SF2000 build script
#
# Based on UAE4ALL build.sh - follow EXACT same patterns!

set -e

# Toolchain path
export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:$PATH

echo "=== FroggyKVM SF2000 Build ==="
echo ""

# Clean previous build
echo "Cleaning..."
make clean platform=sf2000 2>/dev/null || true

# Build
echo "Building..."
make platform=sf2000 -j4

echo ""
echo "=== Build complete ==="
echo "Output: j2me_libretro_sf2000.a"
echo ""
echo "To link final binary, copy to sf2000_multicore_official/cores/j2me/"
echo "and run: make CORE=cores/j2me CONSOLE=j2me core_87000000"
