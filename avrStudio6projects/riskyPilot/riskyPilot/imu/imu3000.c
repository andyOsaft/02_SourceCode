#include "globals.h"
//#include "i2c.h"


/* global variables for IMU3000 values */

/* offset  values */
int16_t gxOffsetExternal;
int16_t gyOffsetExternal;
int16_t gzOffsetExternal;

int16_t axOffsetExternal;
int16_t ayOffsetExternal;
int16_t azOffsetExternal;





void initImu3000(void)
{
	//uint16_t gxOffset;
	//uint16_t gyOffset;
	//uint16_t gzOffset;
	 
	// Initialize Gyro
  	/*  - stop clock and synchronour reset clock state
		- reset device  */
	imuWriteReg8(IMU3000_addr, PWR_MGM, 0x87);

	/*  - select PLL with X Gyro reference as clock source  */
	imuWriteReg8(IMU3000_addr, PWR_MGM, 0x01);


	/* info: analog gyros are sampled internally either at 1KHz or 8KHz
	          -> this sampling is then filtered digitally (low-pass filter)
			  -> the update rate of the filtered sensor data then depends
			     on the frequency f_sample */

	/*  - gyro full scale range +/- 2000 �/s  
	    - low pass filter bandwith 42 Hz
		- analog gyro sampling rate 1KHz */
	imuWriteReg8(IMU3000_addr, DLPF_FS, FS_SEL_2000 | DLPF_CFG_42);

	/*  - set sensor data update frequency f_sample:
	
		-> f_sample = f_internal / (divider+1)
	
		-> here: f_sample = 1KHz / (7+1) = 125Hz -> 8ms per sample	

	*/ 
	imuWriteReg8(IMU3000_addr, SMPLRT_DIV, 7);

	imuWriteReg8(IMU3000_addr, AUX_SLV_ADDR, ADXL345_addr);
	imuWriteReg8(IMU3000_addr, AUX_BURST_ADDR, 0x32);
	imuWriteReg8(IMU3000_addr, USER_CTRL, AUX_IF_RST);

	// Initialize Auxilliary Accelerometer
	initAccelerometer();

	imuWriteReg8(IMU3000_addr, USER_CTRL, AUX_IF_RST | AUX_IF_EN);

	


	/* read gyro registers in stable state to get offset */

	//imuReadReg16(IMU3000_addr, GYRO_XOUT, &gxOffset);
	//imuReadReg16(IMU3000_addr, GYRO_YOUT, &gyOffset);
	//imuReadReg16(IMU3000_addr, GYRO_ZOUT, &gzOffset);

	//gxOffsetExternal = (int16_t) gxOffset;
	//gyOffsetExternal = (int16_t) gyOffset;
	//gzOffsetExternal = (int16_t) gzOffset;


	/* write offset values */
	//imuWriteReg16(IMU3000_addr, X_OFFS, gxOffset);
	//imuWriteReg16(IMU3000_addr, Y_OFFS, gyOffset);
	//imuWriteReg16(IMU3000_addr, Z_OFFS, gzOffset);


}

void initAccelerometer(void)
{
	imuWriteReg8(ADXL345_addr, 0x2D, 8);
	imuWriteReg8(ADXL345_addr, 0x31, 11);	
}


void readImu(int16_t *gx, int16_t *gy, int16_t *gz, int16_t *ax, int16_t *ay, int16_t *az)
{
	volatile uint16_t tempGX, tempGY, tempGZ;
	volatile uint16_t tempAX, tempAY, tempAZ;


	imuReadReg16(IMU3000_addr, GYRO_XOUT, &tempGX);
    imuReadReg16(IMU3000_addr, GYRO_YOUT, &tempGY);
    imuReadReg16(IMU3000_addr, GYRO_ZOUT, &tempGZ);
    imuReadReg16Inv(IMU3000_addr, AUX_XOUT, &tempAX);
    imuReadReg16Inv(IMU3000_addr, AUX_YOUT, &tempAY);
    imuReadReg16Inv(IMU3000_addr, AUX_ZOUT, &tempAZ);


	*gx = ((int16_t) tempGX) - gxOffsetExternal;
	*gy = ((int16_t) tempGY) - gyOffsetExternal;
	*gz = ((int16_t) tempGZ) - gzOffsetExternal;
	*ax = ((int16_t) tempAX) - axOffsetExternal;
	*ay = ((int16_t) tempAY) - ayOffsetExternal;
	*az = ((int16_t) tempAZ) - azOffsetExternal;


}



/*******************************************************
 Public Function: imuWriteReg8

 Purpose: Write an 8bit value into the target큦 register.

 Input Parameter:
 	- uint8_t	Target register address
 	- uint8_t	Target value for register

 Return Value: uint8_t
  	- TRUE:		Value written successfully to register
 	- FALSE:	Error during the writing process

*******************************************************/
uint8_t imuWriteReg8(uint8_t targetAddress, uint8_t reg, uint8_t value)
{
	//start the twi master interface
	if (TWIM_Start(targetAddress, TWIM_WRITE))
	{
		if (TWIM_Write(reg))
	  	{
	    	if (TWIM_Write(value))
	  		{
				//value writte successfully
	    		TWIM_Stop();
		    	return 1;
			}
		}
	}
	//error occurred during write process
	TWIM_Stop();
	return 0;
}




/*******************************************************
 Public Function: imuReadReg8

 Purpose: Read the 8bit value out of the target큦 register.

 Input Parameter:
 	- uint8_t	Target register address
 	- uint8_t	Storage for the read value

 Return Value: uint8_t
  	- TRUE:		Value read successfully from register
 	- FALSE:	Error during the reading process

*******************************************************/
uint8_t imuReadReg8(uint8_t targetAddress, uint8_t reg, uint8_t* buffer)
{
	if (TWIM_Start(targetAddress, TWIM_WRITE))
	{
		if (TWIM_Write(reg))
	    {
			if (TWIM_Start(targetAddress, TWIM_READ))
	    	{ 
	    		*buffer = TWIM_ReadNack();
	    		TWIM_Stop();
	    		return 1;
	  		}
		}
	}
	TWIM_Stop();
	return 0;
}


/*******************************************************
 Public Function: imuWriteReg16

 Purpose: Write an 16bit value into the target큦 register.

 Input Parameter:
 	- uint8_t	Target register address
 	- uint16_t	Target value for register

 Return Value: uint8_t
  	- TRUE:		Value written successfully to register
 	- FALSE:	Error during the writing process

*******************************************************/
uint8_t imuWriteReg16(uint8_t targetAddress, uint8_t reg, uint16_t value)
{
	uint8_t twiStatus = 0;
	
	//at first, write upper 8 bit
	//start the twi master interface
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write(reg);
		if (twiStatus)
	  	{
			twiStatus = TWIM_Write(value);
		}
	}
	TWIM_Stop();

	// check if an error has occurred
	if (twiStatus == 0)
	{
		// an error has occurred -> cancel write operation
		return 0;
	} 


	//if no error has occurred -> write the lower 8 bit
	//start the twi master interface
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write((reg+1));
		if (twiStatus)
	  	{
			twiStatus = TWIM_Write((uint8_t)(value & 0b11111111));
		}
	}
	TWIM_Stop();

	if (twiStatus == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


/*******************************************************
 Public Function: imuReadReg16

 Purpose: Read the 16bit value out of the target큦 register.

 Input Parameter:
 	- uint8_t	Target register address
 	- uint16_t	Storage for the read value

 Return Value: uint8_t
  	- TRUE:		Value read successfully from register
 	- FALSE:	Error during the reading process

*******************************************************/
uint8_t imuReadReg16(uint8_t targetAddress, uint8_t reg, uint16_t* buffer)
{
	uint8_t h=0, l=0;
	uint8_t twiStatus;
	
	//at first, read upper 8 bit
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write(reg);
		if (twiStatus)
	    {
			twiStatus = TWIM_Start(targetAddress, TWIM_READ);
			if (twiStatus)
	    	{ 
	    		h = TWIM_ReadNack();
	  		}
		}
	}

	TWIM_Stop();
	
	//check if an error has occurred
	if (twiStatus == 0)
	{
		// an error has occurred
		return 0;
	}
	

	//read lower 8 bit
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write(reg+1);
		if (twiStatus)
	    {
			twiStatus = TWIM_Start(targetAddress, TWIM_READ);
			if (twiStatus)
	    	{ 
	    		l = TWIM_ReadNack();
	  		}
		}
	}

	TWIM_Stop();

	//check if an error has occurred
	if (twiStatus == 0)
	{
		// an error has occurred
		return 0;
	}
	else
	{
		//merge the upper and lower 8 bits together to have the 16bit variable
		*buffer = (((uint16_t)h) << 8) + (uint16_t)l;
		return 1;
	}
	
}



/***************************************************
 Public Function: imuReadReg16Inv

 Purpose: Read the 16bit value out of the target큦 register.

 Input Parameter:
 	- uint8_t	Target register address
 	- uint16_t	Storage for the read value

 Return Value: uint8_t
  	- TRUE:		Value read successfully from register
 	- FALSE:	Error during the reading process

*******************************************************/
uint8_t imuReadReg16Inv(uint8_t targetAddress, uint8_t reg, uint16_t* buffer)
{

	uint8_t h=0, l=0;
	uint8_t twiStatus;
	
	//at first, read upper 8 bit
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write(reg);
		if (twiStatus)
	    {
			twiStatus = TWIM_Start(targetAddress, TWIM_READ);
			if (twiStatus)
	    	{ 
	    		l = TWIM_ReadNack();
	  		}
		}
	}

	TWIM_Stop();
	
	//check if an error has occurred
	if (twiStatus == 0)
	{
		// an error has occurred
		return 0;
	}
	

	//read lower 8 bit
	twiStatus = TWIM_Start(targetAddress, TWIM_WRITE);
	if (twiStatus)
	{
		twiStatus = TWIM_Write(reg+1);
		if (twiStatus)
	    {
			twiStatus = TWIM_Start(targetAddress, TWIM_READ);
			if (twiStatus)
	    	{ 
	    		h = TWIM_ReadNack();
	  		}
		}
	}

	TWIM_Stop();

	//check if an error has occurred
	if (twiStatus == 0)
	{
		// an error has occurred
		return 0;
	}
	else
	{
		//merge the upper and lower 8 bits together to have the 16bit variable
		*buffer = (((uint16_t)h) << 8) + (uint16_t)l;
		return 1;
	}
	
}


