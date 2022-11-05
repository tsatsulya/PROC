#pragma once

#include "utils.h"
#include "asm.h"
#include "stack.h" 
typedef struct Processor {

    array(double) code;

    Stack* cpu_stack;
    double RAM[0x10000];

} Processor;


status_t cpu_exec(Processor* cpu);
status_t cpu_load(Processor* cpu, const char* in_file_name);
