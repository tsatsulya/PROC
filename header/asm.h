#pragma once

#include "utils.h"
#include "tokenization.h"

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

    char** name;
    int offset;
    bool have_a_jump;

} Label;
typedef struct AsmData {

    char** in_file_name;
    char** out_file_name;
    Label* labels;
    size_t num_of_labels;
    Token* tokens;
    size_t num_of_tokens;

} AsmData;

status_t assemble(AsmData* data);