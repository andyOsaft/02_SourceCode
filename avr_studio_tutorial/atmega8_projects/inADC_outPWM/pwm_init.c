/*	Dieses c-File stellt die PWM-Funktionalität für den Atmega8 zur Verfügung
/	
/   Genutzt wird der 16Bit Timer1
/	
/	Der PWM-Ausgabe-Pin ist -bei KanalA -> B.1
/							-bei KanalB -> B.2
*/

#include <avr/io.h>
#include "pwm_init.h"

void pwm_init(double compare)
{
	DDRB |= (1<<BIT1);			//B.1 als Ausgang definieren

	TCCR1A |= (1<<WGM10) | (1<<WGM11);	//PWM, Phase Correct, 10-bit
									//TOP: 0x03FF
									//Update of OCR1x: TOP
									//Timer Overflow Flag1 set on: BOTTOM
									
	//Der Timer/Counter zählt nun permanent von 0 bis zur 1023 und wieder zurück. 
	//100 % Duty Cycle sind 2046 Zählschritte. 2046*0,271*10^-6 s = 0,555 ms (ohne Vorteiler)									
	


	TCCR1A |= (1<<COM1A1); 	//Nicht invertierende PWM, KanalA -> auf Pin B.1
							//- Clear OC1A/OC1B on Compare Match when up-counting. 
							//- Set OC1A/OC1B on Compare Match when downcounting.


	//TCCR1B |= (1<<CS12) | (1<<CS10); //Prescaler auf 1024 -> 2046 Zählschritte = 0,56s;
	TCCR1B |= (1<<CS10); //Prescaler auf 64


/*	Prescaler-Settings:
	
 CS12/CS11/CS10
	0 	0 	0 	 Stop. Der Timer/Counter wird gestoppt.
	0   0 	1 	CK
	0 	1 	0 	CK / 8
	0 	1 	1 	CK / 64
	1 	0 	0 	CK / 256
	1 	0 	1 	CK / 1024
	1 	1 	0 	Externer Pin 1, negative Flanke
	1 	1 	1 	Externer Pin 1, positive Flanke 
*/
	
	OCR1A=compare;	//Vergleichswert laden
	
	
}
