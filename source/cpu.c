#include "cpu.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"


typedef double _type_; 
// define_array(int);

//ommands.inc

// DEF_CMD(0, JMP)
// DEF_CMD(1, RET)

// #undef DEF_CMD

// // ...

// const char* get_name(int command_id) {

//     switch (command_id) {

//         #define DEF_CMD(id, name, ) 
//             case id:              
//                 return #name;
            
//         #include "commands.inc"

//         ;
//     }
// }

status_t cpu_load(Processor* cpu, const char* in_file_name) {

    if (is_bad_ptr(cpu) || is_bad_ptr((void*)in_file_name)) return BAD_PTR;

    FILE* in_file = fopen(in_file_name, "rb");
    if (in_file == NULL)
        return FILE_ERR;

    int num_of_commands = 0;
    int s;
    
    array(long) code = create_array(long, 16);

    size_t shift = 0;

    int width = sizeof(long);

    while (fread(&s, sizeof(int), 1, in_file)) {
        push_element(long, code, s);
        printf("element %d\n", s);
        fseek(in_file, (++shift)*width, SEEK_SET);
    }
    rewind(in_file);
    fclose(in_file);
    cpu->code = code;
    return OK;
}



status_t cpu_exec(Processor* cpu) {

    Stack cpu_stack;
    stack_init(&cpu_stack, 5);
    printf("size %ld\n\n\n\n", cpu->code.size);

    for (long unsigned i = 0; i < cpu->code.size; i++) {
        int command_id = cpu->code.buffer[i];
        if (command_id == SET_LABEL) continue;

        #define DEF_CMD(id, name, arg, assemble, code_to_run) \
            if (command_id == id) {                           \
                code_to_run                         \
            } else                                  \


        #include "commands.inc"

        {
            assert(false && "Illegal command!");
        } 
    }
    return OK;
}


// static const int SET_LABEL  = 0x1abe1;
// static const int OUT        = 6;
// static const int HLT        = 7;
// static const int JUMP       = 8;
// static const int ERROR      = -1;
