/*  Projektziele:
/	- Per ADC wird ein Wert zwischen 0V und 5V eingelesen. Daraus ergibt sich ein Prozentwert zwischen 0 und 100%.
/ 	- An einem Ausgang soll ein PWM Signal erzeugt werden, dessen Duty Cycle sich aus dem eingelesenen Prozentwert ergibt.
/   - Die Prozentangabe soll auf dem Display ausgegeben werden.  
/   - Realisierung �ber Interrupts
/ 
*/ 
#include <avr/io.h>
#include "lcd-routines.h"
#include <stdlib.h>
#include "adc_single.h"
#include <util/delay.h>
#include <avr/interrupt.h> 


int main(void)
{
	//Variablen
	int8_t LCD_an=0, LCD_aus=0;		//Variablen zum Aktualisieren des LCD -> Eine davon k�nnte durch optimieren weggelassen werden 
	int8_t adc_kanal; 				//Variable zum Anw�hlen des gew�nschten ADC Kanals
	int8_t ADC_an=0; 				//Variable f�r Betrieb An/Aus (getoggelt �ber Schalter einlesen)
	float adc_wert;					//Wert der vom ADC kommt
	char buffer[20];				//Buffer f�r die Ausgabe aufs LCD -> Bei gelegenheit mit pointer und malloc optimieren
	
	//Initialisierungen
	lcd_init();
 	adc_kanal = 0;				//ADC-Kanal0
	DDRB &= ~(1<<BIT0);			//B.0 als Eingang definieren (auf 0 setzen)
	PORTB |= (1<<BIT0);			//B.0 Pull-Up aktivieren
	

    while(1)
    {
		
		if ( !(PINB & (1<<BIT0)) )
		{	
			ADC_an = !ADC_an;  //An/Aus Betrieb toggeln
		}
		

		if ( ADC_an ) //f�hre Aktion aus wenn Port B.0 = low ist (Schalter aktiv)
			{	
				
				if (LCD_an == 0)  //LZum einmaligen L�schen des Displays beim durchlaufen dieses If-Zweiges
				{
					lcd_clear();
					LCD_an = 1;
					LCD_aus=0;
				}
				lcd_home();
				lcd_string("ADC an");
				set_cursor(0,2);
				adc_wert = ReadChannel(adc_kanal); //Einmalige AD-Wandlung auf "Kanal"
  				adc_wert = adc_wert * 0.005;
				dtostrf(adc_wert,5,3,buffer);
				lcd_string(buffer);
			}	
		
			{
				if (LCD_aus == 0)  //Zum einmaligen L�schen des Displays beim durchlaufen dieses If-Zweiges
				{
				{
					lcd_clear();
					LCD_aus = 1;
					LCD_an = 0;
				}
				lcd_home();
				lcd_string("ADC aus");
				}
			}
    }
 
    return 0;
}
