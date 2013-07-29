
#include "myInit.h"

/*--------------------------------------------------------------------------------------

Funktion:
			- f�hrt Initialisierungen durch
			- wird bei Systemstart ausgef�hrt

--------------------------------------------------------------------------------------*/
void myInit()
{

	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);


	//f�r dogm display
	//initialize lcd-display
	//lcd_init();
	
	
	
	//cpu status led
	DDRA |= (1<<PINA7);
	PORTA &= ~(1<<PINA7);


	
/*
	//Port C.0 als Ausgang
	//damit wird das Darlington angesteuert um im SleepMode
	//die Masse f�rs Display abzuschalten (im SleepMode = 0)
	DDRC  |= (1<<BIT0);		 
	PORTC |= (1<<BIT0);		
							

	//Port C.1 als Ausgang
	//damit wird das Darlington angesteuert um im SleepMode
	//die Masse auf den Emittor des Transistors zu schalten
	//der die Display-Beleuchtung steuert. (im SleepMode = 1)
	DDRC  |=(1<<BIT1);		
													
															   

	//PB.2 erh�llt 0V vom Darlington Array wenn die Z�ndung an ist -> weckt den uC aus dem Sleep Mode auf.
	//PB2 als Input
	//Pull-Up aktiv
	DDRB  &= ~(1<<BIT2);
	PORTB |= (1<<BIT2);  


	//PD.0 als Input
	//Liest den Schalter links f�r die Men�-Steuerung
	//Pull-Up aktiv
	DDRD &= ~(1<<BIT0);
	PORTD |= (1<<BIT0);	


	//Port D.6 als Eingang (Pr�ft ob die Z�ndung noch an ist)	
	//Wenn Z�ndung an, dann 0V an D.6 (durch Transistor)
	//Wenn Z�ndung aus, dann 5V �ber internen Pull-Up
	DDRD  &= ~(1<<BIT6);	
	PORTD |= (1<<BIT6);		
							
	//PD7 als Output (System-LED gr�n)
	//PD7 = OFF
	DDRD  |= (1<<BIT7); 
	PORTD &= ~(1<<BIT7);

	
	//Microcontroller unit control register:
	//Falling edge of INT1 generates an interrupt request
	//Falling edge of INT0 generates an interrupt request
	//MCUCR |= (1<<BIT3) | (1<<BIT1); 
									

	//General interrupt control register:
	//Enable interrupts for INT0, INT1, INT2	
	//GICR  |= (1<<BIT7) | (1<<BIT6) | (1<<BIT5); 
	GICR |= (1<<BIT5); //only INT2

*/
}







