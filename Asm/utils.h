typedef enum status {
    OK = 0              ,
    MEM_ERR = -4096     ,
    BAD_PTR             ,
    FILE_ERR            ,
    INCORRECT_INPUT     ,
}status;



const char* commands[]  = {"push", "add", "sub", "mul", "div", "out", "htl"};

const int num_of_commands = 7;