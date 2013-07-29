/*
 * DifferentialAdcTest.c
 *
 * Created: 17.03.2013 19:23:34
 *  Author: xxxburns
 */ 


#include <avr/io.h>
#include "adc.h"

int main(void)
{
    uint16_t adcResult = 0;
    
    adcInit();
    while(1)
    {
         adcResult = adcRead(1);
    }
}