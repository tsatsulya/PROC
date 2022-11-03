#pragma once

#include <stdio.h>
#include <string.h>

typedef struct Line {
    char* first_symbol;
    size_t length;
} Line;

void print_lines(Line* array, int size);

int linecmp(Line string1, char* string2);