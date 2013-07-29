/*  Projektziele:
/	- Per ADC wird ein Wert zwischen 0V und 5V eingelesen. Daraus ergibt sich ein Prozentwert zwischen 0 und 100%.
/ 	- An einem Ausgang soll ein PWM Signal erzeugt werden, dessen Duty Cycle sich aus dem eingelesenen Prozentwert ergibt.
/   - Die Prozentangabe soll auf dem Display ausgegeben werden.  
/   - ADC & PWM Realisierung über Interrupts
/ 	- Zur Veranschaulichung soll der ADC-Wert in Zeile 1 des Displays ausgegeben werden
/	- In Zeile 2 Soll der prozentuale PWM-Wert ausgegeben werden
*/ 
#include <avr/io.h>

#include <stdlib.h>

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7




int main(void)
{
	

	//Initialisierungen



	//Kontroll LED für den Betrieb ON/OFF
	DDRB |= (1<<BIT2);			
	PORTB &= ~(1<<BIT2);		
	

    while(1)
    {
		
	PORTB ^= (1<<BIT2);	

		
    }
 
    return 0;
}




















