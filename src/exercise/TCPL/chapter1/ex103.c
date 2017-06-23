/*
 *  ex103.c
 *
 *  print Fahrenheit-Celsius table with a title
 */
 
#include <stdio.h>

/* print Fahrenheit-Celsius table */
void main()
{
    float fahr, celsius;
    float lower, upper, step;
    
    lower = 0;      /* lower limit of temperatuire scale */
    upper = 300;    /* upper limit */
    step = 30;      /* step size */
        
    printf("Fahrenheit-Celsius table\n");   /* title */
    
    fahr = lower;
    while (fahr <= upper) {
        celsius = (5.0/9.0) * (fahr-32.0);
        printf("%3.0f %6.1f\n", fahr, celsius);
        fahr = fahr + step;
    }
}
