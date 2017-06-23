/*
 *  ex113.c
 *
 *  Histogram of word length
 */
 
#include <stdio.h>

/* inside a word */
#define IN      1

/* outside a word*/
#define OUT     0

/* max word length */
#define MAX_LEN    12

void main()
{
    int c, i, j, len, max_len, state;
    int wordlen[MAX_LEN];    
    
    state = OUT;
    len = max_len = 0;
    for (i = 0; i < MAX_LEN; ++i) {
        wordlen[i] = 0;
    }
    
    while((c = getchar()) != EOF) {        
        if (c == ' ' || c == '\n' || c == '\t') {
            state = OUT;
            ++wordlen[len-1];
            len = 0;
        }
        else {
            ++len;
            if (len > max_len)
                max_len = len;
            if (state == OUT) {
                state = IN;
            }
        }
    }

    printf("\nhorizontal histogram:\n"); 
    for (i = 0; i < MAX_LEN; ++i) {
        printf("%2d ", i+1);
        for (j = 0; j < wordlen[i]; ++j)
            putchar('*');
        putchar('\n');        
    }
    
    printf("\nvertical histogram:\n");
    for (i = max_len; i > 0; --i) {
        for (j = 0; j < MAX_LEN; ++j) {
            len = wordlen[j];
            putchar(' ');
            if (len >= i)
                putchar('*');
            else
                putchar(' ');
            putchar(' ');
        }
        putchar('\n');
    }
    for (i = 0; i < MAX_LEN; ++i)
        printf("%2d ", i+1);
    putchar('\n');
}
