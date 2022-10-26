#include "tokenization.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const int code_string_max_len = 32;
const int input_format_max_len = 16; 
const int words_in_line = 2;
const int max_num_of_lines = 20;
const int start_max_num_of_tokens = max_num_of_lines * 2;


#define ERROR(COND, TO_RET)                                     \
    if (COND) {                                                 \
        return TO_RET;                                          \
    }
        // if (TO_FREE1)                                           
        //     free(TO_FREE1);                                     
        // if (TO_FREE2)                                           
        //     free(TO_FREE2);                                     
        //     




static long long int pow_(int x, int power) {
    long long int result = 1;
    for (int y = 0; y < power; y++) {
        result*=x;
    }
    return result;
}


static int get_word(char** word_, int* word_length, int start, const char *code) {   

    for (; *(code + start) == ' '; start++) {}

    int shift = start;
    char* word = (char*) calloc(sizeof(char), input_format_max_len);

    for (;; shift++) {
        if (code[shift] == ' ' || code[shift] == '\n' || code[shift] == '\0') {
            break;
        }
        else {
            word[shift-start] = *(code + shift);
        }
    }
    puts(word);
   
    *word_length = shift - start;
    *word_ = word;

    return shift++;
}

static int str_to_int(const char* string) {

    int number = 0;
    int length = (int) strlen(string); //<- AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    for (int i = 0; i < length; i++) {
        int n = string[i];
        number += (n - 48) * (int)pow_(10, length - i-1);
    }
    return number;
}

static int string_is_number(const char* string) {
    int length = (int) strlen(string); 
    for (int i = 0; i < length; i++) {
        int n = string[i];
        if ( n < 48 || 57 < n)
            return false;
    }
    return true;
}


static bool is_command(char* string) {
    for (int i = 0; i < num_of_commands; i++) {
        if (strcmp(string, commands[i]) == 0) 
            return true; 
    }
    return false;
}

static status check_command(char** words, int word_count) {

    if (word_count > 2) return INCORRECT_INPUT;

    if (is_command(words[0])) {
        bool is_push = !strcmp(words[0], "push");
        bool args = (word_count == 1) ? false : string_is_number(words[1]);

        if (!(is_push ^ args)) { 
            return OK;
        }
        else 
            return INCORRECT_INPUT;
    }
    else 
        return INCORRECT_INPUT;
     
}


static status realloc_lines(char** lines, int* current_num_of_lines,  int line) {

    *current_num_of_lines *= 2;
    char** implemented_lines = (char**) realloc(lines, sizeof(char*) * (*current_num_of_lines));

    ERROR(!implemented_lines, MEM_ERR);
    
    lines = implemented_lines;

    for (int new_line = line; new_line < *current_num_of_lines; new_line++)
        lines[new_line] = NULL;
    
    return OK;
}

static status realloc_tokens(Token* tokens, int* current_num_of_tokens,  int token_id) {

    *current_num_of_tokens *= 2;
    Token* implemented_tokens = (Token*) realloc(tokens, sizeof(Token) * (*current_num_of_tokens));

    ERROR(!implemented_tokens, MEM_ERR);
    
    tokens = implemented_tokens;

    for (int new_token_id = token_id; new_token_id < *current_num_of_tokens; new_token_id++)
        tokens[new_token_id] = POISON;
    
    return OK;
}


static char** string_split(char* code, int* num_of_words) {

    int current_max_words_in_line = words_in_line;

    char** words = (char**) calloc(sizeof(char*), 2);
    //printf("words: %lu\n", sizeof(char*) * 2);

    int shift = -1;
    int word_count = 0;
    int line_length = strlen(code);

    while (code[shift] != '\n') {

        if (word_count >= current_max_words_in_line) {
            realloc_lines(words, &current_max_words_in_line, word_count);
        }

        words[word_count] = (char*) calloc(sizeof(char), input_format_max_len);
        //printf("word: %lu\n", sizeof(char) * input_format_max_len);

        int word_length = 0;

        shift = get_word(&words[word_count], &word_length, shift+1, code);
        // printf("word: %s, length: %d\n", words[word_count], length);

        word_count++;

        if (shift == line_length) break;
    }
    *num_of_words = word_count;
    return words;
}

static char** code_split(FILE* file, int* num_of_lines) { 

    char** lines = (char**) calloc(sizeof(char*), max_num_of_lines);
    //printf("lines: %lu\n", sizeof(char*) * max_num_of_lines);

    int current_max_num_of_lines = max_num_of_lines;
    int line_count = 0;
    while (!feof(file)) {

        if (line_count >= current_max_num_of_lines) {
            realloc_lines(lines, &current_max_num_of_lines, line_count);
        }
        lines[line_count] = (char*) calloc(sizeof(char), code_string_max_len);
        //printf("line: %lu\n", sizeof(char) * code_string_max_len);

        fgets(lines[line_count], code_string_max_len, file);
        line_count++;
    }
    *num_of_lines = line_count;
    return lines;
}




status tokenize(Token** token_sequence, int* num_of_tokens, const char* const code_file_name) {

    ERROR(!token_sequence, BAD_PTR); //is_bad_ptr() ?
    ERROR(!num_of_tokens, BAD_PTR);
    ERROR(!code_file_name, BAD_PTR);
    
    FILE* code_file = fopen(code_file_name, "rb");

    ERROR(code_file == NULL, FILE_ERR);

    int num_of_code_lines = 0;
    char** code_lines = code_split(code_file, &num_of_code_lines);

    fclose(code_file);

    Token* tokens = (Token*) calloc(sizeof(Token), start_max_num_of_tokens);
    //printf("tokens: %lu\n", sizeof(Token) * start_max_num_of_tokens);
    ERROR(!tokens, MEM_ERR);

    for (int token_id = 0; token_id < start_max_num_of_tokens; token_id++)
        tokens[token_id] = POISON;


    int current_tokens_size = start_max_num_of_tokens;
    int token_id = 0;

    for (int line = 0; line < num_of_code_lines; line++) {

        int num_of_words = 0;
        char** words = string_split(code_lines[line], &num_of_words);

        ERROR(check_command(words, num_of_words), INCORRECT_INPUT);

        for (int i = 0; i < num_of_words; i++) {

            if (token_id >= current_tokens_size) 
                realloc_tokens(tokens, &current_tokens_size, token_id);
                        
            // tokens[token_id] = ({  
            //     .type = (i == 0) ? IDENT_COMMAND : NUMBER,
            //     .name = words[i],
            //     .number = (i == 1) ? str_to_int(words[i]) : 0
            // }) suk(((((((

            tokens[token_id].type = (i == 0) ? IDENT_COMMAND : NUMBER;
            tokens[token_id].name = words[i]; 
            tokens[token_id].number = (i == 1) ? str_to_int(words[i]) : 0;
            
            token_id++;
            // printf("token_id %d\n", token_id);

        }
    }


    *token_sequence = tokens;
    *num_of_tokens = token_id;

    return OK;

}



//realloc for tokens and for code_lines

