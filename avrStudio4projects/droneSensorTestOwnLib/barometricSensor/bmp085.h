

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
extern volatile  int ac1;
extern volatile int ac2; 
extern volatile int ac3; 
extern volatile unsigned int ac4;
extern volatile unsigned int ac5;
extern volatile unsigned int ac6;
extern volatile int b1; 
extern volatile int b2;
extern volatile int mb;
extern volatile int mc;
extern volatile int md;
extern volatile int32_t ut;

// variables to keep the values
//extern int temperature;
//extern long pressure;

void bmp085_setupBarometricSensor();
void  bmp085_getCalibrationData(); 
uint16_t bmp085_readInternalRegister(unsigned char registerAddress);
int32_t bmp085_readUncompensatedTemp(void);
int32_t bmp085_readTemp(void);
void bmp085_displayCalibrationData(void);

	



#endif 
