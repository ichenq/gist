/*
 *  ex105.c
 *
 *  Fahrenheit-Celsius table with inverted sequence
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
        
    printf("Celsius-Fahrenheit table\n");   /* title */
       
    for (fahr = upper; fahr >= 0; fahr -= step) {
        celsius = (5.0/9.0) * (fahr-32.0);
        printf("%3.0f %6.1f\n", fahr, celsius);
    }
}
