/*
 * twi_lib.c
 *
 * Created: 27.02.2012 18:15:15
 *  Author: xxxburns
 */ 

#include "twi_lib.h"


//----------------------------------------
// 
//----------------------------------------
void i2cSetBitrate(uint16_t bitratekHz)
{
	uint8_t bitrate_div;
	// set i2c bitrate
	// Set Prescaler to one --> TWPS = 1
	TWSR =((0<<TWPS0)|(0<<TWPS1));
	//calculate bitrate division	
	bitrate_div = (((float)F_CPU/1000.0)/(float)bitratekHz);
	if(bitrate_div >= 16){
		bitrate_div = (float)(bitrate_div-16)/8;
	}
	//printf("DIV: %d \n",bitrate_div);
	TWBR=(uint8_t)bitrate_div;
	//TWBR=0; -> max speed
}
//----------------------------------------
// 
//----------------------------------------
void i2cSendStart(void)
{
	// send start condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}
//----------------------------------------
// 
//----------------------------------------
uint8_t i2cSendStop(void)
{
	
	uint8_t i = 0;		//time out variable
	uint8_t error_code=0;
	// transmit stop condition
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	

	// wait until stop condition is executed and bus released
    while ((TWCR & (1<<TWSTO)) && (i < 90)){
		_delay_us(1);
		i++;
	}
	if (i>89){
		_delay_us(1);
		error_code=100;

	}
	return error_code;
}
//----------------------------------------
// 
//----------------------------------------
void i2cSendByte(unsigned char data)
{
	// save data to the TWDR
	TWDR = data;
	// begin send
	TWCR = (1<<TWINT)|(1<<TWEN);
}
//----------------------------------------
// 
//----------------------------------------
void i2cReceiveByteACK(void)
{

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
}
//----------------------------------------
// 
//----------------------------------------
void i2cReceiveByteNACK(void)
{

	TWCR = (1<<TWINT)|(1<<TWEN);
}
//----------------------------------------
// 
//----------------------------------------
uint8_t i2cWaitForComplete(void)
{
	uint8_t i = 0;		//time out variable
	uint8_t error_code=0;	

	// wait for i2c interface to complete operation
	// -> wait for 90us to pass OR
	// -> wait for twiInterrupt flag to be set (flag == 0)
	//	  is set by hardware when the twi has finished its current job
    while ((!(TWCR & (1<<TWINT))) || (i < 90))
	{
		_delay_us(1);
		i++;
	}
	
	// if the 90us waiting time has passed -> return error code 100
	if (i>89)
	{
		_delay_us(1);
		error_code=100;

	}
	return error_code;
}
//----------------------------------------
// 
//----------------------------------------
uint8_t i2cGetReceivedByte(void)
{
	// retieve received data byte from i2c TWDR
	return (uint8_t)TWDR;
}
//----------------------------------------
// 
//----------------------------------------
uint8_t checki2cReturnCode(uint8_t expected_return_code)
{
	uint8_t error_code=0;
	uint8_t i=0;
	//while (error_code!=0 && i!=0 && i<3){
		if ((TWSR & 0xF8) != expected_return_code){
			error_code=10;
		}
	i++;
//	}
	return error_code;
}




/*******************************************************
 Public Function: TWIM_Init

 Purpose: Initialise the TWI Master Interface

 The SCL period is controlled by settings in the TWI Bit
 Rate Register (TWBR) and the Prescaler bits in the TWI Status
 Register (TWSR).
 

                            CPU_Frequency
 SCE_Frequency = --------------------------------------	
                    16 + 2*(TWBR) * (4^TWPS)


 With CPU_Frequency = 8MHz &&
      TWBR          = 8    &&
	  TWPS			= 1   

	  -> SCE_Frequency = 100KHz
	

 Input Parameter:
 	- uint16_t	TWI_Bitrate (Hz)

 Return Value: uint8_t
 	- FALSE:	Bitrate too high
 	- TRUE:		Bitrate OK

*******************************************************/
uint8_t TWIM_Init (void)
{	
	/* set prescaler to 1 */
	TWSR = ((0<<TWPS0)|(0<<TWPS1));
	TWBR = 8;

	return TRUE;
}
/*******************************************************
 Public Function: TWIM_Start

 Purpose: Start the TWI Master Interface

 Input Parameter:
 	- uint8_t	Device address
 	- uint8_t	Type of required Operation:
				TWIM_READ: Read data from the slave
				TWIM_WRITE: Write data to the slave

 Return Value: uint8_t
  	- TRUE:		OK, TWI Master accessible
 	- FALSE:	Error in starting TWI Master

*******************************************************/
uint8_t TWIM_Start (uint8_t Address, uint8_t TWIM_Type)
{
	uint8_t		twst;
    /*
    ** Send START condition
	*/
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	/*
	** Wait until transmission completed
	*/
	while (!(TWCR & (1<<TWINT)));
	/*
	** Check value of TWI Status Register. Mask prescaler bits.
	*/
	twst = TWSR & 0xF8;
	if ((twst != TWI_START) && (twst != TWI_REP_START)) return FALSE;
	/*
	** Send device address
	*/
	TWDR = (Address<<1) + TWIM_Type;
	TWCR = (1<<TWINT)|(1<<TWEN);
	/*
	** Wait until transmission completed and ACK/NACK has been received
	*/
	while (!(TWCR & (1<<TWINT)));
	/*
	** Check value of TWI Status Register. Mask prescaler bits.
	*/
	twst = TWSR & 0xF8;
	if ((twst != TWI_MTX_ADR_ACK) && (twst != TWI_MRX_ADR_ACK)) return FALSE;

	return TRUE;
}
/*******************************************************
 Public Function: TWIM_Stop

 Purpose: Stop the TWI Master

 Input Parameter: None

 Return Value: None

*******************************************************/
void TWIM_Stop (void)
{
	/*
	** Send stop condition
	*/
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	/*
	** Wait until stop condition is executed and bus released
	*/
	while (TWCR & (1<<TWINT));
}
/*******************************************************
 Public Function: TWIM_Write

 Purpose: Write a byte to the slave

 Input Parameter:
 	- uint8_t	Byte to be sent

 Return Value: uint8_t
  	- TRUE:		OK, Byte sent
 	- FALSE:	Error in byte transmission

*******************************************************/
uint8_t TWIM_Write (uint8_t byte)
{
	uint8_t   twst;
	/*
	** Send data to the previously addressed device
	*/
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN);
	/*
	** Wait until transmission completed
	*/
	while (!(TWCR & (1<<TWINT)));
	/*
	** Check value of TWI Status Register. Mask prescaler bits
	*/
	twst = TWSR & 0xF8;
	if (twst != TWI_MTX_DATA_ACK) return FALSE;

	return TRUE;
}
/*******************************************************
 Public Function: TWIM_ReadAck

 Purpose: Read a byte from the slave and request next byte
 	      -> the acknowledge pulse is generated.

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t TWIM_ReadAck (void)
{
	
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);

	while (!(TWCR & (1<<TWINT)));    

	return TWDR;
}
/*******************************************************
 Public Function: TWIM_ReadNack

 Purpose: Read the last byte from the slave

 Input Parameter: None

 Return Value: uint8_t
  	- uint8_t	Read byte

*******************************************************/
uint8_t TWIM_ReadNack (void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);

	while(!(TWCR & (1<<TWINT)));
	
	return TWDR;
}

