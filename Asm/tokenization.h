#include "utils.h"
#include "stdio.h"


const size_t NAME_SIZE = 16;

typedef enum TokenType {
    NUMBER    = 'N',
    IDENT_TYPE  = 'I',
    UNDF_TYPE = 'U'
}TokenType;

typedef struct Token {
    TokenType type;
    char name[NAME_SIZE];
    int number; 
}Token;

const Token POISON = {'U', "UNDEFINED", 0};


status tokenize(Token** tokens, size_t* num_of_tokens, const char* const code);

int get_sequence( char *dest, const char* const code, const  input_formats_first_id );
