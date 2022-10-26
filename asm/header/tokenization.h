#pragma once

#include "utils.h"
#include "stdio.h"



typedef enum TokenType {
    NUMBER    = 'N',
    IDENT_COMMAND  = 'I',
    UNDF_TYPE = 'U'
}TokenType;

typedef struct Token {
    TokenType type;
    char* name;
    int number; 
}Token;

static const Token POISON = {UNDF_TYPE, "UNDEFINED", 0};


status tokenize(Token** tokens, int* num_of_tokens, const char* const code_file_name);

//int get_sequence( char *dest, const char* const code, const  input_formats_first_id );
