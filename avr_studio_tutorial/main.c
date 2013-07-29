/* Alle Zeichen zwischen Schrägstrich-Stern 
   und Stern-Schrägstrich sind lediglich Kommentare */
 
// Zeilenkommentare sind ebenfalls möglich
// alle auf die beiden Schrägstriche folgenden
// Zeichen einer Zeile sind Kommentar
 
#include <avr/io.h>          // (1)#
#include <stdint.h>
#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7 

int main (void) {            // (2)
 
	

	DDRD  |= 0xFF;             // (3) PortB 2 & 3 = Ausgang
    PORTD |= (1<<BIT2);  		


	DDRC  &=  ~(1<<BIT5); //C.0 als Eingang
	PORTC |= (1<<BIT5);	  //Pull-Up aktiv


   while(1) {                // (5a)
  
   		if (!(PINC & (1<<BIT0)))
		   PORTD ^= (1<<BIT3);

   }	
   return 0;                 // (6)
}
