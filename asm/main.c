#include <stdio.h>
#include "asm.h"
#include "cpu.h"
int main() {

    Token* tokens = NULL;
    // status_t status = tokenize(&tokens, &num_of_tokens, "test.txt");
    AsmData data = {
        .in_file_name = "test.txt",
        .out_file_name = "test_out.txt",
        .tokens = tokens,
        .num_of_tokens = 0,
    };
    assemble(&data);
    //printf("%d", status);

    Processor proc;
    cpu_load(&proc, "test_out.txt");

    Stack my_stack;
    stack_init(&my_stack, 5);
    stack_push(&my_stack, 66);
    puts("ura");
    cpu_exec(&proc);

    // stack_dump(proc.cpu_stack, __FILE__, __func__, __LINE__, OK);

    return 0;
}