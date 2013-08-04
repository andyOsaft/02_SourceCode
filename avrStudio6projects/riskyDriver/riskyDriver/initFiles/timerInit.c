


#include "timerInit.h"

void timer_init(uint16_t compare)
{

	TCCR1B |= (1<<WGM12);	//CTC Mode:
							//Timer1 zählt hoch, bei compare match wird er wieder zu 0 gesetzt

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
