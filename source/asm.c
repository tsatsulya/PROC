#include "asm.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define token_type_(i) tokens.buffer[i].type
#define token_name_(i) tokens.buffer[i].name

define_array(int);

#define print(i) printf("%ld\n", i);



status_t assemble(AsmData* data) {
    
    array(Token) tokens;

    int num_of_labels = 0;

    status_t status = tokenize(&tokens, *(data->in_file_name));
    if (status != OK) return status;

    int num_of_tokens = tokens.size;
    array(int) token_sequence = create_array(int, num_of_tokens);

    for (int i = 0; i < num_of_tokens; i++) {
        if (tokens.buffer[i].type == LABEL) 
            num_of_labels++;
    }

    array(Label) labels = create_array(Label, num_of_labels);

    //FILE* file_out = fopen(*(data->out_file_name), "w"); // ?

    // TODO: this is the main i dream of:
    // int main() {
    //     const char* input_text = read_file(text);

    //     array(token) tokens = tokenize(input_text);

    //     array(instruction)
    //         instructions = assemble(tokens);

    //     write_binary(instructions, output_file);
    // }

    // AssembleContext ctx = {};
    // assemble(ctx);
    // assemble(ctx);

    for (int i = 0; i < num_of_tokens; i++) { 

        Line token_name = tokens.buffer[i].name;


        if (token_type_(i) == NUMBER) {
            token_sequence.buffer[i] = str_to_int(token_name);
        }

        else if (token_type_(i) == LABEL) {

            Label lb = {
                .name = token_name,
                .offset = i
            };
            push_element(Label, labels, lb);
            push_element(int, token_sequence, SET_LABEL);

        }

        else if (token_type_(i) == COMMAND) {
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

        if (token_sequence.buffer[i] == JUMP) {

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

    for (long unsigned i = 0; i < token_sequence.size; i++) {
        printf("tok %d\n", token_sequence.buffer[i]);
    }

    FILE* file_out = fopen("bin_out.txt", "w");


    for (int i = 0; i < num_of_tokens; i++) {
        fprintf(file_out, "%d ", token_sequence.buffer[i]);
    }

    //FREE CODE_LINES!!!!!!111
    free(tokens.buffer);
    fclose(file_out);
    return OK;
}


