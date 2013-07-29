//Enthält alle Funktionen, die die Ansteuerung des Schrittmotors betreffen
//und zusätzlich spezielle Display-Ansteurugen


#include "heatController.h"




/******************************************************************************************************

Funktion: void updateHeatController(uint8_t targetTemp)

Aufgabe:

Liest die Spannung des Temperatursensors im Innenraum und rechnet diese in Temperatur um.
(Nur volle Grad, keine Nachkomma-Stellen.)

Der Controller besitzt 2 Zustände:
-Heizen -> pwm=63
-Kühlen -> pwm=31

Die Control-Funktion besitzt 1 Grad Hysterese in jede Richtung und updated die Servo 
Position wenn nötigt.
	
TemperaturSensor -> Spannung -> Bitwert -> Spannungswert*100 -> Temperaturwert
						   abtasten	  umrechnen            Sensor-Formel							

******************************************************************************************************/

void updateHeatController(uint8_t targetTemp)
{
	uint16_t voltInside;
	uint16_t tempInside;

	//Lese Sensor-Spannung (Ergebnis is Volt*100)
	voltInside = adcRead5(6);
	//Wandle Ergebnis in Temperatur-Wert um.
	tempInside = (voltInside*(uint16_t)145)-14377;
	
	//Int Division mit korrekter Rundung
	tempInside = (tempInside+(uint16_t)(100>>1))/(uint16_t)100;
	
	//Control-Funktion
	//Muss geheizt werden?
	if ((targetTemp-1)<tempInside)
		//pwmUpdate(63);
		pwmInit(93);
		//pwmUpdate(93);
	//Muss gekühlt werden?
	else if ((targetTemp+1)>tempInside)
		//pwmUpdate(31);
		pwmInit(61);
		//pwmUpdate(61);
}




















/* Der unten stehende Quellcode sind die Überbleibsel als ich die Heizung noch mit einem 
   Schrittmotor steuern wollte.

void heizInit()
{



	
}

void motorRechtslauf(uint8_t anzahlSchritte)
{
	uint8_t vs[4]={10,6,5,9};
	uint8_t z,n=0;
	
	for (z=0;z<anzahlSchritte;z++)
		{
			PORTA=vs[n&3];
			_delay_ms(10);
			n++;
		}
	PORTA &= ~((1<<BIT0)|(1<<BIT1)|(1<<BIT2)|(1<<BIT3));
}



void motorLinkslauf(uint8_t anzahlSchritte)
{
	uint8_t vs[4]={9,5,6,10};
	uint8_t z,n=0;
	
	for (z=0;z<anzahlSchritte;z++)
		{
			PORTA=vs[n&3];
			_delay_ms(10);
			n++;
		}
	PORTA &= ~((1<<BIT0)|(1<<BIT1)|(1<<BIT2)|(1<<BIT3));
}




void lcdHeizung(uint8_t dispPos)
{
   //Ziel: Alle Pixel der DisplayZelle auf high setzeb
   
   set_cursor(dispPos,2);
   	
   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
   //High Teil des Zeichens ausgeben 			
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
   LCD_PORT |= 0xF0;                //das high nibble von PORTD wird auf 0b1111 gesetzt
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   //Low Teil des Zeichens ausgeben
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
   LCD_PORT |= 0xF0;               //das high nibble von temp2 wird PORTD zugewiesen
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   _delay_us(42);
}


void lcdKuehlung(uint8_t dispPos)
{
   //Ziel: Alle Pixel der DisplayZelle auf high setzeb
   
   set_cursor(dispPos,2);
   	
   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
   //High Teil des Zeichens ausgeben 			
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
   LCD_PORT |= 0x80;                //das high nibble von PORTD wird auf 0b1111 gesetzt
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   //Low Teil des Zeichens ausgeben
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
   LCD_PORT |= 0x80;               //das high nibble von temp2 wird PORTD zugewiesen
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   _delay_us(42);
}

void lcdAusschalten()
{
	lcd_clear();
	lcd_home();
	lcd_string("Auf Wiedersehen");
	_delay_ms(2000);

}
*/
