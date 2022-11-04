#include "asm.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define token_type_(i) tokens.buffer[i].type
#define token_name_(i) tokens.buffer[i].name

define_array(int);

status_t assemble(AsmData* data) {
    
    puts("i am in asm");
    array(Token) tokens;

    int num_of_labels = 0;
    int label_id = 0;

    status_t status = tokenize(&tokens, *(data->in_file_name));
    printf("tok status %d\n", status);
    if (status != OK) return status;
    printf("rr %d\n", tokens.size);

    int num_of_tokens = tokens.size;
    array(int) token_sequence = create_array(int, num_of_tokens);

    for (int i = 0; i < num_of_tokens; i++) {
        puts_line(tokens.buffer[i].name);
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
            labels.buffer[label_id] = lb;
            token_sequence.buffer[i] = SET_LABEL;

        }
        else if (token_type_(i) == COMMAND) {
            #define DEF_CMD(id, name, assemble, code_to_run)              \
                if (linecmp(token_name_(i), name) == 0) {                 \
                    assemble                                              \
                } else                                                    \


            #include "commands.inc"

            {
                assert(false && "Illegal command!");
            } 
        }

        printf("token %d\n", token_sequence.buffer[i]);
    }
    //     else if (!strcmp(token_name, "push")) {
    //         token_sequence[i] = PUSH;
    //     }

    //     else if (!strcmp(token_name, "add")) {
    //         token_sequence[i] = ADD;            
    //     }

    //     else if (!strcmp(token_name, "sub")) {            
    //         token_sequence[i] = SUB;
    //     }

    //     else if (!strcmp(token_name, "mul")) {            
    //         token_sequence[i] = MUL;
    //     }

    //     else if (!strcmp(token_name, "div")) {            
    //         token_sequence[i] = DIV;
    //     }

    //     else if (!strcmp(token_name, "out")) {            
    //         token_sequence[i] = OUT;
    //     }

    //     else if (!strcmp(token_name, "hlt")) {            
    //         token_sequence[i] = HLT;
    //     }
    //     else            
    //         token_sequence[i] = ERROR;

    //     //AAAAAAAAAA

    // }

    // for (int i = 0; i < num_of_tokens; i++) { // TODO: link labels

    //     if (!strcmp(*(tokens[i].name), "jump")) {

    //         char* label = *(tokens[i+1].name);
    //         bool is_correct_label = false;
    //         int label_id;

    //         for (int j = 0; j < num_of_labels; j++) {
               
    //            if (strcmp(label, *(labels[j].name)) == 0) {

    //                 if (is_correct_label) 
    //                     return INCORRECT_INPUT;

    //                 is_correct_label = true;
    //                 label_id = j;
    //             }
    //         }

    //         if (!is_correct_label) return INCORRECT_INPUT;

    //         token_sequence[i] = JUMP;
    //         token_sequence[i+1] = labels[label_id].offset;
    //     }
    // }

    // //FILE* bin_file_out = fopen("bin_out.txt", "w");


    // for (int i = 0; i < num_of_tokens; i++) {
    //     fprintf(file_out, "%d ", token_sequence[i]);
    // }
    // printf("labels %d\n", num_of_labels);
    // free(tokens);
    // fclose(file_out);
    return OK;
}


