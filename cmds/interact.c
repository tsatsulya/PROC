DEF_CMD(OUT, {}, {
    printf("%lld", stack_get_last_element(cpu->stack));
})

DEF_CMD(IN, {}, {
    int input = 0;
    scanf("%d", &input);
    stack_push(cpu->stack, input);
})