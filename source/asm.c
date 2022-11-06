#include "asm.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define token_type_(i) tokens.buffer[i].type
#define token_name_(i) tokens.buffer[i].name

// define_array(int);

#define print(i) printf("%ld\n", i);

static int jumps[] = {JUMP, JB, JE, JA, JAE};
static int jumps_array_size = sizeof(jumps)/sizeof(int);

static bool is_jump(int command_id) {
    for (int i = 0; i < jumps_array_size; i++)
        if (jumps[i] == command_id) return true;
    return false;
}

static double sqrt_(double x)
{
    double y;
    int p, square, c;

    p = 0;
    do
    {
        p++;
        square = (p+1) * (p+1);
    }
    while (x > square);

    y = (double)p;
    c = 0;

    while (c < 10)
    {
        y = (x/y + y)/2;
        if (y * y == x)
            return y;
        c++;
    }
    return y;
}

status_t assemble(AsmData* data) {
    
    array(Token) tokens;
    array(Line) code_to_free;
    Token argument;
    int num_of_labels = 0;

    status_t status = tokenize(&tokens, *(data->in_file_name), &code_to_free);
    if (status != OK) return status;
    //printf("tok status %d\n", status);

    int num_of_tokens = tokens.size;
    array(double) token_sequence = create_array(double, num_of_tokens);

    for (int i = 0; i < num_of_tokens; i++) {
        if (tokens.buffer[i].type == LABEL) 
            num_of_labels++;
    }

    array(Label) labels = create_array(Label, num_of_labels);

    for (int i = 0; i < num_of_tokens; i++) { 

        Line token_name = tokens.buffer[i].name;
        if (token_type_(i) == NUMBER) {
            token_sequence.buffer[i] = (double)str_to_int(token_name);
        }

        else if (token_type_(i) == LABEL) {

            Label lb = {
                .name = token_name,
                .offset = i
            };
            push_element(Label, labels, lb);
            push_element(double, token_sequence, SET_LABEL);

        }

        else if (token_type_(i) == COMMAND) {
            //puts_line(token_name_(i));
            #define DEF_CMD(id, name, arg, assemble, code_to_run)               \
                if (linecmp(token_name_(i), name) == 0) {                       \
                     assemble                                                   \
                } else                                                          \

            #include "commands.inc"

            {   
                puts("bl cho eto");
                assert(false && "Illegal command!");
            } 
        }

    }

    for (int i = 0; i < num_of_tokens; i++) { 

        if (is_jump(token_sequence.buffer[i])) {
            Line label = tokens.buffer[i+1].name;
            int label_id;

            bool is_correct_label = false;
            for (long unsigned j = 0; j < labels.size; j++) {
                if (linescmp(label, labels.buffer[j].name) == 0) {
                    is_correct_label = true;
                    label_id = j;
                    break;
                }
            }

            if (!is_correct_label) return INCORRECT_INPUT;

            token_sequence.buffer[i+1] = labels.buffer[label_id].offset;
        }
    }
    // for (long unsigned i = 0; i < token_sequence.size; i++) {
    //     printf("tok %d\n", (int)token_sequence.buffer[i]);
    // }

    FILE* file_out = fopen("bin_out.bin", "w");
        
    for (int i = 0; i < num_of_tokens; i++) {
        fwrite(&token_sequence.buffer[i], sizeof(token_sequence.buffer[i]), 1, file_out);
    }

    //FREE CODE_LINES!!!!!!111
    free(token_sequence.buffer);
    free(tokens.buffer);
    free(code_to_free.buffer);
    free(labels.buffer);
    fclose(file_out);
    return OK;
}


