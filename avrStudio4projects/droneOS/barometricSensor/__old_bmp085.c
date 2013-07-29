

#include "bmp085.h"

// From the datasheet the BMP module address LSB distinguishes
// between read (1) and write (0) operations, corresponding to 
// address 0xEF (read) and 0xEE (write).
// shift the address 1 bit right (0xEF or 0xEE), the Wire library only needs the 7
// most significant bits for the address 0xEF >> 1 = 0x77
// 0xEE >> 1 = 0x77

int barometricSensorAddress = 0x77;  // sensor address

// oversampling setting
// 0 = ultra low power
// 1 = standard
// 2 = high
// 3 = ultra high resolution
const unsigned char oversampling_setting = 3; //oversampling for measurement
const unsigned char pressure_conversiontime[4] = { 
  5, 8, 14, 26 };  // delays for oversampling settings 0, 1, 2 and 3   

// sensor registers from the BOSCH BMP085 datasheet
volatile int16_t ac1;
volatile int16_t ac2; 
volatile int16_t ac3; 
volatile uint16_t ac4;
volatile uint16_t ac5;
volatile uint16_t ac6;
volatile int16_t b1; 
volatile int16_t b2;
volatile int16_t mb;
volatile int16_t mc;
volatile int16_t md;

// variables to keep the values
//int temperature = 0;
//long pressure = 0;

/************************************************************************************************

	Function:
	
		Initialisation for the barometric sensor bmp085.

************************************************************************************************/
void bmp085_setupSensor()
{    
	bmp085_getCalibrationData();
}



/************************************************************************************************

	Function:
	
		Reads the calibration coefficients out of the sensor큦 EEPROM and stores the
		values in the global variables. 
		The coefficients only need to be read once (possibly on system start-up).

************************************************************************************************/
void  bmp085_getCalibrationData() 
{	

	ac1 = bmp085_readInternalRegister(0xAA);
	
	_delay_ms(100);
	
	ac2 = bmp085_readInternalRegister(0xAC);
	
	_delay_ms(100);

	ac3 = bmp085_readInternalRegister(0xAE);

	_delay_ms(100);

	ac4 = bmp085_readInternalRegister(0xB0);

	_delay_ms(100);

	ac5 = bmp085_readInternalRegister(0xB2);

	_delay_ms(100);

	ac6 = bmp085_readInternalRegister(0xB4);

	_delay_ms(100);

	b1 = bmp085_readInternalRegister(0xB6);

	_delay_ms(100);

	b2 = bmp085_readInternalRegister(0xB8);

	_delay_ms(100);

	mb = bmp085_readInternalRegister(0xBA);

	_delay_ms(100);

	mc = bmp085_readInternalRegister(0xBC);

	_delay_ms(100);

	md = bmp085_readInternalRegister(0xBE);

}

/************************************************************************************************

	Function:
	
		Aim of the function is to provide a possibility to check the calibration values of 
		the bmp085 sensor without jtag debugger hardware.
		Therefore all calibration values are displayed in an infinite loop on the lcd.

************************************************************************************************/
void bmp085_displayCalibrationData(void)
{
	while(1)
	{
		lcd_moveto_xy  (0, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac1: "));
		lcd_put_int(ac1);
		lcd_moveto_xy  (1, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac2: "));		
		lcd_put_int(ac2);
		lcd_moveto_xy  (2, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac3: "));		
		lcd_put_int(ac3);
		lcd_moveto_xy  (3, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac4: "));		
		lcd_put_uint(ac4);
		
		_delay_ms(6000);
		
		lcd_moveto_xy  (0, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac5: "));
		lcd_put_uint(ac5);
		lcd_moveto_xy  (1, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ac6: "));		
		lcd_put_uint(ac6);
		lcd_moveto_xy  (2, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("b1:  "));		
		lcd_put_int(b1);
		lcd_moveto_xy  (3, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("b2:  "));		
		lcd_put_int(b2);
		
		_delay_ms(6000);
				
		lcd_moveto_xy  (0, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("mb:  "));
		lcd_put_int(mb);
		lcd_moveto_xy  (1, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("mc:  "));		
		lcd_put_int(mc);
		lcd_moveto_xy  (2, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("md:  "));		
		lcd_put_int(md);
		lcd_moveto_xy  (3, 0);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("last one     "));		

		_delay_ms(6000);

	}	
}


/************************************************************************************************

	Function:
	
	read an internal 16 bit register of the bmp085

************************************************************************************************/
int16_t bmp085_readInternalRegister(unsigned char registerAddress)
{
	
	unsigned char msgBuf[2];
	unsigned char msb, lsb;
	
	//prepare message for i2c transmission
	//1st byte: -> module address write
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (FALSE<<TWI_READ_BIT);
	//2nd byte: -> target register큦 address for reading
	msgBuf[1] = registerAddress;
	TWI_Start_Transceiver_With_Data( msgBuf, 2 );
	

	//---------------------------------------------------------------------------------------------
	//-> read the value out of selected register result register
	//-> send an address_write and the register address that should be read
	
	//send a restart condition -> just start a new message
	//1st byte: -> module address read
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (TRUE<<TWI_READ_BIT);
	TWI_Start_Transceiver_With_Data( msgBuf, 1 );

	//the sensor sends first the 8 msb, then the 8 lsb
	TWI_Get_Data_From_Transceiver(msgBuf,2);
	msb=msgBuf[0];
	lsb=msgBuf[1];
	
	
	return ((((int16_t)msb)<<8) | ((int16_t)lsb));
	
	/*
	Wire.beginTransmission(I2C_ADDRESS);
	
	Wire.write(r);  // register to read
	Wire.endTransmission();

	Wire.requestFrom(I2C_ADDRESS, 2); // request two bytes
	while(!Wire.available()); // wait until data available
	msb = Wire.read();
	while(!Wire.available()); // wait until data available
	lsb = Wire.read();
	return (((int)msb<<8) | ((int)lsb));*/
}


/************************************************************************************************

	Function:
	
	Reads the uncompensated temperature value out of the bmp085 sensor.
	Uncompensated temperature is a unsigned 16bit value.

************************************************************************************************/
int32_t bmp085_readUncompensatedTemp(void)
{
	uint16_t ut;
	
	unsigned char msgBuf[2];
	unsigned char msb, lsb;
	
	//---------------------------------------------------------------------------------------------
	//-> start temperature measurement -> start the adc conversion
	//-> write 0x2E into the control register (address 0xF4)
	
	//prepare message for i2c transmission
	//1st byte: -> module address write
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (FALSE<<TWI_READ_BIT);
	//2nd byte: -> target register큦 address to write into
	msgBuf[1] = 0xF4;
	//3rd byte: -> the value that should be written into the register 
	msgBuf[2] = 0x2E;
	TWI_Start_Transceiver_With_Data( msgBuf, 3 );
	
	//wait the sensor conversion time 
	_delay_ms(10);
	
	ut = (int32_t) bmp085_readInternalRegister(0xF6);
	
	//---------------------------------------------------------------------------------------------
	//-> read the value out of adc result register
	//-> send an address_write and the register address that should be read
	/*
	//prepare message for i2c transmission
	//1st byte: -> module address write
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (FALSE<<TWI_READ_BIT);
	//2nd byte: -> target register큦 address to write into
	msgBuf[1] = 0xF6;
		
	//send a restart condition -> just start a new message
	//1st byte: -> module address read
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (TRUE<<TWI_READ_BIT);
	TWI_Start_Transceiver_With_Data( msgBuf, 1 );

	
	//the sensor sends first the 8 msb, then the 8 lsb
	TWI_Get_Data_From_Transceiver(msgBuf,2);
	msb=msgBuf[0];
	lsb=msgBuf[1];
	
	ut =  (((int16_t)msb<<8) | ((int16_t)lsb));
	*/
	
	/*
	lcd_moveto_xy  (0, 0);		
	lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ut:  "));
	lcd_put_uint(ut);
	while(1);
	*/
	
	return ut;
	
	/*		
	// Write 0x2E into Register 0xF4
	// This requests a temperature reading
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.send(0xF4);
	Wire.send(0x2E);
	Wire.endTransmission();
	// Wait at least 4.5ms
	delay(5);
	// Read two bytes from registers 0xF6 and 0xF7
	ut = bmp085ReadInt(0xF6);
	return ut;*/	
}



/************************************************************************************************

	Function:
	
	At first the function calls the function to get the uncompensated temperature value.
	Then the calibration coefficients are used to calculate the real temperature value in 

************************************************************************************************/
int32_t bmp085_readTemp(void)
{
	
	volatile int32_t ut;
	volatile int32_t x1, x2;
	volatile int32_t b5;
	volatile int32_t temp;
	
	/*
	ac1=-4168;
	ac2=-4268;
	ac3=-4205;
	ac4=61205;
	ac5=61306;
	ac6=61184;
	b1=-4289;
	b2=-4224;
	mb=-4352;
	mc=-4140;
	md=-4163;
	*/
	ut=61430;
	
		
	//read uncompensated temperature
	//ut = bmp085_readUncompensatedTemp();
	
	x1 = ((int32_t)ut - ac6) * ac5 >> 15;
	
	x2 = ((int32_t) mc << 11) / (x1 + md);
	
	b5 = x1 + x2;
	
	temp=((b5 + 8)>>4);
	
	
	//lcd_moveto_xy  (0, 0);		
	//lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("temp:  "));
	//lcd_put_int(temp);
	
	//while(1);
	
	
	return temp;
	
}
