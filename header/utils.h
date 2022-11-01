#pragma once

#include <stdbool.h>
typedef enum status_t {
    OK = 0              ,
    MEM_ERR = -4096     ,
    BAD_PTR             ,
    FILE_ERR            ,
    INCORRECT_INPUT     ,
}status_t;


long long int pow_(int x, int power);
bool string_is_number(const char* string);
int str_to_int(const char* string);




