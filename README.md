# FroggyKVM

J2ME / MIDP 2.0 emulator core for DataFrog SF2000 and GB300 handheld consoles, based on PSPKVM.

## Installation Guide

1. Copy the core binary for your device:
   - GB300: rename `core_87000000_gb300` to `core_87000000` and copy to `cores/j2me/core_87000000` on your SD card.
   - SF2000: rename `core_87000000_sf2000` to `core_87000000` and copy to `cores/j2me/core_87000000` on your SD card.
2. Copy `classes.zip` to `bios/classes.zip` on your SD card.
3. Place your J2ME `.jar` game files in your ROM folder.

## Controls

Default key mappings:
- D-Pad: 2 (Up), 8 (Down), 4 (Left), 6 (Right)
- A: 5 (OK / Action)
- B: 3
- X: 0
- Y: 1
- L: Soft 1 (Left softkey)
- R: Soft 2 (Right softkey)
- SELECT: * (Asterisk)
- START: # (Pound)

Combos:
- Hold SELECT + D-Pad: Directional arrows (Up, Down, Left, Right)
- Hold START + Y/X/B/A: 1, 3, 7, 9

Custom button mappings can be configured per game via `.cfg` files (see `default.cfg`).

## Building

Requires MIPS toolchain (`mips-mti-elf-gcc`). Run:
```bash
./build.sh
```
