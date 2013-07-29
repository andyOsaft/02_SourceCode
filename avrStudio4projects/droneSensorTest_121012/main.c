//#################################################################################
//#################################################################################
//#################################################################################
#include "globals.h"


//#########################################
//#########################################
//#########################################
// S T A R T  M A I N  P R O G R A M 
int main(void)
{	

//-----------------------------------------
// Allocate memory
//-----------------------------------------	
	// ...for temperature and pressure sensor
	/*
	int32_t temperature = 0;
	int32_t pressure = 0;
	int16_t BMP085_calibration_int16_t[8];
	int16_t BMP085_calibration_uint16_t[3];
	uint8_t error_code=0;
	int32_t altitude=0;
	*/

	//... for inertial measurement unit
	volatile int16_t gx;
	volatile int16_t gy;
	volatile int16_t gz;
	volatile int16_t ax; 
	volatile int16_t ay;
	volatile int16_t az;

	volatile int16_t xPosition = 0;
	volatile int16_t yPosition = 0;
	volatile int16_t zPosition = 0;



//-----------------------------------------
// Initialize Stuff
//-----------------------------------------
	
	//basic controller initialization
	myInit();

	//i2cSetBitrate(1000);		//Initialize TWI 1000kHz	
	TWIM_Init(); //Initialize TWI 100KHz
	
	_delay_ms(10);	
	
	//initialize temperature and pressure sensor
	//BMP085_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&error_code);////Initialize BMP085
	
	//initialize inertial measurement unit
	//initImu3000();


	_delay_ms(200);

	/* read gyro and accelerometer sensor values in stable state -> set offsets */
	//readImu(&gxOffsetExternal, &gyOffsetExternal, &gzOffsetExternal, &axOffsetExternal, &ayOffsetExternal, &azOffsetExternal); 




//-----------------------------------------
// Do forever
//-----------------------------------------
		

    while(1)
    {
		/*
		bmp085Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
			
		altitude = bmp085CalcAltitude (pressure);
		

		*/
		

		readImu(&gx, &gy, &gz, &ax, &ay, &az);


		if ((ax < 50 && ax > 0) ||
		    (ax > -50 && ax < 0))
		{
			ax = 0;
		}

		if ((ay < 50 && ay > 0) ||
		    (ay > -50 && ay < 0))
		{
			ay = 0;
		}

		if ((az < 50 && az > 0) ||
		    (az > -50 && az < 0))
		{
			az = 0;
		}


		xPosition += ax;
		yPosition += ay;
		zPosition += az;



			
    }


    return 0;
}
// E N D  M A I N  P R O G R A M 
//#########################################
//#########################################
//#########################################



