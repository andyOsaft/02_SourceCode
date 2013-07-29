

#include "globals.h"




void main (void)
{
	uint16_t sensorRead;
	unsigned char msgBuf[2];
	
	uint16_t statusCode;
	
	sei();
	
	myInit();
	
	TWI_Master_Initialise();
	//pwmInit(100);
	
	lcd_set_font(FONT_FIXED_8,NORMAL);
	lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Init ... done!"));
	
	while(1)
	{
		sensorRead+=1;
		//_delay_ms(200);
		
		//TWEN:  twi module enable   
		//TWSTA: twi start condition -> generates a start contition on the bus (it it is free)
		//                              if it is not free, it waits until a stop condition is
		//								detected 
		//TWIE:  the interrupt request for twi will be activated as long as the twint flag is high
		//TWINT: reset twint flag (by writing a one) -> starts the operation of the twi module
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);// | (1<<TWIE); 
		
		
		//TWSR should now have the status code, that the START has been send successfully
		statusCode = TWSR;
		lcd_moveto_xy  (1, 0);	 
		lcd_put_uint(statusCode);
		
		//after the START has been send, the TWINT flag is set 0 (wait until it is 0)
		//while  (!(TWCR & (1<<TWINT)));

		
		
		//write the bmp addressWrite into the TWI data register
		TWDR = 0xEE;
		//start transmission by reseting the TWINT (by writing a one)
		TWCR = (1<<TWINT) | (1<<TWEN);
		
		//while  (!(TWCR & (1<<TWINT)));
				
		TWDR = 0xF4;
		TWCR = (1<<TWINT) | (1<<TWEN);

		//while  (!(TWCR & (1<<TWINT)));
				
		TWDR = 0xF4;
		TWCR = (1<<TWINT) | (1<<TWEN);
					
		
		//send STOP condition
		TWCR = (1<<TWINT) | (1<<TWEN)| (1<<TWSTO);
			
		//TWSR should now have the status code, that the slave has accepted the data package
		statusCode = TWSR;
		lcd_moveto_xy  (2, 0);	 
		lcd_put_uint(statusCode);	
			
				
				
		//readInternalRegister(0xAA);

	
		//sensorRead = readInternalRegister(0xAA);
		
		//lcd_moveto_xy  (2, 0);		
		//lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Task1"));		
		//lcd_put_uint(sensorRead);
		_delay_ms(100);
	}	
	
}


ISR(__vector_20)
{
  PORTA |= (1<<PINA7);
}  
