#include "asm.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>



status_t assemble(AsmData* data) {
    
    Token* tokens;

    int num_of_tokens = 0;
    status_t status = tokenize(&tokens, &num_of_tokens, data->in_file_name);

    int* token_sequence = (int*) calloc(sizeof(int), num_of_tokens);

    if (status != OK) return status;

    FILE* file_out = fopen(data->out_file_name, "w");

    for (int i = 0; i < num_of_tokens; i++) {

        char* token_name = tokens[i].name;

        if (string_is_number(token_name)) {
            token_sequence[i] = str_to_int(token_name);
        }

        else if (!strcmp(token_name, "push")) {
            token_sequence[i] = PUSH;
        }

        else if (!strcmp(token_name, "add")) {
            token_sequence[i] = ADD;            
        }
        else if (!strcmp(token_name, "sub")) {            
            token_sequence[i] = SUB;
        }

        else if (!strcmp(token_name, "mul")) {            
            token_sequence[i] = MUL;
        }

        else if (!strcmp(token_name, "div")) {            
            token_sequence[i] = DIV;
        }

        else if (!strcmp(token_name, "out")) {            
            token_sequence[i] = OUT;
        }

        else if (!strcmp(token_name, "hlt")) {            
            token_sequence[i] = HLT;
        }

        else            
            token_sequence[i] = ERROR;

    }



    for (int i = 0; i < num_of_tokens; i++) {
        fprintf(file_out, "%d ", token_sequence[i]);
    }
    free(tokens);
    fclose(file_out);
    return OK;
}
