/*
 *  ex108.c
 *
 *  count white, tabs and newlines
 */
 
#include <stdio.h>

void main()
{
    int c, nwhite, ntab, nnewline;
    
    nwhite = 0;
    ntab = 0;
    nnewline = 0;
    
    while((c = getchar()) != EOF) {
        if (c == ' ')
            ++nwhite;
        else if (c == '\t')
            ++ntab;
        else if (c == '\n')
            ++nnewline;
    }
    printf("\nwhite: %d\ntabs: %d\nnewlines: %d\n", nwhite, ntab, nnewline);
}
