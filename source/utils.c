#include "utils.h"
#include "string.h"
#include "stdio.h"

// TODO: utils is a bit low-effort
define_array(int);

long long int pow_(int x, int power) {
    long long int result = 1;
    for (int y = 0; y < power; y++) {
        result*=x;
    }
    return result;
}

bool string_is_number(const char* string) {

    int length = (int) strlen(string); 
    if (!length) return false;
    for (int i = 0; i < length; i++) {
        if (i == 0 && string[i] == '-')
            continue;
        int n = string[i];
        if ( n < 48 || 57 < n)
            return false;
    }
    return true;
}



bool is_bad_ptr(void* ptr) {
    if ((long int)ptr < 0x0000ffff) return true;
    return false;
}