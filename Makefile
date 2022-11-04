
debug:
	${CC} -I./header main.c source/tokenization.c source/asm.c source/utils.c source/cpu.c source/stack.c source/strings.c -DDEBUG -ggdb3 -O0 -Og -Wall -Wextra -o debug

release:
	${CC} -I./header main.c source/tokenization.c -O3 -Wall -Wextra -Werror -o release
