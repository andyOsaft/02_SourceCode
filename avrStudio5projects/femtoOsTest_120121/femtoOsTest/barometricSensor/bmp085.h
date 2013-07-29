

#ifndef __BMP085_H
#define __BMP085_H


#include "globals.h"


// From the datasheet the BMP module address LSB distinguishes
// between read (1) and write (0) operations, corresponding to 
// address 0xEF (read) and 0xEE (write).
// shift the address 1 bit right (0xEF or 0xEE), the Wire library only needs the 7
// most significant bits for the address 0xEF >> 1 = 0x77
// 0xEE >> 1 = 0x77

extern int I2C_ADDRESS;  // sensor address

// oversampling setting
// 0 = ultra low power
// 1 = standard
// 2 = high
// 3 = ultra high resolution
extern const unsigned char oversampling_setting; //oversampling for measurement
extern const unsigned char pressure_conversiontime[4];  // delays for oversampling settings 0, 1, 2 and 3   

// sensor registers from the BOSCH BMP085 datasheet
extern int ac1;
extern int ac2; 
extern int ac3; 
extern unsigned int ac4;
extern unsigned int ac5;
extern unsigned int ac6;
extern int b1; 
extern int b2;
extern int mb;
extern int mc;
extern int md;

// variables to keep the values
extern int temperature;
extern long pressure;

void setupBarometricSensor();
void  getCalibrationData(); 
uint16_t readInternalRegister(unsigned char registerAddress);
	



#endif 