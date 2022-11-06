#pragma once

#include "utils.h"
#include "tokenization.h"
#include "arrays.h"
// TODO: chick enumchick



enum {
    SET_LABEL  = 0x1abe1,
    PUSH       = 1,
    ADD        = 2,
    SUB        = 3,
    MUL        = 4,
    DIV        = 5,
    OUT        = 6,
    HLT        = 7,
    JUMP       = 8,
    ERROR      = -1,
    PUSHA      = 9,
    POP        = 10,
    PRINT      = 11,
    SCAN       = 12,
    IN         = 13,
    JB         = 14,
    JE         = 15,
    DUP        = 16,
    POW        = 17,
    SQRT       = 18,
    JA = 19,
    INC = 20,
    PUTC = 21,
    JAE = 22,
    ABS = 23,
};




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