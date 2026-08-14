#!/bin/bash
# FroggyKVM GB300 / SF2000 Build & Link Script
set -e

export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:/usr/bin:/bin:$PATH

echo "========================================"
echo "=== Building FroggyKVM for GB300 ==="
echo "========================================"
echo ""

# 1. Clean previous build
echo "[1/3] Cleaning previous build..."
make clean platform=sf2000 2>/dev/null || true

# 2. Build core static library
echo "[2/3] Compiling FroggyKVM library..."
make platform=sf2000 -j4

if [ ! -f "j2me_libretro_sf2000.a" ]; then
    echo "ERROR: j2me_libretro_sf2000.a not found!"
    exit 1
fi

# 3. Link for GB300 Multicore
echo "[3/3] Linking core_87000000 for GB300..."
MULTICORE=/mnt/c/Temp/gb300_multicore
mkdir -p ${MULTICORE}/cores/j2me

cp j2me_libretro_sf2000.a ${MULTICORE}/cores/j2me/

cat << 'EOF' > ${MULTICORE}/cores/j2me/Makefile
TARGET_NAME := j2me

ifeq ($(platform), sf2000)
	TARGET := $(TARGET_NAME)_libretro_$(platform).a
	STATIC_LINKING = 1
endif

all:
	@echo "Using pre-built $(TARGET)"

clean:
	@echo "Nothing to clean"

.PHONY: all clean
EOF

cd ${MULTICORE}
rm -f core_87000000 core.elf libretro_core.a 2>/dev/null || true
make CORE=cores/j2me CONSOLE=j2me core_87000000

cp core_87000000 /mnt/d/gb300/core_87000000
cp core_87000000 /mnt/d/gb300/core_87000000_j2me
mkdir -p /mnt/d/gb300/j2me
cp core_87000000 /mnt/d/gb300/j2me/core_87000000

cd /mnt/d/gb300
SIZE=$(stat -c %s core_87000000)
echo ""
echo "========================================"
echo "=== BUILD & LINK SUCCESSFUL (GB300) ==="
echo "========================================"
echo "Output binary: d:\gb300\core_87000000 ($SIZE bytes)"
echo "Output binary: d:\gb300\j2me\core_87000000 ($SIZE bytes)"
echo "To deploy: Copy core_87000000 to SD:\cores\j2me\ or SD:\cores\psx\ on GB300"
