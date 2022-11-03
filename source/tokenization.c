#include "tokenization.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "arrays.h"

define_array(Line);
define_array(Token);


#define ERROR_(COND, TO_RET)                                    \
    if (COND) {                                                 \
        return TO_RET;                                          \
    }


static bool is_not_empty(char* line) {

    if (strlen(line) <= 1) return false;

    for (int i = 0; ; i++) {
        if (line[i] == '\0' && line[i] == '\n') break;
        if (line[i] != 32) return true;
    }
    return false;
}

static char* get_lable_name(char* code_lable) {
    int length = strlen(code_lable);
    char* lable_name = (char*) calloc(1, length-1);
    strncpy(lable_name, code_lable, length - 1);

    return lable_name;
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
    // puts(word);
   
    *word_length = shift - start;
    *word_ = word;
    return shift++;
}




static status_t realloc_lines(char** lines, int* current_num_of_lines,  int line) {

    *current_num_of_lines *= 2;
    char** implemented_lines = (char**) realloc(lines, sizeof(char*) * (*current_num_of_lines));

    ERROR_(!implemented_lines, MEM_ERR);
    
    lines = implemented_lines;

    for (int new_line = line; new_line < *current_num_of_lines; new_line++)
        lines[new_line] = NULL;
    
    return OK;
}

static status_t realloc_tokens(Token* tokens, int* current_num_of_tokens) {

    *current_num_of_tokens *= 2;
    Token* implemented_tokens = (Token*) realloc(tokens, sizeof(Token) * (*current_num_of_tokens));

    ERROR_(!implemented_tokens, MEM_ERR);
    
    tokens = implemented_tokens;
    
    return OK;
}


static char** string_split(char* code, int* num_of_words) {

    int current_max_words_in_line = words_in_line;

    char** words = (char**) calloc(sizeof(char*), 2);

    int shift = -1;
    int word_count = 0;
    int line_length = strlen(code);

    while (code[shift] != '\n') {

        if (word_count >= current_max_words_in_line) {
            realloc_lines(words, &current_max_words_in_line, word_count);
        }

        words[word_count] = (char*) calloc(sizeof(char), input_format_max_len);

        int word_length = 0;

        shift = get_word(&words[word_count], &word_length, shift+1, code);

        if (word_length) word_count++;

        if (shift == line_length) break;
    }
    *num_of_words = word_count;
    return words;
}

static char** code_split(FILE* file, int* num_of_lines, array(Line)* lines__) { 

    char** lines = (char**) calloc(sizeof(char*), max_num_of_lines);
    array(Line) lines_ = create_array(Line, max_num_of_lines);


    // push_element(Line, lines_, ll);

    int current_max_num_of_lines = max_num_of_lines;
    int line_count = 0;
    while (!feof(file)) {

        if (line_count >= current_max_num_of_lines) {
            realloc_lines(lines, &current_max_num_of_lines, line_count);
        }
        char* line = (char*) calloc(sizeof(char), code_string_max_len);


        fgets(line, code_string_max_len, file);

        Line ll = (Line) {
            .first_symbol = line,
            .length = strlen(line)
        };

        push_element(Line, lines_, ll);


        if (is_not_empty(line)) {
            lines[line_count] = line;
            line_count++;
        }
    }
    *num_of_lines = line_count;
    *lines__ = lines_; 
    return lines;
}


static void tokens_output(Token* tokens, int num_of_tokens) {
    for (int i = 0; i < num_of_tokens; i++) {
        printf("type %c    ", tokens[i].type);
        puts(*tokens[i].name);
    }
}



status_t tokenize(Token** token_sequence, int* num_of_tokens, const char* const code_file_name) {

    ERROR_(is_bad_ptr(token_sequence), BAD_PTR);
    ERROR_(is_bad_ptr(num_of_tokens), BAD_PTR);
    ERROR_(is_bad_ptr((void*) code_file_name), BAD_PTR);
    
    FILE* code_file = fopen(code_file_name, "rb");

    ERROR_(code_file == NULL, FILE_ERR);

    int num_of_code_lines = 0;
    array(Line) code_lines_;
    char** code_lines = code_split(code_file, &num_of_code_lines, &code_lines_);

    fclose(code_file);

    Token* tokens = (Token*) calloc(sizeof(Token), start_max_num_of_tokens);

    ERROR_(!tokens, MEM_ERR);

    int current_tokens_size = start_max_num_of_tokens;
    int token_id = 0;

    for (int line = 0; line < num_of_code_lines; line++) {

        int num_of_words = 0;
        char** words = string_split(code_lines[line], &num_of_words);


        for (int i = 0; i < num_of_words; i++) {

            if (token_id >= current_tokens_size) 
                realloc_tokens(tokens, &current_tokens_size);
            
            if (i == 0) {

                int length = strlen(words[0]);

                if (words[0][length - 1] == ':') {

                    tokens[token_id].type = LABEL; 
                    words[0] = get_lable_name(words[0]);
                }
                else {
                    tokens[token_id].type = COMMAND;
                }
            }     
            else 
                tokens[token_id].type = (strcmp(words[0], "jump") == 0 ) ? JUMP_TO : NUMBER;

            tokens[token_id].name = &words[i]; 
            tokens[token_id].number = (i == 1) ? str_to_int(words[i]) : 0;
            
            token_id++;

        }
    }


    *token_sequence = tokens;
    *num_of_tokens = token_id;

    //printf("num_of_tokens = %d\n", token_id);
    tokens_output(tokens, token_id);
    return OK;

}



//realloc for tokens and for code_lines

