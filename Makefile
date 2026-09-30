# J2ME (PSPKVM) libretro Makefile for GB300 / SF2000 Multicore
NAME    = j2me
O       = o
RM      = rm -f
.DEFAULT_GOAL := all

FLUIDLITE_REV = 1606f724b12ba20c33eca8aef3a16155a4227133
FLUIDLITE_URL = https://github.com/katyo/fluidlite/archive/$(FLUIDLITE_REV).tar.gz
FLUIDLITE_SHA256 = 83543805fddb8d5e9bc339b2be1accb38d316bd66b51722444cecc5bb894a7f4
FLUIDLITE_DIR = build/fluidlite-$(FLUIDLITE_REV)
FLUIDLITE_ARCHIVE = build/fluidlite-$(FLUIDLITE_REV).tar.gz
FLUIDLITE_STAMP = $(FLUIDLITE_DIR)/.ready
FLUIDLITE_OBJS = \
	$(FLUIDLITE_DIR)/src/fluid_chan.o \
	$(FLUIDLITE_DIR)/src/fluid_chorus.o \
	$(FLUIDLITE_DIR)/src/fluid_conv.o \
	$(FLUIDLITE_DIR)/src/fluid_defsfont.o \
	$(FLUIDLITE_DIR)/src/fluid_dsp_float.o \
	$(FLUIDLITE_DIR)/src/fluid_gen.o \
	$(FLUIDLITE_DIR)/src/fluid_hash.o \
	$(FLUIDLITE_DIR)/src/fluid_list.o \
	$(FLUIDLITE_DIR)/src/fluid_mod.o \
	$(FLUIDLITE_DIR)/src/fluid_rev.o \
	$(FLUIDLITE_DIR)/src/fluid_settings.o \
	$(FLUIDLITE_DIR)/src/fluid_synth.o \
	$(FLUIDLITE_DIR)/src/fluid_sys.o \
	$(FLUIDLITE_DIR)/src/fluid_tuning.o \
	$(FLUIDLITE_DIR)/src/fluid_voice.o

$(FLUIDLITE_STAMP):
	mkdir -p build
	curl -L --fail --retry 3 -o $(FLUIDLITE_ARCHIVE).tmp $(FLUIDLITE_URL)
	echo "$(FLUIDLITE_SHA256)  $(FLUIDLITE_ARCHIVE).tmp" | sha256sum -c -
	rm -rf $(FLUIDLITE_DIR) $(FLUIDLITE_DIR).src
	mkdir $(FLUIDLITE_DIR).src
	tar -xzf $(FLUIDLITE_ARCHIVE).tmp -C $(FLUIDLITE_DIR).src --strip-components=1
	mv $(FLUIDLITE_DIR).src $(FLUIDLITE_DIR)
	sed -i 's/^#define SF3_SUPPORT 1$$/#define SF3_SUPPORT 0/' $(FLUIDLITE_DIR)/src/fluid_config.h
	touch $(FLUIDLITE_STAMP)
	rm -f $(FLUIDLITE_ARCHIVE).tmp

ifeq ($(platform), sf2000)
    TARGET := $(NAME)_libretro_$(platform).a
    MIPS=/opt/mips32-mti-elf/2019.09-03-2/bin/mips-mti-elf-
    CC = $(MIPS)gcc
    CXX = $(MIPS)g++
    AR = $(MIPS)ar
    MIPS_FLAGS = -EL -march=mips32 -mtune=mips32r2 -msoft-float -ffast-math -G0 -mno-abicalls -fno-pic -ffreestanding -ffunction-sections -fdata-sections -DSF2000 -DNO_THREADS -DGB300 -DPSP_COMPAT -DPRODUCT=1
    override CFLAGS += $(MIPS_FLAGS)
    override CXXFLAGS += $(MIPS_FLAGS) -fno-use-cxa-atexit -fno-exceptions -fno-rtti
    STATIC_LINKING = 1
else ifeq ($(platform), sf3000)
    TARGET := $(NAME)_libretro.so
    MIPS_FLAGS = -EL -mips32r2 -march=mips32r2 -mtune=74kc -mdspr2 -mfp32 -mhard-float -mlong-calls -fPIC -ffunction-sections -fdata-sections -DSF3000 -DFROGGY_SD_ROOT=\"/mnt/sdcard\"
    override CFLAGS += $(MIPS_FLAGS)
    override CXXFLAGS += $(MIPS_FLAGS) -fno-use-cxa-atexit -fno-exceptions -fno-rtti
else
    TARGET = $(NAME)_libretro.so
    CC = gcc
    CXX = g++
    override CFLAGS += -m32 -O3 -fomit-frame-pointer -fPIC -Wno-incompatible-pointer-types -Wno-implicit-function-declaration
    override CXXFLAGS += -m32 -O3 -fomit-frame-pointer -fPIC -fpermissive
    LDFLAGS += -m32
    SHARED := -shared -Wl,--no-undefined
endif

all: $(TARGET)

MORE_CFLAGS = -O3 -fomit-frame-pointer -finline-functions -fno-strict-aliasing -DUSE_PRECOMPILED_HEADER=1 \
	-I. \
	-Ipspkvm/platform_gb300 \
	-I$(FLUIDLITE_DIR)/include \
	-I$(FLUIDLITE_DIR)/src \
	-Ilibretro/core \
	-Ipspkvm/javacall/interface \
	-Ipspkvm/javacall/interface/common \
	-Ipspkvm/javacall/interface/midp \
	-Ipspkvm/javacall/implementation/psp_mips \
	-Ipspkvm/javacall/implementation/psp_mips/common \
	-Ipspkvm/javacall/implementation/psp_mips/midp \
	-Ipspkvm/javacall/implementation/share/properties/inc \
	-Ipspkvm/cldc/src/vm/share \
	-Ipspkvm/cldc/src/vm/share/incls \
	-Ipspkvm/cldc/src/vm/os/javacall \
	-Ipspkvm/cldc/src/vm/os/utilities \
	-Ipspkvm/cldc/src/vm/cpu/mips \
	-Ipspkvm/cldc/src/vm/share/compiler \
	-Ipspkvm/cldc/src/vm/share/debugger \
	-Ipspkvm/cldc/src/vm/share/float \
	-Ipspkvm/cldc/src/vm/share/handles \
	-Ipspkvm/cldc/src/vm/share/interpreter \
	-Ipspkvm/cldc/src/vm/share/isolate \
	-Ipspkvm/cldc/src/vm/share/memory \
	-Ipspkvm/cldc/src/vm/share/memoryprofiler \
	-Ipspkvm/cldc/src/vm/share/natives \
	-Ipspkvm/cldc/src/vm/share/ROM \
	-Ipspkvm/cldc/src/vm/share/runtime \
	-Ipspkvm/cldc/src/vm/share/utilities \
	-Ipspkvm/cldc/src/vm/share/verifier \
	-Ipspkvm/cldc/src/vm/share/natives \
	-Ipspkvm/cldc/src/vm/cpu/mips \
	-Ipspkvm/cldc/src/vm/cpu/c \
	-Ipspkvm/cldc/src/vm/psp \
	-Ipspkvm/midp/src/ams \
	-Ipspkvm/midp/src/ams/ams_base_cldc/include \
	-Ipspkvm/midp/src/ams/example/javacall_common/include \
	-Ipspkvm/midp/src/ams/example/jams/include \
	-Ipspkvm/midp/src/ams/example/jams_port/include \
	-Ipspkvm/midp/src/ams/example/jams_port/javacall/native \
	-Ipspkvm/midp/src/ams/example/ams_common/include \
	-Ipspkvm/midp/src/ams/example/ams_common_port/include \
	-Ipspkvm/midp/src/events/eventqueue/include \
	-Ipspkvm/midp/src/events/eventqueue_port/include \
	-Ipspkvm/midp/src/events/eventsystem/include \
	-Ipspkvm/midp/src/events/mastermode_port/include \
	-Ipspkvm/midp/src/ams/ams_base/include \
	-Ipspkvm/midp/src/ams/suitestore/common_api/include \
	-Ipspkvm/midp/src/ams/suitestore/internal_api/include \
	-Ipspkvm/midp/src/ams/suitestore/task_manager_api/include \
	-Ipspkvm/midp/src/configuration/properties_port/include \
	-Ipspkvm/midp/src/core/vm_services/include \
	-Ipspkvm/midp/src/core/string/include \
	-Ipspkvm/midp/src/core/log_base/include \
	-Ipspkvm/midp/src/core/global_status/include \
	-Ipspkvm/midp/src/core/storage/include \
	-Ipspkvm/midp/src/core/memory/include \
	-Ipspkvm/midp/src/core/timezone/include \
	-Ipspkvm/midp/src/core/suspend_resume/sr_main/include \
	-Ipspkvm/midp/src/core/kni_util/include \
	-Ipspkvm/midp/src/i18n/i18n_main/include \
	-Ipspkvm/midp/src/highlevelui/javacall_application/include \
	-Ipspkvm/midp/src/highlevelui/lcdlf/include \
	-Ipspkvm/midp/src/highlevelui/lcdlf/lfjava/include \
	-Ipspkvm/midp/src/highlevelui/lfjport/include \
	-Ipspkvm/midp/src/push/push_server/include \
	-Ipspkvm/midp/src/lowlevelui/graphics_api/include \
	-Ipspkvm/midp/src/lowlevelui/graphics_api/gxapi_native/native \
	-Ipspkvm/midp/src/lowlevelui/graphics/include \
	-Ipspkvm/midp/src/lowlevelui/graphics/gx_putpixel/include \
	-Ipspkvm/midp/src/lowlevelui/putpixel_port/include \
	-Ipspkvm/midp/src/lowlevelui/image/include \
	-Ipspkvm/midp/src/lowlevelui/image_api/include \
	-Ipspkvm/midp/src/lowlevelui/image_decode/include \
	-Ipspkvm/midp/src/lowlevelui/image/img_putpixel/native \
	-Ipspkvm/midp/src/core/jarutil/include \
	-Ipspkvm/pcsl/memory \
	-Ipspkvm/pcsl/memory/memory_port \
	-Ipspkvm/pcsl/memory/memory_port/javacall \
	-Ipspkvm/pcsl/memory/heap \
	-Ipspkvm/pcsl/memory/pki/include \
	-Ipspkvm/pcsl/file \
	-Ipspkvm/pcsl/print \
	-Ipspkvm/pcsl/string \
	-Ipspkvm/pcsl/string/utf16 \
	-Ipspkvm/pcsl/string/util \
	-Ipspkvm/pcsl/types \
	-Ipspkvm/pcsl/types/javacall_psp_gcc \
	-Ipspkvm/pisces/src/native/midp/include \
	-Ipspkvm/pisces/src/native/common/include \
	-Ipspkvm/jpeg \
	-DENABLE_JPEG=1 \
	-fomit-frame-pointer \
	-Wno-unused -Wno-format -Wno-sign-compare \
	-D__LIBRETRO__ -DHAVE_LIBRETRO -DLC_CORE_STACK=LC_CORE -DLC_HIGHUI=LC_CORE -DLC_LOWUI=LC_CORE \
	-include pspkvm/platform_gb300/eventqueue_compat.h

OBJS = \
	pspkvm/platform_gb300/video.o \
	pspkvm/platform_gb300/audio.o \
	$(FLUIDLITE_OBJS) \
	pspkvm/platform_gb300/input.o \
	pspkvm/platform_gb300/filesystem.o \
	pspkvm/platform_gb300/timer.o \
	pspkvm/platform_gb300/puff.o \
	pspkvm/platform_gb300/gif_decode.o \
	pspkvm/platform_gb300/midlet_meta.o \
	pspkvm/platform_gb300/rms_posix.o \
	pspkvm/platform_gb300/platform.o \
	pspkvm/platform_gb300/vm_stubs.o \
	pspkvm/platform_gb300/vm_stubs_cpp.o \
	pspkvm/platform_gb300/skin_bin.o \
	pspkvm/platform_gb300/localized_strings.o \
	pspkvm/midp/src/highlevelui/lcdlf/lfjava/native/lfj_cskin.o \
	pspkvm/platform_gb300/Throw_override.o \
	pspkvm/platform_gb300/interp_stubs.o \
	pspkvm/platform_gb300/input_mode_kni.o \
	pspkvm/platform_gb300/native_weak_stubs.o \
	pspkvm/pcsl/string/util/utf.o \
	pspkvm/cldc/src/vm/share/natives/sni.o \
	pspkvm/javacall/implementation/psp_mips/common/events.o \
	pspkvm/midp/src/core/kni_util/reference/native/midpUtilKni.o \
	pspkvm/midp/src/core/kni_util/reference/native/kni_globals.o \
	pspkvm/midp/src/core/kni_util/reference/native/midpException.o \
	pspkvm/midp/src/core/vm_services/cldc_vm/native/midp_thread.o \
	pspkvm/midp/src/events/eventsystem/mastermode/native/midp_master_mode_events.o \
	pspkvm/midp/src/events/mastermode_port/javacall/native/midp_msgQueue_md.o \
	pspkvm/midp/src/events/eventqueue/reference/native/midpEvents.o \
	pspkvm/midp/src/events/eventqueue/reference/native/midpEventUtil.o \
	pspkvm/javacall/implementation/psp_mips/common/memory.o \
	pspkvm/javacall/implementation/psp_mips/common/logging.o \
	pspkvm/javacall/implementation/psp_mips/common/file.o \
	pspkvm/javacall/implementation/stubs/common/dir.o \
	pspkvm/cldc/src/vm/share/handles/JavaClass.o \
	pspkvm/cldc/src/vm/share/handles/FieldType.o \
	pspkvm/cldc/src/vm/share/handles/Signature.o \
	pspkvm/cldc/src/vm/share/handles/StackmapList.o \
	pspkvm/cldc/src/vm/share/handles/ClassInfo.o \
	pspkvm/cldc/src/vm/share/handles/ClassParserState.o \
	pspkvm/cldc/src/vm/share/memory/ClassInfoDesc.o \
	pspkvm/cldc/src/vm/share/memory/ClassParserStateDesc.o \
	pspkvm/cldc/src/vm/share/memory/StackmapListDesc.o \
	pspkvm/cldc/src/vm/share/memory/StackmapGenerator.o \
	pspkvm/cldc/src/vm/share/runtime/FileDecoder.o \
	pspkvm/cldc/src/vm/share/runtime/BufferedFile.o \
	pspkvm/cldc/src/vm/share/runtime/HotRoutines0.o \
	pspkvm/cldc/src/vm/share/runtime/HotRoutines1.o \
	pspkvm/cldc/src/vm/share/runtime/Scheduler.o \
	pspkvm/cldc/src/vm/share/compiler/BytecodeClosure.o \
	pspkvm/cldc/src/vm/share/interpreter/GPSkeleton.o \
	pspkvm/cldc/src/vm/share/utilities/CharacterStream.o \
	pspkvm/cldc/src/vm/share/utilities/ErrorMessage.o \
	pspkvm/cldc/src/vm/share/utilities/GlobalDefinitions.o \
	pspkvm/cldc/src/vm/cpu/mips/GlobalDefinitions_mips.o \
	pspkvm/javacall/implementation/psp_mips/midp/lcd.o \
	pspkvm/javacall/implementation/psp_mips/midp/input.o \
	pspkvm/javacall/implementation/psp_mips/midp/font.o \
	pspkvm/javacall/implementation/psp_mips/midp/image.o \
	pspkvm/javacall/implementation/psp_mips/midp/imageRom.o \
	pspkvm/javacall/implementation/psp_mips/midp/lifecycle.o \
	pspkvm/javacall/implementation/psp_mips/midp/keypress.o \
	pspkvm/javacall/implementation/psp_mips/midp/keymap.o \
	pspkvm/javacall/implementation/psp_mips/midp/time.o \
	pspkvm/javacall/implementation/psp_mips/midp/alpha_blend.o \
	pspkvm/javacall/implementation/psp_mips/midp/ft_support.o \
	pspkvm/midp/src/highlevelui/javacall_application/reference/native/javanotify_functions.o \
	pspkvm/midp/src/ams/example/javacall_common/reference/native/javaTask.o \
	pspkvm/midp/src/ams/example/jams/native/runMidlet.o \
	pspkvm/midp/src/ams/example/jams_port/javacall/native/runMidlet_md.o \
	pspkvm/midp/src/ams/ams_base/reference/native/midpInit.o \
	pspkvm/midp/src/ams/ams_base_cldc/reference/native/midpCommandState.o \
	pspkvm/midp/src/ams/ams_base_cldc/reference/native/midpMidletSuiteUtils.o \
	pspkvm/midp/src/ams/ams_base_cldc/reference/native/midp_run.o \
	pspkvm/midp/src/ams/ams_base/reference/native/midpInflate.o \
	pspkvm/midp/src/highlevelui/javacall_application/reference/native/jcapp_export.o \
	pspkvm/midp/src/highlevelui/lcdlf/lfjava/native/lfj_export.o \
	pspkvm/midp/src/highlevelui/lfjport/javacall/native/lfjport_jc_export.o \
	pspkvm/midp/src/highlevelui/lcdui/reference/native/lcdui_display.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_graphics.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_image.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_putpixel.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_screen_buffer.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_text.o \
	pspkvm/midp/src/lowlevelui/putpixel_port/javacall/native/gxjport_text.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_font_bitmap.o \
	pspkvm/midp/src/lowlevelui/graphics/gx_putpixel/native/gxj_graphics_asm.o \
	pspkvm/midp/src/lowlevelui/graphics_api/gxapi_native/native/gxapi_graphics_kni.o \
	pspkvm/midp/src/lowlevelui/graphics_api/gxapi_native/native/gxapi_font_kni.o \
	pspkvm/midp/src/lowlevelui/graphics_api/gxapi_native/native/gxapi_anchor.o \
	pspkvm/midp/src/lowlevelui/image/img_putpixel/native/imgj_imagedatafactory_kni.o \
	pspkvm/midp/src/lowlevelui/image/img_putpixel/native/imgj_imagedata_kni.o \
	pspkvm/midp/src/lowlevelui/image_decode/reference/native/imgdcd_png_decode.o \
	pspkvm/midp/src/lowlevelui/image_decode/reference/native/imgdcd_image.o \
	pspkvm/midp/src/lowlevelui/image_decode/reference/native/imgdcd_image_util.o \
	pspkvm/midp/src/lowlevelui/image_decode/reference/native/imgdcd_image_decode.o \
	pspkvm/jpeg/jcomapi.o \
	pspkvm/jpeg/jdapimin.o \
	pspkvm/jpeg/jdapistd.o \
	pspkvm/jpeg/jdcoefct.o \
	pspkvm/jpeg/jdcolor.o \
	pspkvm/jpeg/jddctmgr.o \
	pspkvm/jpeg/jdhuff.o \
	pspkvm/jpeg/jdinput.o \
	pspkvm/jpeg/jdmainct.o \
	pspkvm/jpeg/jdmarker.o \
	pspkvm/jpeg/jdmaster.o \
	pspkvm/jpeg/jdmerge.o \
	pspkvm/jpeg/jdphuff.o \
	pspkvm/jpeg/jdpostct.o \
	pspkvm/jpeg/jdsample.o \
	pspkvm/jpeg/jerror.o \
	pspkvm/jpeg/jidctfst.o \
	pspkvm/jpeg/jidctred.o \
	pspkvm/jpeg/jmemmgr.o \
	pspkvm/jpeg/jmemnobs.o \
	pspkvm/jpeg/jquant1.o \
	pspkvm/jpeg/jquant2.o \
	pspkvm/jpeg/jutils.o \
	pspkvm/jpeg/jpegdecoder.o \
	pspkvm/ext/nokia/src/native/nokia_ui_kni.o \
	pspkvm/cldc/src/vm/share/handles/JavaNear.o \
	pspkvm/cldc/src/vm/share/runtime/IsolateObj.o \
	pspkvm/pisces/src/native/common/src/PiscesBlit.o \
	pspkvm/pisces/src/native/common/src/PiscesLibrary.o \
	pspkvm/pisces/src/native/common/src/PiscesMath.o \
	pspkvm/pisces/src/native/common/src/PiscesPipelines.o \
	pspkvm/pisces/src/native/common/src/PiscesRenderer.o \
	pspkvm/pisces/src/native/common/src/PiscesTransform.o \
	pspkvm/pisces/src/native/common/src/PiscesUtil.o \
	pspkvm/pisces/src/native/midp/src/JAbstractSurface.o \
	pspkvm/pisces/src/native/midp/src/JGraphicsSurfaceDestination.o \
	pspkvm/pisces/src/native/midp/src/JJavaSurface.o \
	pspkvm/pisces/src/native/midp/src/JNativeFinalizer.o \
	pspkvm/pisces/src/native/midp/src/JNativeSurface.o \
	pspkvm/pisces/src/native/midp/src/JPiscesFinalizer.o \
	pspkvm/pisces/src/native/midp/src/JPiscesRenderer.o \
	pspkvm/pisces/src/native/midp/src/JTransform.o \
	pspkvm/pisces/src/native/midp/src/KNIUtil.o \
	pspkvm/pisces/src/native/midp/src/PiscesSysutils.o \
	pspkvm/pcsl/string/utf16/pcsl_string.o \
	pspkvm/pcsl/memory/memory_port/javacall/pcsl_memory_port.o \
	pspkvm/pcsl/file/javacall/pcsl_file.o \
	pspkvm/pcsl/file/javacall/pcsl_dir.o \
	pspkvm/pcsl/print/javacall/pcsl_print.o \
	pspkvm/cldc/src/vm/os/javacall/JVM_javacall.o \
	pspkvm/cldc/src/vm/os/javacall/OS_javacall.o \
	pspkvm/cldc/src/vm/os/javacall/OsMisc_javacall.o \
	pspkvm/cldc/src/vm/os/javacall/OsFile_javacall.o \
	pspkvm/cldc/src/vm/os/javacall/OsMemory_javacall.o \
	pspkvm/cldc/src/vm/share/runtime/JVM.o \
	pspkvm/cldc/src/vm/share/runtime/OS.o \
	pspkvm/cldc/src/vm/share/runtime/OsMemory.o \
	pspkvm/cldc/src/vm/share/runtime/OsFile.o \
	pspkvm/cldc/src/vm/share/runtime/TaskContext.o \
	pspkvm/cldc/src/vm/share/runtime/Throwable.o \
	pspkvm/cldc/src/vm/share/runtime/Synchronizer.o \
	pspkvm/cldc/src/vm/share/runtime/JavaVTable.o \
	pspkvm/cldc/src/vm/share/runtime/Field.o \
	pspkvm/cldc/src/vm/share/runtime/FilePath.o \
	pspkvm/cldc/src/vm/share/runtime/Frame.o \
	pspkvm/cldc/src/vm/share/runtime/JarFileParser.o \
	pspkvm/cldc/src/vm/share/runtime/ClassFileParser.o \
	pspkvm/cldc/src/vm/share/runtime/ClassPathAccess.o \
	pspkvm/cldc/src/vm/share/runtime/Inflate.o \
	pspkvm/cldc/src/vm/share/runtime/SystemDictionary.o \
	pspkvm/cldc/src/vm/share/runtime/Task.o \
	pspkvm/cldc/src/vm/share/runtime/Thread.o \
	pspkvm/cldc/src/vm/share/handles/Universe.o \
	pspkvm/cldc/src/vm/share/handles/SymbolTable.o \
	pspkvm/cldc/src/vm/share/handles/StringTable.o \
	pspkvm/cldc/src/vm/share/handles/InstanceClass.o \
	pspkvm/cldc/src/vm/share/handles/ArrayClass.o \
	pspkvm/cldc/src/vm/share/handles/ObjArrayClass.o \
	pspkvm/cldc/src/vm/share/handles/TypeArrayClass.o \
	pspkvm/cldc/src/vm/share/handles/Method.o \
	pspkvm/cldc/src/vm/share/handles/ConstantPool.o \
	pspkvm/cldc/src/vm/share/handles/ExecutionStack.o \
	pspkvm/cldc/src/vm/share/handles/Symbol.o \
	pspkvm/cldc/src/vm/share/handles/Symbols.o \
	pspkvm/cldc/src/vm/share/handles/TypeSymbol.o \
	pspkvm/cldc/src/vm/share/handles/Instance.o \
	pspkvm/cldc/src/vm/share/handles/Array.o \
	pspkvm/cldc/src/vm/share/handles/ObjArray.o \
	pspkvm/cldc/src/vm/share/handles/TypeArray.o \
	pspkvm/cldc/src/vm/share/handles/String.o \
	pspkvm/cldc/src/vm/share/handles/TaskMirror.o \
	pspkvm/cldc/src/vm/share/handles/ThreadObj.o \
	pspkvm/cldc/src/vm/share/handles/Oop.o \
	pspkvm/cldc/src/vm/share/handles/RefArray.o \
	pspkvm/cldc/src/vm/share/memory/OopDesc.o \
	pspkvm/cldc/src/vm/share/memory/FarClassDesc.o \
	pspkvm/cldc/src/vm/share/memory/MethodDesc.o \
	pspkvm/cldc/src/vm/share/memory/ExecutionStackDesc.o \
	pspkvm/cldc/src/vm/share/memory/FinalizerConsDesc.o \
	pspkvm/cldc/src/vm/share/memory/SymbolDesc.o \
	pspkvm/cldc/src/vm/share/memory/ObjectHeap.o \
	pspkvm/cldc/src/vm/share/memory/Allocation.o \
	pspkvm/cldc/src/vm/share/ROM/ROM.o \
	pspkvm/cldc/src/vm/share/ROM/ROMSkeleton.o \
	pspkvm/cldc/src/vm/share/interpreter/OopMapsSkeleton.o \
	pspkvm/cldc/src/vm/share/utilities/Arguments.o \
	pspkvm/cldc/src/vm/share/utilities/AccessFlags.o \
	pspkvm/cldc/src/vm/share/utilities/ConstantTag.o \
	pspkvm/cldc/src/vm/share/utilities/Debug.o \
	pspkvm/cldc/src/vm/share/utilities/Globals.o \
	pspkvm/cldc/src/vm/share/utilities/Stream.o \
	pspkvm/cldc/src/vm/share/verifier/Verifier.o \
	pspkvm/cldc/src/vm/share/verifier/VerifierFrame.o \
	pspkvm/cldc/src/vm/share/verifier/VerifyMethodCodes.o \
	pspkvm/cldc/src/vm/share/interpreter/InterpreterRuntime.o \
	pspkvm/cldc/src/vm/share/interpreter/TemplateTable.o \
	pspkvm/cldc/src/vm/share/natives/kni.o \
	pspkvm/cldc/src/vm/share/natives/KniUncommon.o \
	pspkvm/cldc/src/vm/share/natives/Natives.o \
	pspkvm/cldc/src/vm/share/natives/NativesTable.o \
	pspkvm/cldc/src/vm/cpu/c/Interpreter_c.o \
	pspkvm/cldc/src/vm/share/interpreter/Bytecodes.o \
	pspkvm/cldc/src/vm/cpu/c/FloatSupport_c.o \
	pspkvm/cldc/src/vm/cpu/c/Frame_c.o \
	pspkvm/cldc/src/vm/share/float/FloatNatives.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_sin.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_cos.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_tan.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_asin.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_acos.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_atan.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_atan2.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_ceil.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_floor.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_fabs.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_copysign.o \
	pspkvm/cldc/src/vm/share/float/JFP_lib_scalbn.o \
	pspkvm/cldc/src/vm/share/float/IEEE754_sqrt.o \
	pspkvm/cldc/src/vm/share/float/IEEE754_fmod.o \
	pspkvm/cldc/src/vm/share/float/IEEE754_rem_pio2.o \
	pspkvm/cldc/src/vm/share/float/Remainder_pio2_kernel.o \
	pspkvm/cldc/src/vm/share/float/Sine_kernel.o \
	pspkvm/cldc/src/vm/share/float/Cosine_kernel.o \
	pspkvm/cldc/src/vm/share/float/Tangent_kernel.o

$(FLUIDLITE_OBJS): $(FLUIDLITE_STAMP)

$(TARGET): $(OBJS) $(FLUIDLITE_OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJS)
else
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)
endif

clean:
	$(RM) $(TARGET) $(OBJS)

override CFLAGS += $(MORE_CFLAGS)
override CXXFLAGS += $(MORE_CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(FLUIDLITE_OBJS): $(FLUIDLITE_STAMP)
$(OBJS): $(FLUIDLITE_STAMP)

.PHONY: all clean
