// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
// Die Pinbelegung ist über defines in lcd-routines.h einstellbar
 
#include <avr/io.h>
#include "lcd-routines.h"
#include <util/delay.h>
 
// sendet ein Datenbyte an das LCD
 
void lcd_data(unsigned char temp1)
{
   unsigned char temp2 = temp1;
 
   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
   //High Teil des Zeichens ausgeben 			
   temp1 = temp1 & 0xF0;			//das low nibble von temp1 auf null setzen; 
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
   LCD_PORT |= temp1;               //das high nibble von temp1 wird PORTD zugewiesen
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   //Low Teil des Zeichens ausgeben
   temp2 = temp2 << 4;				//das low nibble von temp2 wird auf das high nibble verschoben	
   temp2 = temp2 & 0xF0;			//das alte low nibble von temp2 wird zu 0 gesetzt
   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
   LCD_PORT |= temp2;               //das high nibble von temp2 wird PORTD zugewiesen
   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
   _delay_us(42);
}
 
// sendet einen Befehl an das LCD
 
void lcd_command(unsigned char temp1)
{
   unsigned char temp2 = temp1;
 
   LCD_PORT &= ~(1<<LCD_RS);        // RS auf 0 setzen
 
   temp1 = temp1 & 0xF0;            // maskieren
   LCD_PORT &= 0x0F;
   LCD_PORT |= temp1;               // setzen
   lcd_enable();
 
   temp2 = temp2 << 4;				// unteres Nibble holen
   temp2 = temp2 & 0xF0;            // unteres Nibble maskieren
   LCD_PORT &= 0x0F;
   LCD_PORT |= temp2;               // setzen
   lcd_enable();
   
   _delay_us(42);
}
 
// erzeugt den Enable-Puls
void lcd_enable(void)
{
   // Bei Problemen ggf. Pause gemäß Datenblatt des LCD Controllers einfügen
   // http://www.mikrocontroller.net/topic/81974#685882
   LCD_PORT |= (1<<LCD_EN);			// enable setzen
    _delay_us(1);                   // kurze Pause
   
   
   // Bei Problemen ggf. Pause gemäß Datenblatt des LCD Controllers verlängern
   // http://www.mikrocontroller.net/topic/80900
   LCD_PORT &= ~(1<<LCD_EN);		//enabel zurücksetzen
}
 
// Initialisierung: 
// Muss ganz am Anfang des Programms aufgerufen werden.
 
void lcd_init(void)
{
   LCD_DDR = LCD_DDR | 0xF0 | (1<<LCD_RS) | (1<<LCD_EN);   // Ports auf Ausgang schalten
   //                 Datenbits PD4 bis PD7
   //						 RS = PD2
   //									   E = PD4															 

   // muss 3mal hintereinander gesendet werden zur Initialisierung
 
   _delay_ms(30);

   LCD_PORT &= 0x0F;			  //Oberes Nibble auf 0 setzen
   LCD_PORT |= 0x30;			  //DB7=0  DB6=0  DB5=1  DB4=1	            
   LCD_PORT &= ~(1<<LCD_RS);      // RS auf 0
   lcd_enable();				  // E setzen --> Pause --> E reset 
 
   _delay_ms(5);
   lcd_enable();				  //2.Senden
 
   _delay_ms(1);
   lcd_enable();				  //3.Senden

   _delay_ms(1);
 
   // 4 Bit Modus aktivieren 
   LCD_PORT &= 0x0F;
   LCD_PORT |= 0x20;
   lcd_enable();
   _delay_ms(1);
 
   // 4Bit / 2 Zeilen / 5x7
   lcd_command(0x28);
    
   // Display ein / Cursor aus / kein Blinken
   lcd_command(0x0C); 
 
   // inkrement / kein Scrollen
   lcd_command(0x06);
 
   lcd_clear();
}
 
// Sendet den Befehl zur Löschung des Displays
 
void lcd_clear(void)
{
   lcd_command(CLEAR_DISPLAY);
   _delay_ms(5);
}
 
// Sendet den Befehl: Cursor Home
 
void lcd_home(void)
{
   lcd_command(CURSOR_HOME);
   _delay_ms(5);
}
 
// setzt den Cursor in Spalte x (0..15) und Zeile y (1..4)
 
void set_cursor(uint8_t x, uint8_t y)
{
  uint8_t tmp;
 
  switch (y) {
    case 1: tmp=0x80+0x00+x; break;    // 1. Zeile
    case 2: tmp=0x80+0x40+x; break;    // 2. Zeile
    case 3: tmp=0x80+0x10+x; break;    // 3. Zeile
    case 4: tmp=0x80+0x50+x; break;    // 4. Zeile
    default: return;                   // für den Fall einer falschen Zeile
  }
  lcd_command(tmp);
}
 
// Schreibt einen String auf das LCD
 
void lcd_string(char *data)
{
    while(*data) {
        lcd_data(*data);
        data++;
    }
}
