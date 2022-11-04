#pragma once

#include <stdio.h>
#include <string.h>
#include "arrays.h"

#define array(type) array_##type

typedef struct Line {
    char* first_symbol;
    size_t length;
} Line;

define_array(Line);


void print_lines(array(Line) array);

int linecmp(Line string1, char* string2);
int linescmp(Line string1, Line string2);

void puts_line(Line string);
int str_to_int(Line string);