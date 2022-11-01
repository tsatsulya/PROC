#pragma once

#include "utils.h"
#include "stdio.h"

typedef enum TokenType {
    NUMBER    = 'N',
    IDENT_COMMAND  = 'I',
    UNDF_TYPE = 'U',
    LABEL = 'L', 
    JUMP_TO = 'J'
} TokenType;

typedef struct Token {
    TokenType type;
    char** name;
    int number; 
} Token;

static const Token POISON = {UNDF_TYPE, NULL, 0};


static const int code_string_max_len = 32;
static const int input_format_max_len = 16; 
static const int words_in_line = 2;
static const int max_num_of_lines = 20;
static const int start_max_num_of_tokens = max_num_of_lines * 2;

status_t tokenize(Token** tokens, int* num_of_tokens, const char* const code_file_name);

