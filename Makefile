# J2ME (PSPKVM) libretro Makefile for GB300 / SF2000 Multicore
NAME    = j2me
O       = o
RM      = rm -f

ifeq ($(platform), sf2000)
    TARGET := $(NAME)_libretro_$(platform).a
    MIPS=/opt/mips32-mti-elf/2019.09-03-2/bin/mips-mti-elf-
    CC = $(MIPS)gcc
    CXX = $(MIPS)g++
    AR = $(MIPS)ar
    CFLAGS += -EL -march=mips32 -mtune=mips32 -msoft-float -ffast-math
    CFLAGS += -G0 -mno-abicalls -fno-pic -ffreestanding
    CFLAGS += -ffunction-sections -fdata-sections
    CFLAGS += -DSF2000 -DNO_THREADS -DGB300 -DPSP_COMPAT
    CXXFLAGS += -fno-use-cxa-atexit -fno-exceptions -fno-rtti
    STATIC_LINKING = 1
else
    TARGET = $(NAME)_libretro.so
    CC = gcc
    CXX = g++
endif

all: $(TARGET)

MORE_CFLAGS = -O2 -fno-strict-aliasing -DUSE_PRECOMPILED_HEADER=1 \
	-I. \
	-Ipspkvm/platform_gb300 \
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
	-Ipspkvm/midp/src/push/push_server/include \
	-Ipspkvm/midp/src/lowlevelui/graphics_api/include \
	-Ipspkvm/midp/src/lowlevelui/graphics/include \
	-Ipspkvm/midp/src/lowlevelui/image_api/include \
	-Ipspkvm/pcsl/memory \
	-Ipspkvm/pcsl/memory/memory_port \
	-Ipspkvm/pcsl/memory/memory_port/javacall \
	-Ipspkvm/pcsl/memory/heap \
	-Ipspkvm/pcsl/memory/pki/include \
	-Ipspkvm/pcsl/file \
	-Ipspkvm/pcsl/print \
	-Ipspkvm/pcsl/string \
	-Ipspkvm/pcsl/string/utf16 \
	-Ipspkvm/pcsl/types \
	-Ipspkvm/pcsl/types/javacall_psp_gcc \
	-Ipspkvm/pisces/src/native/midp/include \
	-Ipspkvm/pisces/src/native/common/include \
	-fomit-frame-pointer \
	-Wno-unused -Wno-format -Wno-sign-compare \
	-D__LIBRETRO__ -DHAVE_LIBRETRO -DLC_CORE_STACK=LC_CORE

CFLAGS  += $(MORE_CFLAGS)
CXXFLAGS = $(CFLAGS)

OBJS = \
	pspkvm/platform_gb300/video.o \
	pspkvm/platform_gb300/audio.o \
	pspkvm/platform_gb300/input.o \
	pspkvm/platform_gb300/filesystem.o \
	pspkvm/platform_gb300/timer.o \
	pspkvm/platform_gb300/platform.o \
	pspkvm/platform_gb300/vm_stubs.o \
	pspkvm/platform_gb300/vm_stubs_cpp.o \
	pspkvm/platform_gb300/Throw_override.o \
	pspkvm/cldc/src/vm/share/runtime/ClassFileParser.o \
	pspkvm/cldc/src/vm/share/handles/ConstantPool.o \
	pspkvm/platform_gb300/interp_stubs.o \
	pspkvm/javacall/implementation/psp_mips/common/events.o \
	pspkvm/javacall/implementation/psp_mips/common/memory.o \
	pspkvm/javacall/implementation/psp_mips/common/logging.o \
	pspkvm/javacall/implementation/psp_mips/common/file.o \
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
	pspkvm/midp/src/ams/ams_base_cldc/reference/native/midp_run.o \
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
	pspkvm/cldc/src/vm/share/runtime/Throw.o \
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
	pspkvm/cldc/src/vm/share/natives/Natives.o \
	pspkvm/cldc/src/vm/cpu/c/Interpreter_c.o \
	pspkvm/cldc/src/vm/share/interpreter/Bytecodes.o \
	pspkvm/cldc/src/vm/cpu/c/FloatSupport_c.o

$(TARGET): $(OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJS)
else
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)
endif

clean:
	$(RM) $(TARGET) $(OBJS)

CFLAGS += $(MORE_CFLAGS)
CXXFLAGS += $(CFLAGS) $(MORE_CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean
