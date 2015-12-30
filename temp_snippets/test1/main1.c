/* main.c */
#include <stdio.h>

void push(char);
char pop(void);
int is_empty(void);

int a, b = 1;

int main(void)
{
    push('a');
    push('b');
    push('c');

    while(!is_empty())
        putchar(pop());
    putchar('\n');

    return 0;
}

