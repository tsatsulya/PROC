#include "strings.h"
#include "utils.h"

#define min(x, y) ((x) < (y)) ? (x) : (y)
#define array(type) array_##type


void print_lines(array(Line) array) {
    // printf("sizeee %d\n", array.size);
    for (long unsigned i = 0; i < array.size; i++) {
        for (long unsigned j = 0; j < array.buffer[i].length; j++ )
            printf("%c", *(array.buffer[i].first_symbol + j));
        puts("");
    }
}

void puts_line(Line string) {
    for (long unsigned j = 0; j < string.length; j++) {
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

int linescmp(Line string1, Line string2) {
    
    if (string1.length - string2.length) return string1.length - string2.length;

    for (long unsigned i = 0; i < string1.length; i++) {
        int dif = string1.first_symbol[i] - string2.first_symbol[i];
        if (dif != 0) return dif;
    }
    return 0;
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

bool line_is_number(Line string) {
   int length = string.length; 
    if (!length) return false;

    for (int i = 0; i < length; i++) {
        if (i == 0 && string.first_symbol[i] == '-')
            continue;
        int n = string.first_symbol[i];
        if (n < '0' || '9' < n)
            return false;
    }
    return true;
}
