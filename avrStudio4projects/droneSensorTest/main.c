//#################################################################################
//#################################################################################
//#################################################################################
#include "globals.h"

#define CLEAR_DISPLAY 0x01


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
	
	//i2cSetBitrate(1000);		//Initialize TWI 1000kHz	
	TWIM_Init(); //Initialize TWI 100KHz
	
	_delay_ms(10);	
	
	//initialize temperature and pressure sensor
	//BMP085_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&error_code);////Initialize BMP085
	
	//initialize inertial measurement unit
	initImu3000();
	
	//initialize lcd display
	lcd_init();
	lcd_set_font(FONT_FIXED_8,NORMAL);

	_delay_ms(200);

	/* read gyro and accelerometer sensor values in stable state -> set offsets */
	readImu(&gxOffsetExternal, &gyOffsetExternal, &gzOffsetExternal, &axOffsetExternal, &ayOffsetExternal, &azOffsetExternal); 




//-----------------------------------------
// Do forever
//-----------------------------------------
		

    while(1)
    {
		/*
		bmp085Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
		
		
		lcd_moveto_xy  (1, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Temp: "));
		lcd_put_int((int16_t)temperature);

		lcd_moveto_xy  (2, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Pressure: "));
		lcd_put_long(pressure);
	
		altitude = bmp085CalcAltitude (pressure);
		
		lcd_moveto_xy  (3, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("Altitude: "));
		lcd_put_int((int16_t)altitude);
		*/
		

		readImu(&gx, &gy, &gz, &ax, &ay, &az);
		/*
		lcd_moveto_xy  (0, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("gx:     gy:     gz:"));
		lcd_moveto_xy  (1, 0);
		lcd_put_int((int16_t)gx);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("   "));
		lcd_put_int((int16_t)gy);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("   "));
		lcd_put_int((int16_t)gz);


		lcd_moveto_xy  (2, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("ax:     ay:     az:"));
		lcd_moveto_xy  (3, 0);
		lcd_put_int((int16_t)ax);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("   "));
		lcd_put_int((int16_t)ay);
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("   "));
		lcd_put_int((int16_t)az);
		

		_delay_ms(200);	
		lcd_clear_area_xy(1,132,0, 1, 0);
		lcd_clear_area_xy(1,132,0, 3, 0);
		*/


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

		lcd_moveto_xy  (0, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("xPos: "));
		lcd_put_int((int16_t)xPosition);

		lcd_moveto_xy  (1, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("yPos: "));
		lcd_put_int((int16_t)yPosition);

		lcd_moveto_xy  (2, 0);		
		lcd_put_string_P(FONT_FIXED_8, NORMAL, PSTR("zPos: "));
		lcd_put_int((int16_t)zPosition);


		

		_delay_ms(200);	
		lcd_clear_area_xy(1,132,0, 0, 0);
		lcd_clear_area_xy(1,132,0, 1, 0);
		lcd_clear_area_xy(1,132,0, 2, 0);



			
    }


    return 0;
}
// E N D  M A I N  P R O G R A M 
//#########################################
//#########################################
//#########################################



