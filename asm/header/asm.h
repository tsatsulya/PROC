#pragma once

#include "utils.h"
#include "tokenization.h"

#define PUSH 1
#define ADD  2
#define SUB  3
#define MUL  4
#define DIV  5
#define OUT  6
#define HLT  7
#define ERROR -1

typedef struct AsmData{
    char* in_file_name;
    char* out_file_name;

    /* parsed tokens */
    Token* tokens;
    size_t num_of_tokens;
} AsmData;

status_t assemble(AsmData* data);