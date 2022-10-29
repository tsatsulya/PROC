#pragma once

#include <errno.h>
#include <stdio.h>

enum debug_modes {
    ERROR_ = 0,
    WARNING_ = 1, 
    INFO_ = 2, 
    DEBUG_ = 3, 
    ULTRA_MEGA_DEBUG_ = 4,
};


#define DEBUG_MODE ERROR_
// TODO: maybe debug_exec
#define debug_msg(mode, my_func)                            \
    ({                                                      \
            if (mode <= DEBUG_MODE)                         \
                my_func;                                    \
    })

