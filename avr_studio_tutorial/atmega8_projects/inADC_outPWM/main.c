/*  Projektziele:
/	- Per ADC wird ein Wert zwischen 0V und 5V eingelesen. Daraus ergibt sich ein Prozentwert zwischen 0 und 100%.
/ 	- An einem Ausgang soll ein PWM Signal erzeugt werden, dessen Duty Cycle sich aus dem eingelesenen Prozentwert ergibt.
/   - Die Prozentangabe soll auf dem Display ausgegeben werden.  
/   - ADC & PWM Realisierung über Interrupts
/ 	- Zur Veranschaulichung soll der ADC-Wert in Zeile 1 des Displays ausgegeben werden
/	- In Zeile 2 Soll der prozentuale PWM-Wert ausgegeben werden
*/ 
#include <avr/io.h>
#include "lcd-routines.h"
#include <stdlib.h>
#include "adc_init.h"
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "debounce.h"
#include "pwm_init.h"


volatile double adc_wert;		//Wert der vom ADC kommt
volatile double pwm_wert;		//Compare Wert für die PWM-Generierung

ISR(ADC_vect) //ISR für ADC Conversion Complete
{
	adc_wert = ADCW;
	pwm_wert = adc_wert;				
	
}




int main(void)
{
	cli();
	//Variablen
	int8_t LCD_an=0, LCD_aus=0;		//Variablen zum Aktualisieren des LCD -> Eine davon könnte durch optimieren weggelassen werden 
	int8_t adc_kanal; 				//Variable zum Anwählen des gewünschten ADC Kanals
	int8_t ADC_an=0;				//Variable für Betrieb An/Aus (getoggelt über Schalter einlesen)
	char buffer[10],buffer2[10];	//Buffer für die Ausgabe aufs LCD -> Bei gelegenheit mit pointer und malloc optimieren
	

	//Initialisierungen
	lcd_init();
 	DDRB &= ~(1<<BIT0);			//B.0 als Eingang definieren (auf 0 setzen)
	PORTB |= (1<<BIT0);			//B.0 Pull-Up aktivieren 
	
	adc_kanal = 0;				//ADC-Kanal0
	adc_init(adc_kanal);
	

	//Kontroll LED für den Betrieb ON/OFF
	DDRB |= (1<<BIT2);			//B.1 als Ausgang definieren
	PORTB &= ~(1<<BIT2);		//B.1 = OFF	
	

    while(1)
    {
		

		if(debounce(PINB,PB0))
			ADC_an = !ADC_an;  //An/Aus Betrieb toggeln
						
		if (ADC_an == 1)
			PORTB |= (1<<BIT2); //Wenn ADC=ON -> LED=ON
		else
			PORTB &= ~(1<<BIT2);//Else LED=OFF
	
		
		

		if ( ADC_an ) //führe Aktion aus wenn Port B.0 = low ist (Schalter aktiv)
			{	
				sei();
				if (LCD_an == 0)  //LZum einmaligen Löschen des Displays beim durchlaufen dieses If-Zweiges
				{
					lcd_clear();
					LCD_an = 1;
					LCD_aus=0;
				}
				lcd_home();
				lcd_string("ADC an: ");
				set_cursor(9,1);
				adc_wert = adc_wert * 0.005;
				dtostrf(adc_wert,5,3,buffer);
				lcd_string(buffer);
				pwm_init(adc_wert);
				set_cursor(0,2);
				lcd_string("Cycle:   ");
				dtostrf(pwm_wert,4,0,buffer2);
				lcd_string(buffer2);
			}	

		else
			{
				cli();
				if (LCD_aus == 0)  //Zum einmaligen Löschen des Displays beim durchlaufen dieses If-Zweiges
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




















