/*
 *  ex112.c
 *
 *  print a word each line
 */
 
#include <stdio.h>

/* inside a word */
#define IN      1

/* outside a word*/
#define OUT     0

void main()
{
    int c, nw, state;
    
    state = OUT;
    nw = 0;
    
    while((c = getchar()) != EOF) {        
        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else {
            if (state == OUT) {
                state = IN;
                if (nw > 0)
                    putchar('\n');
                ++nw;
            }
            putchar(c);
        }
    }    
}
