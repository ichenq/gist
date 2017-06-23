/*
 *  ex106.c
 *
 *  print value of (getchar() != EOF)
 */
 
#include <stdio.h>

void main()
{
    int c = (getchar() != EOF);
    printf("value of (getchar() == EOF): %d\n", c);
}
