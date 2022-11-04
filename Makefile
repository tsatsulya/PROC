
debug:
	${CC} -I./header main.c source/tokenization.c source/asm.c source/utils.c source/cpu.c source/stack.c source/strings.c -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -DDEBUG -ggdb3 -O0 -Og -Wall -Wextra -o debug

release:
	${CC} -I./header main.c source/tokenization.c -O3 -Wall -Wextra -Werror -o release
