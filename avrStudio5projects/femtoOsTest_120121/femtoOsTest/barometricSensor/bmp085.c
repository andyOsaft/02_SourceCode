

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
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

// variables to keep the values
int temperature = 0;
long pressure = 0;

/************************************************************************************************

	Function:
	
		Initialisation for the barometric sensor bmp085.

************************************************************************************************/
void setupBarometricSensor()
{    
	getCalibrationData();
}



/************************************************************************************************

	Function:
	
		Reads the calibration coefficients out of the sensor´s EEPROM and stores the
		values in the global variables. 
		The coefficients only need to be read once (possibly on system start-up).

************************************************************************************************/
void  getCalibrationData() 
{	

	ac1 = readInternalRegister(0xAA);
	
	ac2 = readInternalRegister(0xAC);

	ac3 = readInternalRegister(0xAE);

	ac4 = readInternalRegister(0xB0);

	ac5 = readInternalRegister(0xB2);

	ac6 = readInternalRegister(0xB4);

	b1 = readInternalRegister(0xB6);

	b2 = readInternalRegister(0xB8);

	mb = readInternalRegister(0xBA);

	mc = readInternalRegister(0xBC);

	md = readInternalRegister(0xBE);

}


/************************************************************************************************

	Function:
	
	read an internal 16 bit register of the bmp085

************************************************************************************************/
uint16_t readInternalRegister(unsigned char registerAddress)
{
	
	unsigned char msgBuf[2];
	unsigned char msb, lsb;
	
	//prepare message for i2c transmission
	//1st byte: -> module address write
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (FALSE<<TWI_READ_BIT);
	//2nd byte: -> target register´s address for reading
	msgBuf[1] = registerAddress;
	TWI_Start_Transceiver_With_Data( msgBuf, 2 );
	
	//send a restart condition -> just start a new message
	//1st byte: -> module address read
	msgBuf[0] = (barometricSensorAddress<<TWI_ADR_BITS) |  (TRUE<<TWI_READ_BIT);
	TWI_Start_Transceiver_With_Data( msgBuf, 1 );
	
	//the sensor sends first the 8 msb, then the 8 lsb
	TWI_Get_Data_From_Transceiver(msgBuf,2);
	msb=msgBuf[0];
	lsb=msgBuf[1];
	
	
		
	return (((uint16_t)msb<<8) | ((uint16_t)lsb));
	
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