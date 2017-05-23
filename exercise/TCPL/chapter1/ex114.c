/*
 *  ex113.c
 *
 *  Histogram of characters
 */
 
#include <stdio.h>

#define MAX_CHAR    128

void main()
{
    int c, i, j, len;
    int histogram[MAX_CHAR];
    
    for (i = 0; i < MAX_CHAR; ++i)
        histogram[i] = 0;
    
    while((c = getchar()) != EOF) {        
        if ((c >= '0' && c <= '9') 
            || (c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'))
            ++histogram[c];
    }

    printf("\nhorizontal histogram:\n"); 
    for (i = 0; i < MAX_CHAR; ++i) {
        len = histogram[i];
        if (len > 0) {
            printf("%-2c", i);
            for (j = 0; j < histogram[i]; ++j)
                putchar('*');
            putchar('\n');
        }
    }
}
