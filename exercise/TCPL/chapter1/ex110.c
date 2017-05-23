/*
 *  ex110.c
 *
 *  replace '\t', '\b', '\\' to visible string
 */
 
#include <stdio.h>

void main()
{
    int c;
    
    while((c = getchar()) != EOF) {        
        if (c == '\t') {
            putchar('\\');
            putchar('t');
        }
        else if (c == '\b') {
            putchar('\\');
            putchar('b');
        }
        else if (c == '\\') {
            putchar('\\');
            putchar('\\');
        }
        else
            putchar(c);
    }
}
