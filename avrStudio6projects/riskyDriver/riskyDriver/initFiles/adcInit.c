	//Dieses Source-File dient zur vereinfachten Nutzung des integrierten ADCs des ATmega8
//Der A/D Wandler verf�gt �ber 6 Kan�le. Diese liegen auf auf PortC.0 bis PortC.5
//

#include "adcInit.h"

void adcInit()
{
  
  uint16_t result;
 									
								
  ADMUX &= ~((1<<REFS1) | (1<<REFS0)); // externe Referenzspannung nutzen (typischerweise Vcc-Pegel)


  ADCSRA |= ((1<<ADEN) | (1<<ADPS2) | (1<<ADPS1));    	// ADC Prescaler w�hlen. BIT0 bis BII 2 in ADCSRA
  													// ADC-Takt muss im Bereich 50 - 200 kHz liegen
													// ---> Prescaler = 32 ---> ADC-Takt = 115 kHz
                               					    // UND ADC aktivieren



  //Erste conversion ansto�en, danach l�uft der ADC im free run mode
  ADCSRA |= (1<<ADSC);	
  

  //Auf Abschluss der Konvertierung warten
  //Das Register ADCSRA wird mit "1<<ADSC" maskiert. Da ADSC den Wert 6 hat, wartet das while 
  //also solange, bis das Bit6 von ADCSRA == 0 wird.
  while (ADCSRA & (1<<ADSC) ) {}
  	
  //ADCSRA |= (1<<ADIE);		// ADC - > Interrupt Enable		
  
  /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der n�chsten
     Wandlung nicht �bernommen. */
  result = ADCW;		 
   
}
 



/**************************************************************************************************************

Funktion: uint16_t adcRead(uint8_t channel)

Aufgabe:

W�hlt den gew�nschten Kanal im Register ADMUX aus, startet eine Wandlung und wartet auf das Ergebnis.

Die maximale SystemSpannung des Busses von 15V wird �ber einen externen Spannungsteiler auf 5V geteilt.

Alle Ergebnis-Werte werden *100 genommen um Flie�komma-Operationen zu vermeiden. Dies wird in der 
Display Funktion wieder zur�ckgerechnet.

Rechnung (mit 15V und 10bit Aufl�sung des ADC):


 x V	  y bits				   15V * y bits  * 100
------ = --------  --->    x V = -----------------------
 15V	   1023						     1023


**************************************************************************************************************/
uint16_t adcRead15(uint8_t channel)
{
	uint16_t result=0;
	
	// 	Kanal waehlen(durch Parameter�bergabe)	
	//  Kan�le 4 bis 7 sind verf�gbar
	if ((channel >= 4) && (channel <=7))
 		ADMUX = channel;                  // 	Kanal waehlen(durch Parameter�bergabe)

	//Erste conversion ansto�en, danach l�uft der ADC im free run mode
  	ADCSRA |= (1<<ADSC);	
  

  	//Auf Abschluss der Konvertierung warten
	while (ADCSRA & (1<<ADSC) ) {}
	
	//result in bit value
	result = ADCW;
	//convert result into voltage value
	//korrektes runden mit +(divisor>>1)
	//ohne die 16bit casts w�rde das zwischenergebnis auf die bitwertigkeit des
	//niedrigsten teil des terms reduziert (hier 8 bit von 100 oder 15)
	result = ((result * (uint16_t)15 * (int16_t)100)+(1023>>1)) / (uint16_t)1023;

	return result;
}
/**************************************************************************************
Die  gleich Funktion noch einmal mit 5V als Bezugs-Spannung
**************************************************************************************/
uint16_t adcRead5(uint8_t channel)
{
	uint16_t result=0;
	
	// 	Kanal waehlen(durch Parameter�bergabe)	
	//  Kan�le 4 bis 7 sind verf�gbar
	if ((channel >= 4) && (channel <=7))
 		ADMUX = channel;                  // 	Kanal waehlen(durch Parameter�bergabe)

	//Erste conversion ansto�en, danach l�uft der ADC im free run mode
  	ADCSRA |= (1<<ADSC);	
  

  	//Auf Abschluss der Konvertierung warten
	while (ADCSRA & (1<<ADSC) ) {}
	
	//result in bit value
	result = ADCW;
	//convert result into voltage value
	//korrektes runden mit +(divisor>>1)
	//ohne die 16bit casts w�rde das zwischenergebnis auf die bitwertigkeit des
	//niedrigsten teil des terms reduziert (hier 8 bit von 100 oder 15)
	result = ((result * (uint16_t)5 * (uint16_t)100) + (1023>>1)) / (uint16_t)1023;

	return result;

}



