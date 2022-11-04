// #define DEF_CMD(id, name, code_to_run, assemble, disassemble) 
        //     if (command_id == id) {                                   
        //         code_to_run                         
        //     } else 



DEF_CMD (PUSH, "push", 
{    
    token_sequence.buffer[i] = PUSH;
}, {})


DEF_CMD (ADD, "add", 
{    
    token_sequence.buffer[i] = ADD;
}, {})


DEF_CMD (MUL, "mul", 
{    
    token_sequence.buffer[i] = MUL;
}, {})


DEF_CMD (SUB, "sub", 
{    
    token_sequence.buffer[i] = SUB;
}, {})


DEF_CMD (DIV, "div", 
{    
    token_sequence.buffer[i] = DIV;
}, {})


DEF_CMD (OUT, "out", 
{    
    token_sequence.buffer[i] = PUSH;
}, {})


#undef DEF_CMD

//ommands.inc

// DEF_CMD(0, JMP)
// DEF_CMD(1, RET)

// #undef DEF_CMD

// // ...

// const char* get_name(int command_id) {

//     switch (command_id) {

//         #define DEF_CMD(id, name, ) 
//             case id:              
//                 return #name;
            
//         #include "commands.inc"

//         ;
//     }
// }


// #define foo(x) #x

        // foo(1) //=> "1"
        // foo(x) //=> "x"

        // #define DEF_CMD(id, name, code_to_run, assemble, disassemble) 
        //     if (command_id == id) {                                   
        //         code_to_run                         
        //     } else                                  


        // #include "commands.inc"

        // {
        //     assert(false && "Illegal command!");
        // } 