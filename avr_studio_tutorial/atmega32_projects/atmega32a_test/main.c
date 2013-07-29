
#include <avr/io.h>
#include "lcd-routines.h"
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7 

int main (void) {            
 
	

	DDRA  |= 0x01	;        //Port A.0 als Ausgang
	lcd_init();

	//lcd_home();



   while(1) {                
  
	    PORTA |= (1<<BIT0);  //Port A.0 = High
		lcd_home();
		lcd_string("Svenja:");
		set_cursor(0,2);
		lcd_string("liebesSchlaftier");
	
   }	
   return 0;                 
}
