/************************************************************************************************	

	Dieses c-File stellt die PWM-Funktionalität für den Atmega32 zur Verfügung
	
    Genutzt wird der 16Bit Timer1
	
	Der PWM-Ausgabe-Pin ist -bei KanalA -> D.5 (OC1A)
							-bei KanalB -> D.4 (OC1B)


	Mit Übergabe des Parameters "compare" wird der Duty-Cycle eingestellt.
	

	Um den Duty-Cycle aus dem laufenden Betrieb zu modifizieren muss,
	je nach Kanal A oder B das folgende Register mit einem Werte
	zwischen 0 & 1023 beschrieben werden

	Kanal A -> OCR1A
	Kanal B -> OCR1B



		
	Ziel:	-> PWM zur Ansteuerung von Servos erzeugen, d.h. - Periode ca 20ms 
															 - High pulse zwischen 1ms und 2ms
															 - 1ms = 0% ; 2ms = 100%

	Berechnung:

	Prozessortakt: 4MHz/Prescaler64 -> T=16us

	Periode: Register ICR1 legt den TOP Value des Timers fest.
			 (20ms/16us)/2 = 625 -> modifiziert 1250

	
	625*2=1250 Zählschritte mit 16us pro Schritt -> 1 PWM-Periode = 20ms

	High Pulse ist von runterzählenMatch bis hochzählenMatch:	

	1ms Pulse -> Compare Value = (1ms/16us) / 2 =31         modifiziert:61
	2ms Pulse -> Compare Value = (2ms/16us) / 2 = 63 (62.5)				93

	Auflösung zwischen 1ms und 2ms ->  63 - 31 =32  
	-> 31 Zwischenschritte sind ausreichend.


*********************************************************************************************** */

#include <avr/io.h>
#include "pwmInit.h"

void pwmInit(uint16_t compare)
{
	DDRD |= (1<<BIT4) | (1<<BIT5);		//D.4&5 als Ausgänge definieren
	
	//PWM, Phase Correct with ICR1 as TOP value
	//TOP: 625
	TCCR1A |= (1<<WGM11);	
	TCCR1A &= ~(1<<WGM10);	

	TCCR1B |= (1<<WGM13);
	TCCR1B &= ~(1<<WGM12);

	//ICR1 = 625;
	ICR1 = 1250;
										
									
	//Der Timer/Counter zählt nun permanent von 0 -> 625 -> 0. 
	//Periode sind 1250 Zählschritte. 
	//Bei Systemtakt=4MHz/64 -> PWM-Frequenz=50Hz=> 20ms									
	

	//Nicht invertierende PWM, KanalA -> auf Pin D.5
	//- Clear OC1A/OC1B on Compare Match when up-counting. 
	//- Set OC1A/OC1B on Compare Match when downcounting.
	TCCR1A |= (1<<COM1A1); 	
	TCCR1A &= ~(1<<COM1A0);

	//Prescaler = 64
	TCCR1B |= (1<<CS10) | (1<<CS11);
	TCCR1B |= ~(1<<CS12); 


	/*	Prescaler-Settings: 
	
	 CS12/CS11/CS10
		0 	0 	0 	 Stop. Der Timer/Counter wird gestoppt.
		0   0 	1 	CK
		0 	1 	0 	CK / 8
		0 	1 	1 	CK / 64
		1 	0 	0 	CK / 256
		1 	0 	1 	CK / 1024
		1 	1 	0 	Externer Pin 1, negative Flanke
		1 	1 	1 	Externer Pin 1, positive Flanke 
	*/
	
	//OCR1A->16bit register
	OCR1A=compare;	//Vergleichswert laden
	
	
}


void pwmUpdate(uint16_t compare)
{
	OCR1A=compare;	
}
