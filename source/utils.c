#include "utils.h"
#include "string.h"
#include "stdio.h"

// TODO: utils is a bit low-effort

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

int str_to_int(const char* string) { // TODO: bool* is_correct? (also see strtol)

    int number = 0;
    int mult = 1;
    int length = (int)strlen(string); 

    for (int i = 0; i < length; i++) {
        if (i == 0 && string[i] == '-') {
            mult = -1;
            continue;
        }
        int n = string[i];
        number += (n - 48) * (int)pow_(10, length - i-1);
        // TODO:       ^~ is this '0'?...
    }
    return number * mult;
}

bool is_bad_ptr(void* ptr) {
    if ((long int)ptr < 0x0000ffff) return true;
    return false;
}