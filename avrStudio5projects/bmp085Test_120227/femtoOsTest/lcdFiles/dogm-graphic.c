/******************************************************************************
 * Display Library
 * for EA-DOGS102 GLCD (102px x 64px)
 *     EA-DOGL128 GLCD (128px x 64px)
 *     EA-DOGM128 GLCD (128px x 64px)
 *     EA-DOGM132 GLCD (132px x 32px)
 * 
 * Provides all basic functions to access the display
 * Since no graphics ram is used, the memory footprint is rather small but
 * also does not allow to change single pixels. Data written to the LCD can
 * not be read back!
 * Text can be displayed using the attached font generator containing several
 * character sets and font variants.
 * Thanks to Oliver Schwaneberg for adding several functions to this library!
 * 
 * Author:  Jan Michel (jan at mueschelsoft dot de)
 * License: GNU General Public License, version 3
 * Version: v0.93 September 2010
 * ****************************************************************************
 * New features in v0.93
 *   - modified initialization for DOGM128 and DOGS102
 * New features in v0.92
 *  - Added initialization for DOGS102 and DOGL128
 * 
 * New features in v0.91
 *  - Control of chip select pin
 *  - backlight & chip select are optional - can be turned of in header file
 *  - added function lcd_draw_image_P 
 *  - added function lcd_draw_image_xy_P
 *  - added function lcd_clear_area
 *  - added SPI initialization
 *****************************************************************************/

#include "dogm-graphic.h"

//=============================================================================
//keeping track of current position in ram - necessary for big fonts & bitmaps
//=============================================================================

uint8_t lcd_current_page = 0;
uint8_t lcd_current_column = 0;



/******************************************************************************
 * Changes the internal cursor by s pages
 * s             - number of pages to move
 */ 
uint8_t lcd_inc_page(int8_t s) {
  uint8_t p = lcd_current_page;
  p += s;
  p %= LCD_RAM_PAGES;    //all lcd have lcd_ram_pages which is power of two
  lcd_current_page = p;
  return p;
  }

/******************************************************************************
 * Changes the internal cursor by s columns, including wrapping (if selected)
 * s             - number of columns to move
 */ 
uint8_t lcd_inc_column(int16_t s) {
  uint16_t c = lcd_current_column;
  c += s;
#if LCD_WRAP_AROUND == 1
  while (c >= LCD_WIDTH) {
    if (s > 0) lcd_inc_page(1);
    else       lcd_inc_page(-1);
    if (s > 0) c -= LCD_WIDTH;
    else       c += LCD_WIDTH;
    }
#endif
  lcd_current_column = c;
  return c;
  }
  
  
/******************************************************************************
 * Moves the cursor to the given position
 * pages         - page to move to
 * columns       - column to move to
 */ 
void lcd_moveto_xy(uint8_t page, uint8_t column) {
  LCD_GOTO_ADDRESS(page,column);
  lcd_current_column = column; 
  lcd_current_page = page;
  }

/******************************************************************************
 * Moves the cursor relative to the current position
 * pages         - number of pages to move
 * columns       - number of columns to move
 */  
void lcd_move_xy(int8_t pages, int16_t columns) {
  lcd_moveto_xy(lcd_inc_page(pages),lcd_inc_column(columns));
  }


//=============================================================================
//Basic Byte Access to Display
//=============================================================================

/******************************************************************************
 * Writes one data byte
 * data          - the data byte
 */
void lcd_data(uint8_t data) {
  LCD_SELECT();
  LCD_DRAM();
  spi_write(data);
  LCD_UNSELECT();
  lcd_inc_column(1);
  }

/******************************************************************************
 * Writes one command byte
 * cmd           - the command byte
 */
void lcd_command(uint8_t cmd) {
  LCD_SELECT();
  LCD_CMD();
   
  spi_write(cmd);
  
  
  //LCD_UNSELECT();
  while(!(SPSR & (1<<SPIF)));
  PORT_CS |= _BV(PIN_CS);
  }
  

//=============================================================================
//Puts raw data from Flash to the Display
//=============================================================================
#if LCD_INCLUDE_GRAPHIC_FUNCTIONS >= 1
/******************************************************************************
 * This function draws a bitmap from the current position on the screen.
 * Parameters:
 * progmem_image - prog_uint8_t array of columns aka the bitmap image
 * pages         - height of image in pages
 * columns       - width of image in pixels (or columns)
 * style         - Bit2: sets inverse mode
 */  
void lcd_draw_image_P(PGM_P progmem_image, uint8_t pages, uint8_t columns, uint8_t style) {
	uint8_t i,j = 0;
  uint8_t inv = (style & INVERT_BIT);
	while(j<pages && (lcd_get_position_page() < LCD_RAM_PAGES)) {
		for (i=0; i<columns && (lcd_get_position_column() < LCD_WIDTH); i++) {
      uint8_t tmp = pgm_read_byte(progmem_image++);
			if(!inv)
				lcd_data(tmp);
			else
				lcd_data(~tmp);
      }
    if(++j != pages)
      lcd_move_xy(1,-columns);
    }
  }

  
/******************************************************************************
 * This function draws a bitmap at any xy-position on the screen. 
 * Be aware that some pixels are deleted due to memory organization!
 * Parameters:
 * progmem_image - prog_uint8_t array of columns aka the bitmap image
 * x             - x start coordinate on the screen (in pixel)
 * y             - y start coordinate on the screen (in pixel)
 * pages         - height of image in pages
 * columns       - width of image in pixels
 * style         - Bit2: sets inverse mode
 */
void lcd_draw_image_xy_P(PGM_P progmem_image, uint8_t x, uint8_t y, uint8_t pages, uint8_t columns, uint8_t style) {
	uint16_t i,j;
  uint8_t data   = 0;
  uint8_t inv    = style & INVERT_BIT;
	uint8_t offset = y & 0x7; //Optimized modulo 8
	//If there is an offset, we must use an additional page
	if(offset)  
    pages++;
  //If there is not enough vertical space -> cut image
  if(pages > LCD_RAM_PAGES - lcd_get_position_page())   
    pages = LCD_RAM_PAGES - lcd_get_position_page();
  //Goto starting point and draw
	lcd_moveto_xy((y>>3), x);
	for (j=0; j<pages; j++) {
		for (i=0; i<columns && (lcd_get_position_column() < LCD_WIDTH); i++){
      data = 0;
      if (!offset || j+1 != pages)
        data = pgm_read_byte(&progmem_image[j*columns + i]) << offset;
			if(j > 0 && offset)
				data |= pgm_read_byte(&progmem_image[(j-1)*columns + i]) >> (8-offset);
			if(inv)	lcd_data(~data);
      else 		lcd_data(data);
		  }
		if(j+1 != pages)
			lcd_move_xy(1,-columns);
	  }
  }
#endif


/******************************************************************************
 * This function clears an area of the screen
 * pages         - height of area in pages
 * columns       - width of area in pixels
 * style         - Bit2: sets inverse mode
 * Cursor is moved to start of area after clear
 */
void lcd_clear_area(uint8_t pages, uint8_t columns, uint8_t style) {
  uint8_t i,j,max;
  uint8_t inv = (style & INVERT_BIT)?0xFF:0;
  
  if(pages > (max = LCD_RAM_PAGES - lcd_get_position_page()))   
    pages = max;
  if(columns > (max = LCD_WIDTH - lcd_get_position_column()))   
    columns = max;
  
  for(j=0; j<pages; j++) {
    for(i=0; i<columns; i++) {
      lcd_data(inv);
      }
    lcd_move_xy(1,-columns);
    }
  lcd_move_xy(-pages,0);
  }

/******************************************************************************
 * This function clears an area of the screen starting at the given coordinates
 * pages         - height of area in pages
 * columns       - width of area in pixels
 * style         - style modifier
 * col           - column of upper left corner
 * page          - page of upper left corner
 * Cursor is moved to start of area after clear
 */
void lcd_clear_area_xy(uint8_t pages, uint8_t columns, uint8_t style, uint8_t col, uint8_t page) {
  lcd_moveto_xy(col,page);
  lcd_clear_area(pages,columns,style);
  }
  

/******************************************************************************
  * Initializes the display in 6 o'clock mode, 4x booster for 2.4-3.3V supply voltage
  * scheme according to datasheet
  * Suitable for all DOGS, DOGM and DOGL displays.
  */
void lcd_init() {
  //for better understanding -> write macros in clear text


  //set outputs
  //LCD_SET_PIN_DIRECTIONS();
  DDR_A0 |= (1<<PIN_A0);
  DDR_RST |= (1<<PIN_RST);
  DDR_CS  |= (1<<PIN_CS);
  
  
  //Initialize SPI Interface
  //LCD_INIT_SPI();   
  init_spi_lcd();
    
  
  //Apply Reset to the Display Controller
  //LCD_RESET();
  PORT_RST &= ~(1<<PIN_RST);
  _delay_ms(1);
  PORT_RST |= (1<<PIN_RST);
  
    
  //Load settings
  
  //(2)
  LCD_SET_FIRST_LINE(0);              //first bit in RAM is on the first line of the LCD
  // |
  //  --> lcd_command(LCD_START_LINE | ((i) & 0x3F))
  //					-> LCD_START_LINE = display start line 0

  //without this delay the content is displayed mirror inverted
  _delay_ms(1); 


  //(8)     
  LCD_SET_BOTTOM_VIEW();              //6 o'clock mode, normal orientation
  // |
  //  --> lcd_command(LCD_BOTTOMVIEW)
  //					-> LCD_BOTTOMVIEW = ADC reverse
  //_delay_ms(1);
  //(15)
  LCD_ORIENTATION_NORMAL();
  // |
  //  --> lcd_command(LCD_SCAN_DIR_NORMAL)
  //					-> LCD_SCAN_DIR_NORMAL = Normal COM0 - COM31
  
  //without this delay the content is displayed mirror inverted
  _delay_ms(1);


  //(10)
  //LCD_SHOW_ALL_PIXELS_OFF();          //Normal Pixel mode
  // |
  //   --> lcd_command(LCD_SHOW_NORMAL)
  //					-> LCD_SHOW_NORMAL = display all points, normal display (10)
    
  //(9)	
  LCD_SET_MODE_POSITIVE();            //positive display
  // |
  //  --> lcd_command(LCD_DISPLAY_POSITIVE)
  //      				-> LCD_DISPLAY_POSITIVE = sets display normal (9)
   
  #if DISPLAY_TYPE == 132
    //(11)
    LCD_SET_BIAS_RATIO_1_9();           //bias 1/9
    // |
	//  --> lcd_command(LCD_BIAS_1_9)
	
    //(16)	
	LCD_SET_POWER_CONTROL(7);           //power control mode: all features on
    // |
	//  --> lcd_command(LCD_POWER_CONTROL | ((i) & 0x07))  
	//						-> command 16 with operating mode = 0b111
	
	
	//(20) -> selbst eingefügtes macro
	LCD_SET_BOOSTER_RATIO();
	
	
	//(17)
	LCD_SET_BIAS_VOLTAGE(3);            //set voltage regulator R/R
	// |
	//   --> lcd_command(LCD_VOLTAGE | ((i) & 0x07))
	// 						-> command 17 with resistor ration 0b011
    
	//(18)
	LCD_SET_VOLUME_MODE(0x1F);          //volume mode set
    // |
	//  --> lcd_command(LCD_VOLUME_MODE_1)
	//						-> standard command 18
	//  --> lcd_command(LCD_VOLUME_MODE_2 | ((i) & 0x3F))
	//						-> command 18 with electronic volume value 0b011111
	
	//(19)
	LCD_SET_INDICATOR_OFF();            //switch indicator off, no blinking
	// |
	//  --> passt scho
	
  #endif
  #if DISPLAY_TYPE == 128
    LCD_SET_BIAS_RATIO_1_7();           //bias 1/7
    LCD_SET_POWER_CONTROL(7);           //power control mode: all features on
    LCD_SET_BIAS_VOLTAGE(7);            //set voltage regulator R/R
    LCD_SET_VOLUME_MODE(0x06);          //volume mode set
    LCD_SET_INDICATOR_OFF();            //switch indicator off, no blinking
  #endif
  #if DISPLAY_TYPE == 102
    LCD_SET_BIAS_RATIO_1_9();           //bias 1/9
    LCD_SET_POWER_CONTROL(7);           //power control mode: all features on
    LCD_SET_BIAS_VOLTAGE(3);            //set voltage regulator R/R
    LCD_SET_VOLUME_MODE(0x1F);          //volume mode set
    LCD_SET_ADV_PROG_CTRL(LCD_TEMPCOMP_HIGH);
  #endif
  
  
  uint8_t i,j;                        //clear display content
 
 for(j=0;j<LCD_RAM_PAGES;j++) {
    lcd_moveto_xy(j,0);   // array (uint8_t page, uint8_t column)
    for (i=0;i<LCD_WIDTH;i++)
      lcd_data(0);
    }

  lcd_moveto_xy(0,0);                 //Set write pointer
  LCD_SWITCH_ON();                    //Switch display on (1)
  return;
  }
/******************************************************************************
  * This function initializes the SPI device on an atmega8#
  */
void init_spi_lcd() {
   
   
   //set MOSI and SCK as output pins
   DDRB |= (1<<PINB5)|(1<<PINB7);
   
   //set A0, LCD_Reset as outputs
   DDRA |= (1<<PINA5) | (1<<PINA4);
   
   //set SS` as an output pin
   DDRB |= (1<<PINB4);
   
   
   /* SPI control register:
		SPIE: SPI Interrupt Enable
		SPE: 	SPI Enable
		MSTR: 	Select as Master
		CPOL:   Clock Polarity -> leading edge  = falling edge
							   -> trailing edge = rising edge
		CPHA:	Clock Phase -> Leading edge  = setup
							   Trailing edge = sample
   */
   SPCR = (0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (1<<CPOL) | (1<<CPHA) | (0<<SPR1) | (0<<SPR0);
         
   
   
   //SPI status register
   //SPI2X: Double SPI Speed Bit -> minimum SCK period will be two CPU clock periods
   SPSR = (1<<SPI2X);

   SPDR = LCD_NOP; //Do not use 0 here, only LCD_NOP is allowed!
   

   
   }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
