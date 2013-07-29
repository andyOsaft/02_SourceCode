/*  Projektziele:
	- Aufbau einer Test-Station für die Sharp IR Sensoren der GP2Dxxx serie
	- Sensor wird auf ein Servo montiert das von 1ms bis 2ms "rotiert"
	- Der Drehbereich des Servos wird in x Teile geteilt.
	- Für jeden Teilbereich wird eine Sensor-Messung vorgenommen die dann in ein Array gespeichert wird
	- Parallel werden die Sensor-Daten auf dem LCD-Display ausgegben. 

*/ 
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

#include "adcInit.h"
#include "debounce.h"
#include "pwmInit.h"
#include "myInit.h"
#include "lcdRoutines.h"

volatile uint16_t adcWert;		//Wert der vom ADC kommt
volatile uint16_t pwmWert;		//Compare Wert für die PWM-Generierung

ISR(ADC_vect) //ISR für ADC Conversion Complete
{
	uint32_t tempValue=0;
	tempValue = ADCW;
	tempValue = tempValue * (uint32_t) 500;
	tempValue = tempValue / (uint32_t) 1023;
	//adc-wert * 100 wird gespeichert (5V als Referenz-Spannung)
	adcWert = tempValue;
	
	//pwm-Wert für Duty Cycle wird gespeichert
	pwmWert = ADCW;				
}




int main(void)
{

	//Interrupts abschalten
	cli();
	

	//Variablen
	int8_t LCD_an=0, LCD_aus=0;		//Variablen zum Aktualisieren des LCD -> Eine davon könnte durch optimieren weggelassen werden 
	int8_t adc_kanal; 				//Variable zum Anwählen des gewünschten ADC Kanals
	int8_t ADC_an=0;				//Variable für Betrieb An/Aus (getoggelt über Schalter einlesen)
	char buffer[10];	
	
	
	//Initialisierungen
	myInit();
	lcd_init();
	pwmInit(31);

	
	adc_kanal = 0;				//ADC-Kanal0 -> Pin C.0
	adc_init(adc_kanal);
	
	//Interrupts aktivieren


	

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
				if (LCD_an == 0)  //Zum einmaligen Löschen des Displays beim durchlaufen dieses If-Zweiges
				{
					lcd_clear();
					LCD_an = 1;
					LCD_aus = 0;
				}
				lcd_home();
				//erste Zeile ausgeben
				lcd_string("ADC an:  ");
				set_cursor(9,1);
				//Vorkommastelle
				itoa((adcWert/100), buffer,10);
				lcd_string(buffer);
				lcd_string(".");
				//Nachkommastelle
				itoa((adcWert%100), buffer, 10);	
				lcd_string(buffer);
				lcd_string("   ");			

				//zweite Zeile ausgebens
				set_cursor(0,2);
				lcd_string("Cycle:   ");
				set_cursor(9,2);
				itoa(pwmWert,buffer, 10);
				lcd_string(buffer);
				lcd_string("   ");
				
				//PWM aktualisieren
				if (pwmWert<31)
					pwmUpdate(31);
				else if (adcWert>63)
					pwmUpdate(63);
				else	
					pwmUpdate(pwmWert);
				sei();
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
				sei();
			}
    }
 
    return 0;
}




















