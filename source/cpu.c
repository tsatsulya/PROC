#include "cpu.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

const int EPS = 4;

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

static double sqrt_(double x)
{
    double y;

    int square, c;
    int p = 0;

    do {
        p++;
        square = (p+1) * (p+1);
    } while (x > square);

    y = (double)p;
    c = 0;

    while (c < 50) {
        y = (x/y + y)/2;
        if (y * y == x)
            return y;
        c++;
    }
    return y;
}

static double double_round(double x, int decimal) {
    int multiplier = pow_(10, decimal);
    x = x * multiplier + 0.5;
    x = (double)((int)x) /multiplier;
    return x; 
}

static double double_cmp(double x, double y, int decimal) {
    x = double_round(x, decimal);
    y = double_round(y, decimal);
    return x - y;
}

status_t cpu_load(Processor* cpu, const char* in_file_name) {

    if (is_bad_ptr(cpu) || is_bad_ptr((void*)in_file_name)) return BAD_PTR;

    FILE* in_file = fopen(in_file_name, "rb");
    if (in_file == NULL)
        return FILE_ERR;

    double s;
    
    array(double) code = create_array(double, 16);

    size_t shift = 0;

    int width = sizeof(double);

    while (fread(&s, sizeof(double), 1, in_file)) {
        push_element(double, code, s);
        //printf("element %lf\n", s);
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

    for (long unsigned i = 0; i < cpu->code.size; i++) {
        int command_id = cpu->code.buffer[i];
        if (command_id == SET_LABEL) continue;

        #define DEF_CMD(id, name, arg, assemble, code_to_run)   \
            if (command_id == id) {                             \
                code_to_run                                     \
            } else                                              \


        #include "commands.inc"

        {
            printf("command id %d\n", command_id);
            assert(false && "Illegal command!");
        } 
    }
    free (cpu->code.buffer);
    stack_destruct(&cpu_stack);
    return OK;
}
