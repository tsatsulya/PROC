#pragma once

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef enum status {
    OK = 0              ,
    MEM_ERR = -4096     ,
    BAD_PTR             ,
    FILE_ERR            ,
    INCORRECT_INPUT     ,
}status;



static const char* commands[]  = {"push", "add", "sub", "mul", "div", "out", "htl"};
static const int num_of_commands = 7;

#endif

