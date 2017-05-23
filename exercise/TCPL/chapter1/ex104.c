/*
 *  ex104.c
 *
 *  Celsius-Fahrenheit table
 */
 
#include <stdio.h>

/* print Celsius-Fahrenheit table */
void main()
{
    float fahr, celsius;
    float lower, upper, step;
    
    lower = 0;      /* lower limit of temperatuire scale */
    upper = 300;    /* upper limit */
    step = 30;      /* step size */
    
    printf("Celsius-Fahrenheit table\n"); 
    celsius = lower;
    while (celsius <= upper) {        
        fahr = (9.0/5.0) * celsius + 32;
        printf("%3.0f %6.0f\n", celsius, fahr);
        celsius = celsius + step;
    }
}
