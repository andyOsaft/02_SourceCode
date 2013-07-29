
#include "myInit.h"

/*--------------------------------------------------------------------------------------

Funktion:
			- führt Initialisierungen durch
			- wird bei Systemstart ausgeführt

--------------------------------------------------------------------------------------*/
void myInit()
{

	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);


	//für dogm display
	//initialize lcd-display
	//lcd_init();
	
	
	
	//cpu status led
	DDRA |= (1<<PINA7);
	PORTA &= ~(1<<PINA7);


	
/*
	//Port C.0 als Ausgang
	//damit wird das Darlington angesteuert um im SleepMode
	//die Masse fürs Display abzuschalten (im SleepMode = 0)
	DDRC  |= (1<<BIT0);		 
	PORTC |= (1<<BIT0);		
							

	//Port C.1 als Ausgang
	//damit wird das Darlington angesteuert um im SleepMode
	//die Masse auf den Emittor des Transistors zu schalten
	//der die Display-Beleuchtung steuert. (im SleepMode = 1)
	DDRC  |=(1<<BIT1);		
													
															   

	//PB.2 erhällt 0V vom Darlington Array wenn die Zündung an ist -> weckt den uC aus dem Sleep Mode auf.
	//PB2 als Input
	//Pull-Up aktiv
	DDRB  &= ~(1<<BIT2);
	PORTB |= (1<<BIT2);  


	//PD.0 als Input
	//Liest den Schalter links für die Menü-Steuerung
	//Pull-Up aktiv
	DDRD &= ~(1<<BIT0);
	PORTD |= (1<<BIT0);	


	//Port D.6 als Eingang (Prüft ob die Zündung noch an ist)	
	//Wenn Zündung an, dann 0V an D.6 (durch Transistor)
	//Wenn Zündung aus, dann 5V über internen Pull-Up
	DDRD  &= ~(1<<BIT6);	
	PORTD |= (1<<BIT6);		
							
	//PD7 als Output (System-LED grün)
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







