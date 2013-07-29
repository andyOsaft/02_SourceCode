

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/sleep.h>



int main()
{
	DDRA = 0xFF;
	while (1)
	{
		PORTA = ~PINA;
		//nop();  // added some nop, to slow the clock a bit
		//nop();
	}
}
