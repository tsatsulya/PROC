// #define DEF_CMD(id, name, code_to_run, assemble, disassemble) 
        //     if (command_id == id) {                                   
        //         code_to_run                         
        //     } else 



DEF_CMD (PUSH, "push", 1,
{    
    Token argument = tokens.buffer[i+1]; 
    push_element(int, token_sequence, PUSH);
    if (argument.type == NUMBER) 
        push_element(int, token_sequence, argument.number);
    else 
        assert(false && "incorrect input!");
    i++;
}, {})


DEF_CMD (ADD, "add", 0,
{    
    push_element(int, token_sequence, ADD);
}, {})


DEF_CMD (MUL, "mul", 0,
{    
    push_element(int, token_sequence, MUL);

}, {})


DEF_CMD (SUB, "sub", 0,
{    
    push_element(int, token_sequence, SUB);

}, {})


DEF_CMD (DIV, "div", 0,
{    
    push_element(int, token_sequence, DIV);
    
}, {})


DEF_CMD (OUT, "out", 0,
{    
    push_element(int, token_sequence, OUT);

}, {})

DEF_CMD (JUMP, "jump", 0,
{    
    Token argument = tokens.buffer[i+1]; 
    push_element(int, token_sequence, JUMP);
    if (argument.type == COMMAND) 
        push_element(int, token_sequence, 0);
    else 
        assert(false && "incorrect input!");
    i++;

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