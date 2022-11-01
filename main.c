#include <stdio.h>
#include "asm.h"
#include "cpu.h"
int main() {

    Token* tokens = NULL;
    char* in_file_name = "test.txt";
    char* out_file_name = "test_out.txt";  
    AsmData data = {
        .in_file_name = &in_file_name,
        .out_file_name = &out_file_name,
        .tokens = tokens,
        .num_of_tokens = 0,
    };
    status_t status = assemble(&data);
    printf(" status %d\n", status);
    Processor proc;
    cpu_load(&proc, "test_out.txt");
    cpu_exec(&proc);

    return 0;
}