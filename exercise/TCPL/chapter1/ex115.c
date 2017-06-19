/*
 *  ex105.c
 *
 *  Fahrenheit-Celsius table with subroutine
 */
 
#include <stdio.h>

float celsius_to_fahrenheit(float celsius);

/* print Celsius-Fahrenheit table */
void main()
{
    float celsius;
    float lower, upper, step;
    
    lower = 0;      /* lower limit of temperatuire scale */
    upper = 300;    /* upper limit */
    step = 30;      /* step size */
        
    printf("Celsius-Fahrenheit table\n"); /* title */
    
    celsius = lower;
    while (celsius <= upper) {        
        printf("%3f %6f\n", celsius, celsius_to_fahrenheit(celsius));
        celsius = celsius + step;
    }
}

float celsius_to_fahrenheit(float celsius)
{
    return (9.0/5.0) * celsius + 32;
}