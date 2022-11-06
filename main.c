#include <stdio.h>
#include "asm.h"
#include "cpu.h"
#include <stdlib.h>

int main() {

    array(Token) tokens;
    array(Label) labels;

    char* in_file_name = "test.txt";
    char* out_file_name = "test_out.txt";  
    AsmData data = {
        .in_file_name = &in_file_name,
        .out_file_name = &out_file_name,
        .tokens = tokens,
        .labels = labels
    };
    status_t status = assemble(&data);
    Processor proc;
    cpu_load(&proc, "bin_out.bin");
    cpu_exec(&proc);

    return 0;

    //Сашины комменты
    //ввод дабла
    //вывод дабла
    //функции 
    //проверка на скомпилированность
    
}
