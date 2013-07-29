//uint16_t steht für einen 16-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 65535. 
//uint8_t steht für einen 8-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 255 

#include <avr/io.h>
#include "lcd-routines.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "timer_init.h"
#include "heiz-routines.h"
#include <avr/sleep.h>


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7 

#define maxHeizStufe 7 //maximale Heizstufe
#define anzahlSchritte 100

volatile uint8_t heizungUpdate=0;
volatile uint8_t buttonWarm=0;
volatile uint8_t lcdIni=0;
	


ISR(INT0_vect) //bei Button "warm", läuft durch wenn fallende Flanke auf INT0 (PD.2)
{
	heizungUpdate=1;
	buttonWarm=1;
	
}


ISR(INT1_vect) //bei Button "kalt", läuft durch wenn fallende Flanke auf INT1 (PD.3)
{
	heizungUpdate=1;
	buttonWarm=0;
	
}


ISR(INT2_vect)
{	
	PORTD |= (1<<BIT7);
	PORTC |= (1<<BIT0);
	PORTC &= ~(1<<BIT1);
	lcdIni=1;
	sleep_disable();	//ISR läuft durch, wenn fallende Flanke auf INT2 (PB.2)
}



int main (void) {            
 
	//Initialisierungen

	heizInit();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	sei();	

	//Variablen
	uint8_t heizStufe=0;
	uint8_t dispPos=0;
	uint8_t tempDispPos=0;

			
	DDRB  |= 0x01;	        //Port A.0 als Ausgang (Kontroll LED grün)
	PORTB |= (1<<BIT0);		//Port A.0 = HIGH

	DDRC  |= (1<<BIT0);		//Port C.0 als Ausgang 
	PORTC |= (1<<BIT0);		//damit wird das Darlington angesteuert um im SleepMode
							//die Masse fürs Display abzuschalten (im SleepMode = 0)

	DDRC  |=(1<<BIT1);		//Port C.1 als Ausgang
							//damit wird das Darlington angesteuert um im SleepMode
							//die Masse auf den Emittor des Transistors zu schalten
							//der die Display-Beleuchtung steuert. (im SleepMode = 1)

	

	lcd_init();
	lcd_home();
	lcd_string("kalt        warm");
	lcdHeizung(dispPos);
	
	DDRA |= (1<<BIT0) | (1<<BIT1) | (1<<BIT2) | (1<<BIT3); //A.0 bis A.3 als Ausgänge für Darlington-Array
														   //Bei Ansteuerung schaltet das Darlington Masse.
	

  while(1)  {                
  

		if (lcdIni==1)
		{
			lcd_init();	
			lcd_init();
			lcd_home();
			lcd_string("kalt        warm");
			for(tempDispPos=0;tempDispPos<=dispPos;tempDispPos++)
			{
				lcdHeizung(tempDispPos);
			}
			lcdIni=0;
		}

		if (heizungUpdate)
		{
			if (buttonWarm)
			{
				if (heizStufe<=maxHeizStufe)
				{	
					dispPos++;
					lcdHeizung(dispPos);
					dispPos++;
					lcdHeizung(dispPos);
					heizStufe++;
					buttonWarm=0;
					motorRechtslauf(anzahlSchritte);
				}
			}
			
			else //buttonKalt=gedrückt
			{
				if (heizStufe!=0)
				{
					lcdKuehlung(dispPos);
					dispPos--;
					lcdKuehlung(dispPos);
					dispPos--;
					heizStufe--;
					motorLinkslauf(anzahlSchritte);
					
				}
			}
			heizungUpdate=0;
		}
		
		if (PIND & (1<<BIT6))	// Zündung=ON  -> Transistor schaltet 0V -> PIND.6 = 0
		{						// Zündung=OFF -> Transistor sperrt      -> PIND.6 = 1 (interner Pull-Up)
			 					// '->Sleep ON
			PORTC &= ~(1<<BIT0);//Display -> Spannungslos (Display-Masse wird abgeschaltet)
			PORTC |= (1<<BIT1); //DisplayBeleuchtung -> spannungslos (über Darlington 0V an TransistorBasis)
			sleep_enable();	   
			sleep_cpu();
		}					   
   
   }	
   return 0;                 
}
