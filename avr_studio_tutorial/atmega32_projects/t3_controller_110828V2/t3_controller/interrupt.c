/*********************************************************************************************

Interrupt-Vektoren sind definiert in Header-File "iom32.h"


**********************************************************************************************/

#include "interrupt.h"
#include "myInit.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/sleep.h>
#include "global.h"



//bei Button "warm" (button ganz rechts), l�uft durch wenn fallende Flanke auf INT0 (PD.2)
ISR(INT0_vect) 
{
//	buttonRechts=0;	
}

//----------------------------------------------------------------------------------------------

//bei Button "kalt" (button in der Mitte), l�uft durch wenn fallende Flanke auf INT1 (PD.3)
ISR(INT1_vect) 
{
//	buttonMitte=0;	
}

//----------------------------------------------------------------------------------------------

//ISR l�uft durch, wenn fallende Flanke auf INT2 (PB.2)
ISR(INT2_vect)
{	

	

	//Display wieder anschalten.
	PORTC |= (1<<BIT0);
	PORTC &= ~(1<<BIT1);

	sleep_disable();	
	
	//Variable setzen um zu das Aufwachen aus sleepmode zu signalisieren
//	wakeUp=1;
}

