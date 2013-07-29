/*	Dieses c-File stellt die PWM-Funktionalität für den Atmega32 zur Verfügung
/	
/   Genutzt wird der 16Bit Timer1
/	
/	Der PWM-Ausgabe-Pin ist -bei KanalA -> D.5 (OC1A)
/							-bei KanalB -> D.4 (OC1B)
/
/
/	Mit Übergabe des Parameters "compare" wird der Duty-Cycle eingestellt.
/	
/	100% => compare = 1023
/
/	Um den Duty-Cycle aus dem laufenden Betrieb zu modifizieren muss,
/	je nach Kanal A oder B das folgende Register mit einem Werte
/	zwischen 0 & 1023 beschrieben werden
/
/	Kanal A -> OCR1A
/	Kanal B -> OCR1B
/
/
*/

#include <avr/io.h>
#include "pwm_init.h"

void pwm_init(uint16_t compare)
{
	DDRD |= (1<<BIT4) | (1<<BIT5);		//D.4&5 als Ausgänge definieren

	TCCR1A |= (1<<WGM10) | (1<<WGM11);	//PWM, Phase Correct, 10-bit
										//TOP: 0x03FF=1023
										
									
	//Der Timer/Counter zählt nun permanent von 0 bis zur 1023 und wieder zurück. 
	//100 % Duty Cycle sind 2046 Zählschritte. 
	//Bei Systemtakt=4MHz -> PWM-Frequenz=1,955 KHz => 0,5115ms									
	

	
	TCCR1A |= (1<<COM1A1); 	//Nicht invertierende PWM, KanalA -> auf Pin D.5
							//- Clear OC1A/OC1B on Compare Match when up-counting. 
							//- Set OC1A/OC1B on Compare Match when downcounting.


	TCCR1B |= (1<<CS10); //Prescaler nicht aktiviert - es liegt der Systemtakt an


/*	Prescaler-Settings: (noch vom Atmega8, also Achtung)
	
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
