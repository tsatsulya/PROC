#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h> 

typedef long Stack_data;
typedef long long int Canary_data;

typedef struct Stack {
    size_t capacity;
    size_t min_capacity; 
    double size_multiplier;
    size_t element_count;
    long long int hash;
    char* data;
    bool is_alive; // TODO: could be represented as some special value inside of other field

} Stack;

typedef enum stack_status {
    SUCCESS =               0b000000000, 
    STACK_NULL =            0b000000001,
    EMPTY_STACK =           0b000000010,
    STACK_OVERFLOW =        0b000000100,
    DATA_NULL =             0b000001000,
    INVALID_HASH =          0b000010000,
    DAMAGED_LEFT_CANARY =   0b000100000,
    DAMAGED_RIGHT_CANARY =  0b001000000,
    NULL_PTR =              0b010000000
} stack_status;

typedef enum fullness {
    EMPTY = 0,
    UNDER_HALF = 1,
    OVER_HALF = 2,
    FULL = 3,
}fullness;


stack_status stack_init(Stack* stack, size_t length);
stack_status stack_push(Stack* stack, Stack_data element);
Stack_data stack_pop(Stack* stack);
Stack_data stack_get_last_element(Stack* stack);
stack_status stack_resize(Stack* stack);
stack_status stack_destruct(Stack* stack);
void stack_data_print(Stack* stack, FILE* file);
void print_size_info(Stack* stack, FILE* file);
void stack_dump(Stack* stack, const char* file, const char* func, int line, unsigned status);
