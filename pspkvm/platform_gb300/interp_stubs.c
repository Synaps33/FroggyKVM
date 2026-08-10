/*
 * interp_stubs.c - Symbol stubs for GB300 MIPS C-loop interpreter
 *
 * When using Interpreter_c.cpp (the actual C-loop interpreter) instead of
 * InterpreterSkeleton.cpp (empty stubs), some symbols that were defined in
 * InterpreterSkeleton.cpp are no longer available. This file provides them.
 *
 * Background: Interpreter_c.cpp contains the real bytecode dispatch loop
 * (primordial_to_current_thread, Interpret(), etc.). InterpreterSkeleton.cpp
 * contained only empty stubs that caused the JVM to jump to NULL/garbage
 * when executing bytecodes -> exception10 (Reserved Instruction) crash.
 */

#include <stddef.h>

/* These are defined in GlobalDefinitions.hpp but need storage.
 * Some are also in GlobalDefinitions_mips.cpp - we only provide what's missing. */

/* Used by AOT compiler - stub arrays */
int compiler_patched_code[1] = {0};
int compiler_unpatched_code[1] = {0};

/* Method execution sensor - used by CompiledMethodCache
 * Size must match method_execution_sensor_size in CompiledMethodCache.cpp
 * That value is 4096 on most platforms. */
#define METHOD_SENSOR_SIZE 4096
unsigned char _method_execution_sensor[METHOD_SENSOR_SIZE];

/* Current task ID as offset - used by isolates code */
int _current_task_id_as_offset = 0;

/* ROM constant pool fast access - ARM only, but linker needs the symbol */
int* _rom_constant_pool_fast = 0;

/* Global assembler_loop_type - Interpreter_c.cpp has it as a local struct member
 * for the non-MIPS path. For MIPS, we need the global. 
 * Value: has_Interpreter(1) | has_FloatingPoint(0x40) | has_TraceBytecodes(0x4) 
 * Since we have float support (ENABLE_FLOAT=1) and no trace:
 * = 1 | 0x40 = 0x41 */
int assembler_loop_type = 0x41;

/* Bytecode counter */
int _bytecode_counter = 0;

/* Protected page - used when ENABLE_PAGE_PROTECTION=1 which we don't have
 * but the linker may still want the symbol from GPSkeleton refs */
unsigned char _protected_page[1] = {0};
