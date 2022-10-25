#include "tokenization.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const size_t code_string_max_len = 32;
const size_t input_format_max_len = 16; 

const size_t max_num_of_lines = 256;
const size_t start_max_num_of_tokens = max_num_of_lines * 2;

#define ERROR(COND, TO_RET)                                     \
    if (COND) {                                                 \
        return TO_RET;                                          \
    }
        // if (TO_FREE1)                                           
        //     free(TO_FREE1);                                     
        // if (TO_FREE2)                                           
        //     free(TO_FREE2);                                     
        //     



static int get_word(char* word, int* length, int start, const char *code) {   

    for (; *(code + start) == ' '; start++) {}

    int shift = start;

    for (;; shift++) {
        if (*(code + shift) == ' ' || *(code + shift) == '\n') {
            break;
        }
        else {
            //printf("%c",*(code + shift));
            word[shift-start] = *(code + shift);
        }
    }
    *length = shift - start;
    return shift++;
}

static int string_is_number(const char* string, int* number) {
    *number = 0;
    int length = sizeof(string) - 1;

    for (int i = 0; i < length; i++) {
        int n = (int*)string[i];

        if ( 48 <= n && n <= 57)
            *number += (n - 48) * pow(10, length - i);
        else {
            return false;
        }
    }
    return true;
}

static int string_without_digits(char* string, int length) {
    for (int i = 0; i < length; i++) {
        if (isdigit(string[i]))
            return false;
    }
    return true;
}

static is_command(char* string) {
    for (int i = 0; i < num_of_commands; i++) {
        if (strcmp(string, commands[i]) == 0) 
            return true; 
    }
    return false;
}

status check_command(char** words, int word_count) {

    if (word_count > 2) return INCORRECT_INPUT;

    if (is_command(words[0])) {
        bool is_push = (words[0] == "push");
        int number = 0;
        bool args = (word_count == 1) ? false : string_is_number(words[1], &number);

        if (args) 
        if (!(is_push ^ args)) 
            return OK;
        else 
            return INCORRECT_INPUT;
    }
    else 
        return INCORRECT_INPUT;
     
}



static int string_split(char* code_string, char** words[2]) {

    if (!code_string) return BAD_PTR;
    int shift = -1;
    int word_count = 0;

    while (code_string[shift] != '\n') {

        if (word_count == 2) {
            return INCORRECT_INPUT;
        }

        char word[input_format_max_len] = {};   
        int length = 0;

        shift = get_word(word, &length, shift+1, code_string);
        //printf("word: %s, length: %d\n", word, length);
        *words[word_count] = word;
        word_count++;


        // if (word_count == 1) {
        //     (&words)[0] = word;
        //     if (!string_without_digits(word, length))
        //         return INCORRECT_INPUT; 
        // }

        // if (word_count == 2) {
        //     (&words)[1] = word;
        //     if (!string_is_number(word, length)) 
        //         return INCORRECT_INPUT;
        // }

    }
    return  word_count;
    //puts("end");
}

Token* realloc_tokens(Token* tokens, int* current_num_of_tokens,  int token_id) {

    *current_num_of_tokens *= 2;
    Token* implemented_tokens = (Token*) realloc(tokens, sizeof(Token) * (*current_num_of_tokens));

    ERROR(!implemented_tokens, MEM_ERR);
    
    tokens = implemented_tokens;

    for (int new_token_id = token_id; new_token_id < current_num_of_tokens; new_token_id++)
        tokens[new_token_id] = POISON;
    
    return tokens;
}

int code_split(char* full_line, char** array_of_lines) { 

    int current_max_num_of_lines = max_num_of_lines;
    // 
    int line_count = 1;
    array_of_lines[0] = full_line;

    for (int i = 0; full_line[i] != '\0'; i++) {

        if (line_count >= current_max_num_of_lines) {

            current_max_num_of_lines *= 2;
            char* implemented_array_of_lines = (char*) realloc(array_of_lines, sizeof(char) * (current_max_num_of_lines));

            ERROR(!implemented_array_of_lines, MEM_ERR);
            array_of_lines = implemented_array_of_lines;
        }

        if (full_line[i] == '\n')  {

                full_line[i] = '\0';

                array_of_lines[line_count] = full_line + i + 1;
                line_count++;
        }   
    }

    return line_count;
}



status tokenize(Token** token_sequence, size_t* num_of_tokens, const char* const code_file_name) {

    ERROR(!token_sequence, BAD_PTR); //is_bad_ptr() ?
    ERROR(!num_of_tokens, BAD_PTR);
    ERROR(!code_file_name, BAD_PTR);
    
    FILE* code_file = fopen(code_file_name, "rb");

    ERROR(code_file == NULL, FILE_ERR);

    fseek(code_file, 0, SEEK_END);
    int num_of_symbols = ftell(code_file) + 1;
    rewind(code_file);

    char* code = (char*) calloc(sizeof (char), num_of_symbols);
    
    if (code == NULL) {
        fclose (code_file);
        return MEM_ERR;
    }

    fread(code, sizeof(char), num_of_symbols, code_file);

    char* code_lines = (char*) calloc(sizeof(char), max_num_of_lines);

    int num_of_code_lines = code_split(code, &code_lines);

    fclose(code_file);




    Token* tokens = (Token*) calloc(sizeof(Token), start_max_num_of_tokens);

    ERROR(!tokens, MEM_ERR);
    
    for (int token_id = 0; token_id < start_max_num_of_tokens; token_id++)
        tokens[token_id] = POISON;

    int current_tokens_size = start_max_num_of_tokens;

    int token_id = 0;
    for (int line = 0; line < num_of_code_lines; line++) {
        char* words[2] = {};
        int num_of_words = string_split(code_lines[line], words);

        ERROR(check_command(words, num_of_words), INCORRECT_INPUT);

        for (int i = 0; i < num_of_words; i++) {

            if (token_id == current_tokens_size) 
                realloc_tokens(tokens, current_tokens_size, token_id);
            token_id++;
            tokens[token_id] = {
                .type = (i == 0) ? IDENT_COMMAND : NUMBER, 
                .name = words[i],
                .number = (i == 1) ? string_is_number
            };
        }

    }


        




    }   //sequence!!!!!


    return OK;

    }



//realloc for tokens and for code_lines