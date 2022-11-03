#include "strings.h"

#define min(x, y) ((x) < (y)) ? (x) : (y)


void print_lines(Line* array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; (long unsigned)j < array[i].length; j++) {
            printf("%c", *(array[i].first_symbol + j));
        }
        puts("");
    }
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
