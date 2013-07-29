//Enthält alle Funktionen, die die Ansteuerung des Schrittmotors betreffen
//und zusätzlich spezielle Display-Ansteurugen

#include <avr/io.h>
#include "lcd-routines.h"
#include <util/delay.h>
#include "heiz-routines.h"


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7 

void heizInit()
{
   	DDRB  &= ~(1<<BIT2);//PB2 als Input
	PORTB |= (1<<BIT2); //Pull-Up aktiv 

	DDRD  &= ~(1<<BIT6);	//Port D.6 als Eingang (Prüft ob die Zündung noch an ist)
	PORTD |= (1<<BIT6);		//Wenn Zündung an, dann 0V an D.6 (durch Transistor)
							//Wenn Zündung aus, dann 5V über internen Pull-Up

	DDRD  |= (1<<BIT7); //PD7 als Output (ISR-Kontroll_LED)
	PORTD |= ~(1<<BIT7);//PD7 = OFF

	MCUCR |= (1<<BIT3) | (1<<BIT1);
	GICR  |= (1<<BIT7) | (1<<BIT6) | (1<<BIT5);

	
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

