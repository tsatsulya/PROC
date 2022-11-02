#pragma once

#include "utils.h"
#include "tokenization.h"

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
    char** name;
    int offset;

} Label;



// #define array(type) array_##type

// #define define_array(type) /
//     struct array(type) {   /
//         type* buffer;      /
//         size_t length;     /
//     };

// define_array(int)

// void foo() {

//     array(int) new_array = create_array(int, 10);
// }


// struct label_array; //

typedef struct AsmData { // TODO: maybe asm_context?
    char** in_file_name;
    char** out_file_name;
    Label* labels;       // TODO: array?
    size_t num_of_labels;
    Token* tokens;       // TODO: array?
    size_t num_of_tokens;

} AsmData;

status_t assemble(AsmData* data);