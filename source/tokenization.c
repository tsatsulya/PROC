#include "tokenization.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrays.h"
#include "strings.h"


#define ERROR_(COND, TO_RET)                                    \
    if (COND) {                                                 \
        return TO_RET;                                          \
    }
#define print(i) printf("%ld\n", i);

static bool is_not_empty(char* line) {

    if (strlen(line) <= 1) return false;

    for (int i = 0; ; i++) {
        if (line[i] == '\0' && line[i] == '\n') break;
        if (line[i] != 32) return true;
    }
    return false;
}

static int get_word(Line* word, int start,  char* code) {   

    for (; *(code + start) == ' '; start++) {}

    int shift = start;

    for (;; shift++) {
        if (code[shift] == ' ' || code[shift] == '\n' || code[shift] == '\0') {
            break;
        }
    }
    
    word->length = shift - start;
    word->first_symbol = code + start;
    return shift++;
}




static array(Line) string_split(char* code) {

    array(Line) words = create_array(Line, 0);

    int shift = -1;
    int word_count = 0;
    int line_length = strlen(code);
    Line word;

    while (code[shift+1] != '\n') {
    
        shift = get_word(&word, shift+1, code);

        if (word.length) {
            push_element(Line, words, word);
            word_count++;
        }

        if (shift == line_length) break;
    }

    words.size = word_count;
    return words;
}   

static array(Line) code_split(FILE* file) { 

    array(Line) lines = create_array(Line, max_num_of_lines);

    int line_count = 0;
    while (!feof(file)) {

        char* line_of_code = (char*) calloc(sizeof(char), code_string_max_len);

        fgets(line_of_code, code_string_max_len, file);;

        if (is_not_empty(line_of_code)) {
            Line ll = {
                .first_symbol = line_of_code,
                .length = strlen(line_of_code),
            };

            push_element(Line, lines, ll);
            line_count++;
        }
    }
    return lines;
}


static void tokens_output(array(Token) tokens) {
    for (long unsigned i = 0; i < tokens.size; i++) {
        printf("type: %c\n", tokens.buffer[i].type);
        printf("name:");
        puts_line(tokens.buffer[i].name);
        printf("number: %ld\n", tokens.buffer[i].number);
        puts("");
    }
}



status_t tokenize(array(Token)* token_sequence, const char* code_file_name, array(Line)* code_to_free) {

    ERROR_(is_bad_ptr(token_sequence), BAD_PTR);
    ERROR_(is_bad_ptr((void*) code_file_name), BAD_PTR);
    
    FILE* code_file = fopen(code_file_name, "rb");

    ERROR_(code_file == NULL, FILE_ERR);

    array(Line) code_lines = code_split(code_file);

    fclose(code_file);

    array(Token) tokens = create_array(Token, start_max_num_of_tokens);

    int token_id = 0;
    int num_of_code_lines = code_lines.size;

    for (int line = 0; line < num_of_code_lines; line++) {
        array(Line) words = string_split(code_lines.buffer[line].first_symbol);
        // print_lines(words);

        for (long unsigned i = 0; i < words.size; i++) {

            TokenType type;

            if (i == 0) {
                if (*(words.buffer[0].first_symbol + words.buffer[0].length - 1) == ':') {
                    type = LABEL;
                    words.buffer[0].length--;
                }
                else {

                    type = COMMAND;
                }
            }     
            else {               
                char* first_symbol = words.buffer[i].first_symbol;
                size_t length =  words.buffer[i].length;

                if (*(first_symbol) == '[') {

                    if (*(first_symbol + length - 1) == ']') {
                        
                        Line address = {
                            .first_symbol = first_symbol + 1,
                            .length = length - 2
                        };

                        if (line_is_number(address)) {
                            type = ADDRESS;
                            words.buffer[i].first_symbol++;
                            words.buffer[i].length -= 2;

                        }
                        else 
                            type = COMMAND;
                    }
                }
                else {

                    type = (line_is_number(words.buffer[i])) ? NUMBER : COMMAND;
                    }
            }
            tokens.buffer[token_id].type = type; 
            tokens.buffer[token_id].name = words.buffer[i]; 
            tokens.buffer[token_id].number = (i == 1) ? (double)str_to_int(words.buffer[i]) : 0;
            
            token_id++;

        }

        free(words.buffer);

    }
    tokens.size = token_id;
    *token_sequence = tokens;
    *code_to_free = code_lines;
    tokens_output(tokens);
    
    return OK;

}



//realloc for tokens and for code_lines

