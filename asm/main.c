#include <stdio.h>
#include "tokenization.h"

int main() {

    Token* tokens;
    int num_of_tokens;
    if (commands[0]) {}
    status status_ = tokenize(&tokens, &num_of_tokens, "test.txt");
    printf("%d", status_);
    return 0;
}