#pragma once

#include "arrays.h"
#include "utils.h"
#include "stdio.h"
#include "strings.h"


typedef enum TokenType {
    NUMBER    = 'N',
    COMMAND  = 'I', // TODO: I would list all possible commands
    LABEL = 'L', 
    JUMP_TO = 'J' // TODO: I wouldn't extract jump in different file
} TokenType;


typedef struct Token {
    TokenType type;
    Line name;
    int number; 
} Token;
define_array(Token);


static const int code_string_max_len = 32;
static const int input_format_max_len = 16; 
static const int words_in_line = 2;
static const int max_num_of_lines = 20;
static const int start_max_num_of_tokens = max_num_of_lines * 2;


status_t tokenize(array(Token)* token_sequence, const char* code_file_name);
