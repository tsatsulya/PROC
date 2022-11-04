#include "strings.h"
#include "utils.h"

#define min(x, y) ((x) < (y)) ? (x) : (y)
#define array(type) array_##type


void print_lines(array(Line) array) {
    // printf("sizeee %d\n", array.size);
    for (int i = 0; i < array.size; i++) {
        puts(array.buffer[i].first_symbol);
    }
}

void puts_line(Line string) {
    for (int j = 0; (long unsigned)j < string.length; j++) {
        printf("%c", *(string.first_symbol + j));
    }
    puts("");
}

int linecmp(Line string1, char* string2) {
    
    int length2 = strlen(string2);
    int min_len = min((int)string1.length, length2);

    for (int i = 0; i < min_len; i++) {

        int dif = string1.first_symbol[i] - string2[i];
        if (dif != 0) return dif;
    }
    return string1.length - length2;
}

int str_to_int(Line string) { // TODO: bool* is_correct? (also see strtol)

    int number = 0;
    int mult = 1;
    int length = string.length; 

    for (int i = 0; i < length; i++) {
        if (i == 0 && string.first_symbol[i] == '-') {
            mult = -1;
            continue;
        }
        int n = string.first_symbol[i];
        number += (n - '0') * (int)pow_(10, length - i-1);
    }
    return number * mult;
}