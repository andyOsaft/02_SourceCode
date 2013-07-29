

#include "globals.h"
//#include "bmp085.h"






int main (void)
{
	
	
	volatile int16_t temperature;
	
	sei();
	
	myInit();
	
	//TWI_Master_Initialise();
	//pwmInit(100);
	
	lcd_set_font(FONT_FIXED_8,NORMAL);
	//lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Init ... done!"));
	
	bmp085_getCalibrationData();
	//bmp085_displayCalibrationData();
	
	while(1)
	{

		temperature = bmp085_readTemp();	
		
		//PORTA |= (1<<PINA7);

		//lcd_moveto_xy  (1, 0);		
		//lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Temp: "));
		//lcd_put_int(temperature);
		
		
		//_delay_ms(2000);

	}	
	
}

