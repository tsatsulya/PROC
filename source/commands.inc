//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (PUSH, "push", 1,
{    
    Token argument = tokens.buffer[i+1]; 
    
    if (argument.type == NUMBER) { 
        push_element(long, token_sequence, PUSH);
        push_element(long, token_sequence, argument.number);
    } 
    else if (argument.type == ADDRESS) {
        push_element(long, token_sequence, PUSHA);
        push_element(long, token_sequence, argument.number);
    }
    else 
        assert(false && "incorrect input!");
    i++;
}, 
{
    stack_push(&cpu_stack, cpu->code.buffer[++i]);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (PUSHA, "pusha", 1,
{}, 
{
    long element = cpu->RAM[cpu->code.buffer[++i]];
    stack_push(&cpu_stack, element);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (POP, "pop", 1,
{
    Token argument = tokens.buffer[i+1]; 
    
    if (argument.type == ADDRESS) {
        push_element(long, token_sequence, POP);
        push_element(long, token_sequence, argument.number);
    }
    else 
        assert(false && "incorrect input!");
    i++;
}, 
{
    long index = cpu->code.buffer[++i];
    cpu->RAM[index] = stack_pop(&cpu_stack);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (ADD, "add", 0,
{    
    push_element(long, token_sequence, ADD);
}, 
{
    stack_push(&cpu_stack, stack_pop(&cpu_stack) + stack_pop(&cpu_stack));
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (MUL, "mul", 0,
{    
    push_element(long, token_sequence, MUL);

}, 
{
stack_push(&cpu_stack, stack_pop(&cpu_stack) * stack_pop(&cpu_stack));
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (SUB, "sub", 0,
{    
    push_element(long, token_sequence, SUB);

}, 
{
    stack_push(&cpu_stack, (-1) * stack_pop(&cpu_stack) + stack_pop(&cpu_stack));
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (DIV, "div", 0,
{    
    push_element(long, token_sequence, DIV);
    
}, 
{
    stack_push(&cpu_stack, 1 / stack_pop(&cpu_stack) * stack_pop(&cpu_stack)); 
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (OUT, "out", 0,
{    
    push_element(long, token_sequence, OUT);

}, 
{
    printf("%ld\n", stack_get_last_element(&cpu_stack));
})




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (IN, "in", 0,
{    
    push_element(long, token_sequence, IN);

}, 
{
    long s;
    if (!scanf("%ld", &s)) return INCORRECT_INPUT;
    stack_push(&cpu_stack, s);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (PRINT, "print", 1,
{    
    Token argument = tokens.buffer[++i]; 
    
    if (argument.type == ADDRESS) {
        push_element(long, token_sequence, PRINT);
        push_element(long, token_sequence, argument.number);
    }
    else 
        assert(false && "incorrect input!");
    
}, 
{
    int index = cpu->code.buffer[++i];
    printf("%d\n", cpu->RAM[index]);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (SCAN, "scan", 1,
{    
    Token argument = tokens.buffer[++i]; 
    
    if (argument.type == ADDRESS) {
        push_element(long, token_sequence, SCAN);
        push_element(long, token_sequence, argument.number);
    }
    else 
        assert(false && "incorrect input!");
    
}, 
{
    long index = cpu->code.buffer[++i];
    scanf("%d", &cpu->RAM[index]);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (JE, "je", 0,
{    
    Token argument = tokens.buffer[++i]; 
    push_element(long, token_sequence, JE);
    if (argument.type == COMMAND) 
        push_element(long, token_sequence, 0);
    else 
        assert(false && "incorrect input!");

}, 
{   
    long first_element = stack_pop(&cpu_stack);
    long second_element = stack_pop(&cpu_stack);

    if (first_element == second_element) {
        i = cpu->code.buffer[++i];

    }
    else
        i++;
    stack_push(&cpu_stack, second_element);
    stack_push(&cpu_stack, first_element+1);

})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (JB, "jb", 0,
{   
    Token argument = tokens.buffer[i+1]; 
    push_element(long, token_sequence, JB);
    if (argument.type == COMMAND) 
        push_element(long, token_sequence, 0);
    else 
        assert(false && "incorrect input!");
    i++;
}, 
{   
    long first_element = stack_pop(&cpu_stack);
    long second_element = stack_pop(&cpu_stack);

    if (first_element < second_element) {
        i = cpu->code.buffer[++i];
    }
    else
        i++;

    stack_push(&cpu_stack, second_element);
    stack_push(&cpu_stack, first_element);

})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (JUMP, "jump", 0,
{    
    Token argument = tokens.buffer[i+1]; 
    push_element(long, token_sequence, JUMP);
    if (argument.type == COMMAND) 
        push_element(long, token_sequence, 0);
    else 
        assert(false && "incorrect input!");
    i++;

}, {i++;})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



DEF_CMD (DUP, "dup", 0,
{    
    push_element(long, token_sequence, DUP);
}, 
{
    long element = stack_get_last_element(&cpu_stack);
    stack_push(&cpu_stack, element);
})



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#undef DEF_CMD



/*
jb <
jbe <=
ja >
jae >=
je ==
jne !=
*/