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
#define print(i) printf("%d\n", i);

static bool is_not_empty(char* line) {

    if (strlen(line) <= 1) return false;

    for (int i = 0; ; i++) {
        if (line[i] == '\0' && line[i] == '\n') break;
        if (line[i] != 32) return true;
    }
    return false;
}

// static char* get_lable_name(char* code_lable) {
//     int length = strlen(code_lable);
//     char* lable_name = (char*) calloc(1, length-1);
//     strncpy(lable_name, code_lable, length - 1);

//     return lable_name;
// }

static int get_word(Line* word, int start,  char* code) {   

    for (; *(code + start) == ' '; start++) {}

    int shift = start;

    for (;; shift++) {
        if (code[shift] == ' ' || code[shift] == '\n' || code[shift] == '\0') {
            break;
        }
    }
    
    printf("ppp %p\n", word);
    word->length = shift - start;
    word->first_symbol = code + start;
    return shift++;
}




static array(Line)* string_split(char* code) {

    array(Line) words = create_array(Line, 0);

    int shift = -1;
    int word_count = 0;
    int line_length = strlen(code);

    while (code[shift+1] != '\n') {
        print(shift);
        shift = get_word(&words.buffer[word_count], shift+1, code);
        print(shift);
        puts_line(words.buffer[word_count]);

        if (words.buffer[word_count].length) {

            word_count++;
        }

        if (shift == line_length) break;
    }
    // print_lines(words_.buffer, words_.size);
    words.size = word_count;
    return &words;
}   

static array(Line) code_split(FILE* file) { 

    array(Line) lines = create_array(Line, max_num_of_lines);

    int line_count = 0;
    while (!feof(file)) {

        char* line_of_code = (char*) calloc(sizeof(char), code_string_max_len);

        fgets(line_of_code, code_string_max_len, file);
        
        Line ll = (Line) {
            .first_symbol = line_of_code,
            .length = strlen(line_of_code)
        };

        // printf("%s\n\n", ll.first_symbol);

        if (is_not_empty(line_of_code)) {
            lines.buffer[line_count].first_symbol = line_of_code;
            line_count++;
        }
    }
    lines.size = line_count;
    //printf("size %d\n", lines.size);
    //print_lines(lines);
    return lines;
}


// static void tokens_output(Token* tokens, int num_of_tokens) {
//     for (int i = 0; i < num_of_tokens; i++) {
//         printf("type %c    ", tokens[i].type);
//         puts_line(tokens[i].name);
//     }
// }



status_t tokenize(array(Token)* token_sequence, const char* code_file_name) {

    ERROR_(is_bad_ptr(token_sequence), BAD_PTR);
    ERROR_(is_bad_ptr((void*) code_file_name), BAD_PTR);
    
    FILE* code_file = fopen(code_file_name, "rb");

    ERROR_(code_file == NULL, FILE_ERR);

    array(Line) code_lines = code_split(code_file);

    fclose(code_file);

    puts("------------------------------------------------------------------------------------------");
    print_lines(code_lines);
    array(Token) tokens = create_array(Token, start_max_num_of_tokens);

    int token_id = 0;
    int num_of_code_lines = code_lines.size;


    for (int line = 0; line < num_of_code_lines; line++) {

        int num_of_words = 0;

        array(Line) words = *string_split(code_lines.buffer[line].first_symbol);
        puts("------------------------------------------------------------------------------------------");
        print_lines(words);
        for (int i = 0; i < num_of_words; i++) {

            TokenType type;
            // char** name;
            // int number;

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
                type = (linecmp(words.buffer[0], "jump") == 0) ? JUMP_TO : NUMBER;
            }

            tokens.buffer[token_id].type = type; 
            tokens.buffer[token_id].name = words.buffer[i]; 
            tokens.buffer[token_id].number = (i == 1) ? str_to_int(words.buffer[i]) : 0;
            
            token_id++;

        }
    }

    tokens.size = token_id;
    *token_sequence = tokens;

    //printf("num_of_tokens = %d\n", token_id);
    // tokens_output(tokens, token_id);
    return OK;

}



//realloc for tokens and for code_lines

