//Initialierungsfunktion für Timer0

#include <avr/io.h>
#include "timer_init.h"
#include <util/delay.h>
#include <stdint.h>


void timer_init(uint8_t compare_wert)
{
	//TCCR0 = 0xD;
	TCCR0 |= ((1<<WGM01) | (1<<CS02) | (1<<CS00)); 	// - WGM01: Clear Timer on Compare Match Mode (CTC-Mode)
						 							// - Clock: Clk_sys / 1024


	OCR0 = compare_wert;	//Compare Register mit übergebenen Wert laden.
	
	TIMSK |= (1<<OCIE0);  //Compare Match Interrupt Enable
	
}
