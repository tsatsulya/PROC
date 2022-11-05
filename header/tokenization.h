#pragma once

#include "arrays.h"
#include "utils.h"
#include "stdio.h"
#include "strings.h"

define_array(int);
define_array(long);


typedef enum TokenType {
    NUMBER    = 'N',
    COMMAND  = 'I', 
    LABEL = 'L', 
    ADDRESS = 'A'
} TokenType;


typedef struct Token {
    TokenType type;
    Line name;
    double number; 
} Token;
define_array(Token);


static const int code_string_max_len = 10000;
static const int input_format_max_len = 16; 
static const int words_in_line = 2;
static const int max_num_of_lines = 20;
static const int start_max_num_of_tokens = max_num_of_lines * 2;


status_t tokenize(array(Token)* token_sequence, const char* code_file_name, array(Line)* code_to_free);
