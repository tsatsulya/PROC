#include "cpu.h"
#include "stdlib.h"
#include "string.h"

status_t cpu_load(Processor* cpu, const char* const in_file_name) {

    // if (isBadPtr (CPU) || isBadPtr (in_file_name)) return BAD_PTR;

    FILE* in_file = fopen(in_file_name, "rb");
    if (in_file == NULL)
        return FILE_ERR;

    int num_of_commands = 0;
    int s;
    
    while ((fscanf(in_file, "%d", &s)!=EOF))
        num_of_commands+=1;
    rewind(in_file);
    
    cpu->code_size = num_of_commands;

    int *code = (int*) calloc(cpu->code_size, sizeof(int));  

    for(int i = 0; i < cpu->code_size; i++) {
        fscanf(in_file, "%d", &code[i]);
    }
    fclose(in_file);

    cpu->code = code;
    Stack stk;
    stack_init(&stk, 5); 
    cpu->cpu_stack = &stk;   

    return OK;
}



status_t cpu_exec(Processor* cpu) {

    Stack cpu_stack;
    stack_init(&cpu_stack, 5);
    int ax = 0;
    for (int i = 0; i < cpu->code_size; i++) {

        //printf("cpu i %d\n", i);
        int command_id = cpu->code[i];

        if (command_id == PUSH) {
            stack_push(&cpu_stack, cpu->code[i+1]);
        }
        else if (command_id == ADD) {
            stack_push(&cpu_stack, stack_pop(&cpu_stack) + stack_pop(&cpu_stack)); //1 element?
        }
        else if (command_id == SUB) {
            stack_push(&cpu_stack, (-1) * stack_pop(&cpu_stack) + stack_pop(&cpu_stack)); //
        }
        else if (command_id == MUL) {
            stack_push(&cpu_stack, stack_pop(&cpu_stack) * stack_pop(&cpu_stack)); //
        }
        else if (command_id == DIV) {
            stack_push(&cpu_stack, 1 / stack_pop(&cpu_stack) * stack_pop(&cpu_stack)); //
        }
        else if (command_id == OUT) {
            printf("%d\n", stack_get_last_element(&cpu_stack));
        }
        else if (command_id == JUMP) {
            if (ax) {
                //puts("jump");
                i = cpu->code[i+1];
                ax--;
            }
        }
    }
    //HLT!!!!!!!!!!!!!!!
    return OK;
}


// static const int SET_LABEL  = 0x1abe1;
// static const int OUT        = 6;
// static const int HLT        = 7;
// static const int JUMP       = 8;
// static const int ERROR      = -1;
