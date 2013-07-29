/*  Projektziele:
/	- Kennenlernen des Grafik-Displays dogm135-w
/	- Ausprobieren der dogm-library von mikrocontroller.net
/
/
/   Device:
/	- Atmega32 -> 8MHz
/
/
/   ÄNDERN:
/	//#define spi_wait_for_idle() while(! (UCSR1A & _BV(TXC1)));UCSR1A |=
/	_BV(TXC1)
/	#define spi_wait_for_idle() while(!(SPSR & (1<<SPIF)))
/
/
/http://www.mikrocontroller.net/topic/171444#new
/
/
*/ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "dogm-graphic.h"
#include "font.h"
#include "pwm_init.h"




//volatile double adc_wert;		//Wert der vom ADC kommt


ISR(ADC_vect) //ISR für ADC Conversion Complete
{

}

void myInit()
{
	//initialize lcd-display
	lcd_init();
	
	//PortC.3 as status led output
	DDRA |= (1<<PINA7);
	PORTA |= (1<<PINA7);
	

}



int main(void)
{
    //variables
	int8_t compareValue = 15;
    int8_t currentPage = 1;
	int8_t currentColumn = 1; 	


	cli();
	//Variablen
	myInit();
    pwm_init(compareValue);

	
    //lcd ouput:
	lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("pos(page/column): "));
	//currentPage =  lcd_get_position_page();
	//currentColumn = lcd_get_position_column();
	//lcd_put_short(currentPage);
    //lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("/"));
    //lcd_put_short(currentColumn);   


    lcd_moveto_xy  (1, 0);
	lcd_set_font(FONT_FIXED_8,NORMAL);
    lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("PWM Wert: "));
    lcd_put_short(compareValue);

	int8_t leftFlag=1;

    while(1)
    {
		//for servo demo
		if (leftFlag)
		{
			compareValue++;
			pwm_init(compareValue);
			_delay_ms(100);
		   	if (compareValue==30)
			{
				leftFlag=0;			 
		    }	
		}
		else
		{
			compareValue--;
			pwm_init(compareValue);
			_delay_ms(100);
			if (compareValue==17)
			{
				leftFlag=1;
			}
		}

    }
 
    return 0;
}




















