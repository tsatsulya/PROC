#include <stdio.h>
#include "asm.h"
#include "cpu.h"
#include <stdlib.h>

int main() {

    array(Token) tokens = create_array(Token, 0);
    array(Label) labels = create_array(Label, 0);

    char* in_file_name = "test.txt";
    char* out_file_name = "test_out.txt";  
    AsmData data = {
        .in_file_name = &in_file_name,
        .out_file_name = &out_file_name,
        .tokens = tokens,
        .labels = labels
    };
    status_t status = assemble(&data);
    printf(" status %d\n", status);
    Processor proc;
    cpu_load(&proc, "test_out.txt");
    puts("a");
    cpu_exec(&proc);

    return 0;
}