

#include "myInit.h"

void myInit()
{
	//Input-Pin der den ADC-Betrieb an/ausschaltet
	DDRB &= ~(1<<BIT0);			//B.0 als Eingang definieren (auf 0 setzen)
	PORTB |= (1<<BIT0);			//B.0 Pull-Up aktivieren 


	//Kontroll LED für den Betrieb ON/OFF
	DDRB |= (1<<BIT2);			//B.1 als Ausgang definieren
	PORTB &= ~(1<<BIT2);		//B.1 = OFF	
}
