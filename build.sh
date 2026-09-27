#!/bin/bash
# FroggyKVM GB300 / SF2000 Build & Link Script
set -e

export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:/usr/bin:/bin:$PATH

echo "=================================================="
echo "=== Building FroggyKVM for GB300 and SF2000 ==="
echo "=================================================="
echo ""

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

# 1. Build core static library
echo "[1/4] Compiling FroggyKVM static library..."
make platform=sf2000 clean 2>/dev/null || true
make platform=sf2000 -j$(nproc)

if [ ! -f "j2me_libretro_sf2000.a" ]; then
    echo "ERROR: j2me_libretro_sf2000.a not found!"
    exit 1
fi

# 2. Detect Multicore directory
if [ -d "/mnt/c/Temp/gb300_multicore" ]; then
    MULTICORE="/mnt/c/Temp/gb300_multicore"
elif [ -d "/home/Sajnaps/gb300/sf2000_multicore" ]; then
    MULTICORE="/home/Sajnaps/gb300/sf2000_multicore"
else
    echo "ERROR: Multicore directory not found!"
    exit 1
fi

mkdir -p "${MULTICORE}/cores/j2me"
cp j2me_libretro_sf2000.a "${MULTICORE}/cores/j2me/"

cat << 'EOF' > "${MULTICORE}/cores/j2me/Makefile"
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

# 3. Build GB300 core
echo "[2/4] Linking core for GB300 (FROGGY_TYPE=GB300V2)..."
cd "${MULTICORE}"
rm -rf build 2>/dev/null || true
make FROGGY_TYPE=GB300V2 CORE=cores/j2me CONSOLE=j2me MIPS=/opt/mips32-mti-elf/2019.09-03-2/bin/mips-mti-elf- "${MULTICORE}/build/core_87000000"

cp "${MULTICORE}/build/core_87000000" "${SCRIPT_DIR}/core_87000000_gb300"
cp "${MULTICORE}/build/core_87000000" "${SCRIPT_DIR}/core_87000000"

# 4. Build SF2000 core
echo "[3/4] Linking core for SF2000 (FROGGY_TYPE=SF2000)..."
rm -rf build 2>/dev/null || true
make FROGGY_TYPE=SF2000 CORE=cores/j2me CONSOLE=j2me MIPS=/opt/mips32-mti-elf/2019.09-03-2/bin/mips-mti-elf- "${MULTICORE}/build/core_87000000"

cp "${MULTICORE}/build/core_87000000" "${SCRIPT_DIR}/core_87000000_sf2000"

cd "${SCRIPT_DIR}"

# 5. Prepare Release Packages
echo "[4/4] Assembling release packages in /home/Sajnaps/gb300/release/..."
RELEASE_DIR="/home/Sajnaps/gb300/release"

# GB300 Package
mkdir -p "${RELEASE_DIR}/gb300/cores/j2me"
mkdir -p "${RELEASE_DIR}/gb300/bios"
mkdir -p "${RELEASE_DIR}/gb300/config/j2me"
cp "${SCRIPT_DIR}/core_87000000_gb300" "${RELEASE_DIR}/gb300/cores/j2me/core_87000000"
cp "${SCRIPT_DIR}/classes.zip" "${RELEASE_DIR}/gb300/bios/classes.zip"
if [ -f "${SCRIPT_DIR}/default.cfg" ]; then
    cp "${SCRIPT_DIR}/default.cfg" "${RELEASE_DIR}/gb300/config/j2me/default.cfg"
elif [ -f "/media/Sajnaps/GB300/ROMS/j2me/keymaps/default.cfg" ]; then
    cp "/media/Sajnaps/GB300/ROMS/j2me/keymaps/default.cfg" "${RELEASE_DIR}/gb300/config/j2me/default.cfg"
fi

# SF2000 Package
mkdir -p "${RELEASE_DIR}/sf2000/cores/j2me"
mkdir -p "${RELEASE_DIR}/sf2000/bios"
mkdir -p "${RELEASE_DIR}/sf2000/config/j2me"
cp "${SCRIPT_DIR}/core_87000000_sf2000" "${RELEASE_DIR}/sf2000/cores/j2me/core_87000000"
cp "${SCRIPT_DIR}/classes.zip" "${RELEASE_DIR}/sf2000/bios/classes.zip"
if [ -f "${SCRIPT_DIR}/default.cfg" ]; then
    cp "${SCRIPT_DIR}/default.cfg" "${RELEASE_DIR}/sf2000/config/j2me/default.cfg"
elif [ -f "/media/Sajnaps/GB300/ROMS/j2me/keymaps/default.cfg" ]; then
    cp "/media/Sajnaps/GB300/ROMS/j2me/keymaps/default.cfg" "${RELEASE_DIR}/sf2000/config/j2me/default.cfg"
fi

# Central bios directory
mkdir -p /home/Sajnaps/gb300/bios
cp "${SCRIPT_DIR}/classes.zip" /home/Sajnaps/gb300/bios/classes.zip

# Optional copies if SD card is mounted
if [ -d "/mnt/d/gb300" ]; then
    cp "${SCRIPT_DIR}/core_87000000_gb300" /mnt/d/gb300/core_87000000
    mkdir -p /mnt/d/gb300/cores/j2me /mnt/d/gb300/bios /mnt/d/gb300/config/j2me
    cp "${SCRIPT_DIR}/core_87000000_gb300" /mnt/d/gb300/cores/j2me/core_87000000
    cp "${SCRIPT_DIR}/classes.zip" /mnt/d/gb300/bios/classes.zip
    if [ -f "${RELEASE_DIR}/gb300/config/j2me/default.cfg" ]; then
        cp "${RELEASE_DIR}/gb300/config/j2me/default.cfg" /mnt/d/gb300/config/j2me/default.cfg
    fi
fi
if [ -d "/media/Sajnaps/GB300" ]; then
    echo "Copying to mounted SD card at /media/Sajnaps/GB300..."
    mkdir -p /media/Sajnaps/GB300/cores/j2me /media/Sajnaps/GB300/bios /media/Sajnaps/GB300/config/j2me /media/Sajnaps/GB300/configs/j2me
    cp "${SCRIPT_DIR}/core_87000000_gb300" /media/Sajnaps/GB300/cores/j2me/core_87000000
    if [ -d "/media/Sajnaps/GB300/cores/bounce" ]; then
        cp "${SCRIPT_DIR}/core_87000000_gb300" /media/Sajnaps/GB300/cores/bounce/core_87000000
    fi
    cp "${SCRIPT_DIR}/classes.zip" /media/Sajnaps/GB300/bios/classes.zip
    if [ -f "${RELEASE_DIR}/gb300/config/j2me/default.cfg" ]; then
        cp "${RELEASE_DIR}/gb300/config/j2me/default.cfg" /media/Sajnaps/GB300/config/j2me/default.cfg
        cp "${RELEASE_DIR}/gb300/config/j2me/default.cfg" /media/Sajnaps/GB300/configs/j2me/default.cfg
    fi
    sync
fi

GB_SIZE=$(stat -c %s "${SCRIPT_DIR}/core_87000000_gb300")
SF_SIZE=$(stat -c %s "${SCRIPT_DIR}/core_87000000_sf2000")

echo ""
echo "=================================================="
echo "=== BUILD SUCCESSFUL: GB300 & SF2000 ==="
echo "=================================================="
echo "GB300 binary : ${SCRIPT_DIR}/core_87000000_gb300 ($GB_SIZE bytes)"
echo "SF2000 binary: ${SCRIPT_DIR}/core_87000000_sf2000 ($SF_SIZE bytes)"
echo "Classes ZIP  : /home/Sajnaps/gb300/bios/classes.zip"
echo ""
echo "Release packages ready:"
echo "  - GB300 : ${RELEASE_DIR}/gb300/ (copy 'cores' and 'bios' to SD root)"
echo "  - SF2000: ${RELEASE_DIR}/sf2000/ (copy 'cores' and 'bios' to SD root)"
