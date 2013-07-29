
#include <avr/io.h>
#include "lcd-routines.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "timer_init.h"


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7 



volatile uint8_t k=0;

ISR(TIMER0_COMP_vect)
{
	k++;	//Laufvaribale für Sekunden-Erkennung
			
			//Sleep Mode = OFF

}




int main (void) {            
 	
	DDRA  |= 0x01;		//Port A.0 als Ausgang -> System Status LED
	PORTA |= (1<<BIT0);  //Port A.0 = High

	lcd_init();			//LCD initialisieren
	lcd_home();		
	lcd_string("UHRZEIT");	
	
	
	uint8_t compare_wert = 217;
	timer_init(compare_wert);   //Timer0 initialisieren

	uint8_t s=0,m=0,h=0;	//Sekunden, Minuten Stunden 
	char buffer[9]="00:00:00";	//buffer für Übergabe an LCD-Anzeige-Funktion
	    

	set_cursor(0,2);
	lcd_string(buffer);

	sei(); //Global Interrupt Enable



	while(1) {                
		

		if (k==18) //1 Sekunde vorbei
		{	
			
			k=0;
			
			cli();
			if(m<60)//1 Stunde vorbei?
			{
				if (s<60)//1 Minute vorbei?
				{	
					if (s<10)
					{
						*(buffer+6)='0'; //Sekunden-Zehnerstelle zurücksetzen 
						utoa(s,buffer+7,10);							
						set_cursor(0,2);
						lcd_string(buffer);
						s++;
					}	
					else
					{
						utoa(s,buffer+6,10);	//Wandeln in String, 10->Dezimalsystem
						set_cursor(0,2);
				   		lcd_string(buffer);
						s++;
					}		
				}
				else //1 Minute vorbei
				{
						s=0;
						*(buffer+6)='0'; //Sekunden-Zehnerstelle zurücksetzen 
						utoa(s,buffer+7,10);							
						set_cursor(0,2);
						lcd_string(buffer);
						s++;
						m++;
											
						if (m<10)
						{
							*(buffer+3)='0';
							utoa(m,buffer+4,10);
							set_cursor(0,2);
							lcd_string(buffer);
						}
						else				
						{
							utoa(m,buffer+3,10);	//Wandeln in String, 10->Dezimalsystem
							set_cursor(0,2);
				   			lcd_string(buffer);
						}
				}	
			}	
			else //1 Stunde vorbei
			{
				m=0;
				s=0;
				*(buffer+6)='0'; //Sekunden-Zehnerstelle zurücksetzen 
				utoa(s,buffer+7,10);							
				set_cursor(0,2);
				lcd_string(buffer);
				s++;

				*(buffer+3)='0';
				utoa(m,buffer+4,10);
				set_cursor(0,2);
				lcd_string(buffer);
				m++;
				h++;
				if (h>23)
				{	
					h=0;
				}
												
				if (h<10)
				{
					*(buffer)='0';
					utoa(h,buffer+1,10);
					set_cursor(0,2);
					lcd_string(buffer);					
				}
				else				
				{	
					utoa(h,buffer,10);	//Wandeln in String, 10->Dezimalsystem
					set_cursor(0,2);
				   	lcd_string(buffer);

				}
			}
			
			sei();					//Sleep-Mode = ON	-> wenn k!=18	
		}
	}	
	return 0;                 
}











