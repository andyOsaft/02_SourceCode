/*	Dieses c-File stellt die PWM-Funktionalität für den Atmega32 zur Verfügung
/	
/   - Genutzt wird der 8Bit Timer0
/	- Nicht-invertierende PWM:
/			|-> Timer0 zählt von 0 bis 255
/			|-> Treffen des Vergleichswertes beim Hochzählen 	-> OC0=Low
/			|-> Treffen des Vergleichswertes beim Runterzählen 	-> OC0=High
/
/						   f_OSC
/	- PWM-Frequenz:  f = ---------    (N=Prescaler-Werte); 510=2*255 hoch&runterzählen
/						   N*510
/
/
/
/														        gesuchte_Zeit_für_High_Pegel
/   - Vergleichswert für Duty-Cycle (OCR0-value):  Compare = ------------------------------------   
/															            T_OSC * N	
/
/
/
/	- Der PWM-Ausgabe-Pin ist OC0 -> PinB.3
/   
/
/	- f_OSC = 8MHz
/							
*/

#include <avr/io.h>
#include "pwm_init.h"

void pwm_init(void)
{

	//B.3 als Ausgang definieren = PWM Output Pin
	DDRB |= (1<<BIT3);			


	//PWM, Phase Correct, 8-bit
	//TOP: 255
	//Update of OCR1x: TOP
	//Timer Overflow Flag1 set on: BOTTOM
	//Der Timer/Counter zählt nun permanent von 0 bis zur 255 und wieder zurück. 
	TCCR0 |= (1<<WGM00);	
							


	//Nicht invertierende PWM
	//- Clear OC0 on Compare Match when up-counting. 
	//- Set OC0 on Compare Match when downcounting.
	TCCR0 |= (1<<COM01); 


    //Prescaler auf 256 setzen:
	TCCR0 |= (1<<CS02);

	
/*	Prescaler-Settings:
	
 CS02/CS01/CS00
	0 	0 	0 	 Stop. Der Timer/Counter wird gestoppt.
	0   0 	1 	CK
	0 	1 	0 	CK / 8
	0 	1 	1 	CK / 64
	1 	0 	0 	CK / 256
	1 	0 	1 	CK / 1024
	1 	1 	0 	Externer Pin 1, negative Flanke
	1 	1 	1 	Externer Pin 1, positive Flanke 
*/
	
	
	
}

void pwm_update(int8_t compare)
{
	OCR0=compare;	//Vergleichswert laden
}
