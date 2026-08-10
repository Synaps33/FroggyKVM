@echo off
setlocal enabledelayedexpansion

REM ========================================
REM QPSX SF2000 FrogUI Build Script
REM For use with sf2000_multicore (Trademarked69)
REM Platform: SF2000 (FROGGY_TYPE=SF2000)
REM ========================================

for %%I in (.) do set FOLDER_NAME=%%~nxI

echo ========================================
echo Building %FOLDER_NAME% - PSX Emulator for SF2000 FrogUI
echo Using Trademarked69/sf2000_multicore
echo ========================================

set MULTICORE_DIR=C:\Temp_FrogUI\sf2000_multicore
set QPSX_DIR=%~dp0

REM Convert Windows path to WSL path
set DRIVE_LETTER=%QPSX_DIR:~0,1%
for %%a in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do call set DRIVE_LETTER=%%DRIVE_LETTER:%%a=%%a%%
set QPSX_PATH_TAIL=%QPSX_DIR:~2,-1%
set QPSX_PATH_TAIL=%QPSX_PATH_TAIL:\=/%
set WSL_QPSX_PATH=/mnt/%DRIVE_LETTER%%QPSX_PATH_TAIL%
set WSL_MULTICORE=/mnt/c/Temp_FrogUI/sf2000_multicore

echo WSL Path: %WSL_QPSX_PATH%
echo Multicore: %MULTICORE_DIR%

echo.
echo [1/6] Cleaning previous build...
wsl -e bash -c "cd '%WSL_QPSX_PATH%' && find . -name '*.o' -delete 2>/dev/null; rm -f pcsx4all_libretro_sf2000.a core_87000000 2>/dev/null; true"

echo.
echo [2/6] Compiling QPSX library...
wsl -e bash -c "cd '%WSL_QPSX_PATH%' && export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:$PATH && make -f Makefile clean platform=sf2000 2>/dev/null; make -f Makefile platform=sf2000 -j4"

if %errorlevel% neq 0 (
    echo.
    echo *** COMPILATION FAILED ***
    pause
    exit /b 1
)

if not exist "%QPSX_DIR%pcsx4all_libretro_sf2000.a" (
    echo.
    echo *** ERROR: pcsx4all_libretro_sf2000.a not created! ***
    pause
    exit /b 1
)

echo.
echo [3/6] Building multicore support files...
wsl -e bash -c "cd '%WSL_MULTICORE%' && export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:$PATH && make -C libs/libretro-common 2>/dev/null || true"

echo.
echo [4/6] Copying multicore linker files...
copy /Y "%MULTICORE_DIR%\linker_scripts\core.ld" "%QPSX_DIR%" >nul
copy /Y "%MULTICORE_DIR%\linker_scripts\bisrv_08_03-core.ld" "%QPSX_DIR%" >nul
copy /Y "%MULTICORE_DIR%\libs\libretro-common\libretro-common.a" "%QPSX_DIR%" >nul 2>nul

echo.
echo [5/6] Linking core_87000000 (SF2000 FrogUI)...
wsl -e bash -c "cd '%WSL_QPSX_PATH%' && chmod +x link_multicore.sh && ./link_multicore.sh SF2000"

if %errorlevel% neq 0 (
    echo.
    echo *** LINKING FAILED ***
    pause
    exit /b 1
)

if not exist "%QPSX_DIR%core_87000000" (
    echo.
    echo *** ERROR: core_87000000 not created! ***
    pause
    exit /b 1
)

echo.
echo [6/6] Done!

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Platform: SF2000 FrogUI (Trademarked69 multicore)
echo FROGGY_TYPE: SF2000
echo.
echo Output: %QPSX_DIR%core_87000000
for %%A in ("%QPSX_DIR%core_87000000") do echo Size: %%~zA bytes
echo.
echo To deploy: copy core_87000000 to SD:\cores\psx\
echo.
pause
