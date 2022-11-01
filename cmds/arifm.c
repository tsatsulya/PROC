#define ARIFMETIC_OPERATION(operation) {                    \
    int first = pop(cpu->cpu_stack);                        \
    int second = pop(cpu->cpu_stack);                       \
    stack_push(cpu->stack, first operation second);         \
}

DEF_CMD(ADD, {}, __STACK_ELEM_OPERATION(+))

DEF_CMD(SUB, {}, __STACK_ELEM_OPERATION(-))

DEF_CMD(MUL, {}, __STACK_ELEM_OPERATION(*))

DEF_CMD(DIV, {}, __STACK_ELEM_OPERATION(/))

#undef ARIFMETIC_OPERATION