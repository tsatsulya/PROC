#define _ ,

DEF_CMD(1, POP, {}, {
    stack_pop(cpu->cpu_stack);
    int i = 0 _ j = 0;
    for (; i < j; ++ j)
        foo();
})

DEF_CMD (PUSH,
{
    if (!CPU.code_[++RIP_])
        CPU.stk_.push (CPU.code_ [++RIP_], &error);
    else
        CPU.stk_.push (CPU.Regs_ [CPU.code_ [RIP_] >> 16], &error);
},
{
    token = (data->tokens)[++token_id];
    token_sequence[] = CMD_PUSH_;

    if (tok.type == NUM_)
    {
        binCode[binShift++] = 0;
        binCode[binShift++] = tok.number;
    }

    else if (tok.type == TERM_ && tok.name[0] == 'R')
    {   
        binCode[binShift++] = 1 + ((tok.id - TR_R00_) << 16);
    }

    else
        CRITICAL_ERR (true, SYNT_ERR, binCode, nullptr);
})

DEF_CMD(DUP, {}, {
    stack_push((stack_get_last_element(cpu->cpu_stack));
})

DEF_CMD(VSET, {}, {
    int elem1 = stack_pop(cpu->cpu_stack);
    int elem2 = stack_pop(cpu->cpu_stack);
    cpu->equal_flag = elem1 == elem2;
    cpu->above_flag = elem1 > elem2;
    cpu->less_flag = elem1 < elem2;
})
