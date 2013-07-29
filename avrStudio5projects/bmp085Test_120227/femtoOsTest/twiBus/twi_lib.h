//#################################################################################
//#################################################################################
//#################################################################################
/*	Library to use TWI with ATMEL Atmega family
	
	Copyright (C) 2010 Stefan Sicklinger

	For support check out http://www.sicklinger.com
    
	This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
//#################################################################################
//#################################################################################
//#################################################################################
		
#include "globals.h"

#ifndef _TWI_LIB_H_
#define _TWI_LIB_H_ 1
//----------------------------------------
// Prototypes
//----------------------------------------
//! Set the I2C transaction bitrate (in KHz)
void i2cSetBitrate(uint16_t bitratekHz);
//! Send an I2C start condition in Master mode or repeated start condition
void i2cSendStart(void);
//! Send an I2C stop condition in Master mode
uint8_t i2cSendStop(void);
//! Send an (address|R/W) combination or a data byte over I2C
void i2cSendByte(unsigned char data);
//! Receive a data byte over I2C  
void i2cReceiveByteACK(void);
//! Receive a data byte over I2C  
void i2cReceiveByteNACK(void);
// get received byte back
uint8_t i2cGetReceivedByte(void);
//! Wait for current I2C operation to complete
uint8_t i2cWaitForComplete(void);
// Check for expected error code
uint8_t checki2cReturnCode(uint8_t expected_return_code);
//#################################################################################
//#################################################################################
//#################################################################################

//----------------------------------------
// 
//----------------------------------------
#endif /* _TWI_LIB_H_ */
