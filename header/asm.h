#pragma once

#include "utils.h"
#include "tokenization.h"
#include "arrays.h"
// TODO: chick enumchick

static const int SET_LABEL  = 0x1abe1;
static const int PUSH       = 1;
static const int ADD        = 2;
static const int SUB        = 3;
static const int MUL        = 4;
static const int DIV        = 5;
static const int OUT        = 6;
static const int HLT        = 7;
static const int JUMP       = 8;
static const int ERROR      = -1;


typedef struct Label {
    Line name;
    int offset;

} Label;

define_array(Label);



typedef struct AsmData { // TODO: maybe asm_context?
    char** in_file_name;
    char** out_file_name;
    array(Label) labels;       // TODO: array?
    array(Token) tokens;       // TODO: array?

} AsmData;

status_t assemble(AsmData* data);