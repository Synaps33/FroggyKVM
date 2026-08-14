@echo off
setlocal enabledelayedexpansion

REM ========================================
REM FroggyKVM GB300 Build Script (Multicore Official)
REM Uses: C:\Temp\gb300_multicore
REM ========================================

for %%I in (.) do set FOLDER_NAME=%%~nxI

echo ========================================
echo Building FroggyKVM (J2ME) for GB300
echo Using gb300_multicore
echo ========================================

set MULTICORE_DIR=C:\Temp\gb300_multicore
set FROGGY_DIR=%~dp0
set CORE_NAME=j2me

REM Convert Windows path to WSL path
set DRIVE_LETTER=%FROGGY_DIR:~0,1%
for %%a in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do call set DRIVE_LETTER=%%DRIVE_LETTER:%%a=%%a%%
set FROGGY_PATH_TAIL=%FROGGY_DIR:~2,-1%
set FROGGY_PATH_TAIL=%FROGGY_PATH_TAIL:\=/%
set WSL_FROGGY_PATH=/mnt/%DRIVE_LETTER%%FROGGY_PATH_TAIL%

echo WSL Path: %WSL_FROGGY_PATH%
echo Core name: %CORE_NAME%

echo.
echo [1/5] Cleaning previous build...
wsl -e bash -c "cd '%WSL_FROGGY_PATH%' && make clean platform=sf2000 2>/dev/null; true"

echo.
echo [2/5] Compiling FroggyKVM library...
wsl -e bash -c "cd '%WSL_FROGGY_PATH%' && export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:/usr/bin:/bin:$PATH && make platform=sf2000 -j4"

if %errorlevel% neq 0 (
    echo.
    echo *** COMPILATION FAILED ***
    pause
    exit /b 1
)

if not exist "%FROGGY_DIR%j2me_libretro_sf2000.a" (
    echo.
    echo *** ERROR: j2me_libretro_sf2000.a not created! ***
    pause
    exit /b 1
)

echo.
echo [3/5] Setting up GB300 multicore framework...
wsl -e bash -c "mkdir -p /mnt/c/Temp/gb300_multicore/cores/%CORE_NAME%"

REM Create the core Makefile
echo TARGET_NAME := j2me> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo.>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo ifeq ($(platform), sf2000)>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo 	TARGET := $(TARGET_NAME)_libretro_$(platform).a>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo 	STATIC_LINKING = 1>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo endif>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo.>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo all:>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo 	@echo "Using pre-built $(TARGET)">> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo 	@test -f $(TARGET) ^|^| (echo "ERROR: $(TARGET) not found!" ^&^& exit 1)>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo.>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo clean:>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo 	@echo "Nothing to clean">> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo.>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"
echo .PHONY: all clean>> "%MULTICORE_DIR%\cores\%CORE_NAME%\Makefile"

REM Copy the .a file
copy /Y "%FROGGY_DIR%j2me_libretro_sf2000.a" "%MULTICORE_DIR%\cores\%CORE_NAME%\"

echo.
echo [4/5] Linking core_87000000 for GB300...
wsl -e bash -c "cd /mnt/c/Temp/gb300_multicore && rm -f core_87000000 core.elf libretro_core.a 2>/dev/null; export PATH=/opt/mips32-mti-elf/2019.09-03-2/bin:/usr/bin:/bin:$PATH && make CORE=cores/%CORE_NAME% CONSOLE=j2me core_87000000"

if %errorlevel% neq 0 (
    echo.
    echo *** LINKING FAILED ***
    pause
    exit /b 1
)

if not exist "%MULTICORE_DIR%\core_87000000" (
    echo.
    echo *** ERROR: core_87000000 not created! ***
    pause
    exit /b 1
)

echo.
echo [5/5] Copying result...
copy /Y "%MULTICORE_DIR%\core_87000000" "%FROGGY_DIR%core_87000000"
if not exist "%FROGGY_DIR%j2me" mkdir "%FROGGY_DIR%j2me"
copy /Y "%MULTICORE_DIR%\core_87000000" "%FROGGY_DIR%j2me\core_87000000"

echo.
echo ========================================
echo BUILD SUCCESSFUL! (GB300)
echo ========================================
echo.
echo Output: %FROGGY_DIR%core_87000000
echo Output: %FROGGY_DIR%j2me\core_87000000
for %%A in ("%FROGGY_DIR%core_87000000") do echo Size: %%~zA bytes
echo.
echo To deploy: copy core_87000000 to SD:\cores\j2me\ or SD:\cores\psx\ on GB300
echo.
pause
