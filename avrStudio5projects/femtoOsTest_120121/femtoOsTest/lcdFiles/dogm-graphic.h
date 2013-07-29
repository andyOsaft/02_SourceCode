#ifndef DOGMGRAPHIC_H_INCLUDED
#define DOGMGRAPHIC_H_INCLUDED

#include <avr/io.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
//#include "font.h"
#include "globals.h"


/*****************************************************************************
 * BEGIN CONFIG BLOCK
 *****************************************************************************/
//Select the display type: DOGS102: 102, DOGM128/DOGL128: 128, DOGM132: 132
#define DISPLAY_TYPE  132

//Should chip select (CS) be used?
#define LCD_USE_CHIPSELECT  1

//Use Backlight?  (0: no backlight, 1: backlight (on when pin is high), 2: backlight (on when pin is low))
#define LCD_USE_BACKLIGHT   0

//A0 Port (here: PortA.5)
#define PORT_A0  PORTA
#define DDR_A0   DDRA
#define PIN_A0   PINA5

//Reset Port (here PortA.4)
#define PORT_RST PORTA
#define DDR_RST  DDRA
#define PIN_RST  PINA4

//Backlight Port
#if LCD_USE_BACKLIGHT != 0
  #define PORT_LED PORTB
  #define DDR_LED  DDRB
  #define PIN_LED  4
#endif

//Chip select (here: PortB.4)
#if LCD_USE_CHIPSELECT == 1
  #define PORT_CS  PORTB
  #define DDR_CS   DDRB
  #define PIN_CS   PINB4
#endif

//22: NOP command
#define LCD_NOP    0xE3 


//SPI routines
//Define a function that initializes  the SPI interface, see below for an example
void init_spi_lcd(void);
#define LCD_INIT_SPI() init_spi_lcd()

//Define a function that waits until SPI interface is idle
// wait until the SPI interrupt flag is cleared
//was: #define spi_wait_for_idle() while(! (UCSR1A & _BV(TXC1))); UCSR1A |= _BV(TXC1)
#define spi_wait_for_idle() while(!(SPSR & (1<<SPIF)));

//Define how to write to SPI data register
//was: #define spi_write(i) UDR1 = i
#define spi_write(i) SPDR = i

//Define this if LCD Output should continue in next line when reaching edge of display
//Used for all outputs. To enable this feature for text only, use the appropriate flag in font.h
#define LCD_WRAP_AROUND  1

//Include graphic functions, i.e. lcd_draw_image_P, lcd_draw_image_xy_P, lcd_clear_area ? 
#define LCD_INCLUDE_GRAPHIC_FUNCTIONS  1

//*Example SPI setup (Atmega162)
//*init spi: msb first, update on falling edge , read on rising edge, 9 MHz

//*****************************************************************************
// * END CONFIG BLOCK
//*****************************************************************************/

/*****************************************************************************
 * Command Codes
 *****************************************************************************/
//1: Display on/off
#define LCD_DISPLAY_ON        0xAF  //switch display on
#define LCD_DISPLAY_OFF       0xAE  //switch display off

//2: display start line set (lower 6 bits select first line on lcd from 64 lines in memory)
#define LCD_START_LINE        0x40

//3: Page address set (lower 4 bits select one of 8 pages)
#define LCD_PAGE_ADDRESS      0xB0

//4: column address (lower 4 bits are upper / lower nibble of column address)
#define LCD_COL_ADDRESS_MSB   0x10
#define LCD_COL_ADDRESS_LSB   0x00  //second part of column address

//8: select orientation (black side of the display should be further away from viewer)
#define LCD_BOTTOMVIEW        0xA1  //6 o'clock view 
#define LCD_TOPVIEW           0xA0  //12 o'clock view

//9: select normal (white background, black pixels) or reverse (black background, white pixels) mode
#define LCD_DISPLAY_POSITIVE  0xA6  //not inverted mode
#define LCD_DISPLAY_INVERTED  0xA7  //inverted display

//10: show memory content or switch all pixels on
#define LCD_SHOW_NORMAL       0xA4  //show dram content
#define LCD_SHOW_ALL_POINTS   0xA5  //show all points

//11: lcd bias set
#define LCD_BIAS_1_9          0xA2
#define LCD_BIAS_1_7          0xA3

//14: Reset Controller
#define LCD_RESET_CMD         0xE2

//15: output mode select (turns display upside-down)
#define LCD_SCAN_DIR_NORMAL   0xC0  //normal scan direction
#define LCD_SCAN_DIR_REVERSE  0xC8  //reversed scan direction

//16: power control set (lower 3 bits select operating mode)
//Bit 0: Voltage follower on/off - Bit 1: Voltage regulator on/off - Bit 2: Booster circuit on/off
#define LCD_POWER_CONTROL     0x28  //base command
#define LCD_POWER_LOW_POWER   0x2F
#define LCD_POWER_WIDE_RANGE  0x2F 
#define LCD_POWER_LOW_VOLTAGE 0x2B 

//17: voltage regulator resistor ratio set (lower 3 bits select ratio)
//selects lcd voltage - 000 is low (~ -2V), 111 is high (~ - 10V), also depending on volume mode. Datasheet suggests 011
#define LCD_VOLTAGE           0x20

//18: Volume mode set (2-byte command, lower 6 bits in second word select value, datasheet suggests 0x1F)
#define LCD_VOLUME_MODE_1     0x81
#define LCD_VOLUME_MODE_2     0x00

#if DISPLAY_TYPE == 128 || DISPLAY_TYPE == 132
//19: static indicator (2-byte command), first on/off, then blinking mode
  #define LCD_INDICATOR_ON       0xAD  //static indicator on
  #define LCD_INDICATOR_OFF      0xAC  //static indicator off
  #define LCD_INDICATOR_MODE_OFF 0x00
  #define LCD_INDICATOR_MODE_1HZ 0x01
  #define LCD_INDICATOR_MODE_2HZ 0x10
  #define LCD_INDICATOR_MODE_ON  0x11

//20: booster ratio set (2-byte command)
  #define LCD_BOOSTER_SET       0xF8  //set booster ratio
  #define LCD_BOOSTER_234       0x00  //2x-4x
  #define LCD_BOOSTER_5         0x01  //5x
  #define LCD_BOOSTER_6         0x03  //6x
#endif
  

#if DISPLAY_TYPE == 102
//25: advanced program control 
  #define LCD_ADV_PROG_CTRL     0xFA
  #define LCD_ADV_PROG_CTRL2    0x10
#endif



/*****************************************************************************
 * Public Functions
 *****************************************************************************/

//initializes the display in 6 o'clock mode & 4x booster for 2.4-3.3V supply voltage
//for EA-DOGM132 and EA_DOGM128
void lcd_init();

//write data word or command to the LCD
void lcd_data     (uint8_t data);
void lcd_command  (uint8_t cmd);


//Function to read the current position
extern uint8_t lcd_current_page;
extern uint8_t lcd_current_column;
static inline uint8_t lcd_get_position_page()   {return lcd_current_page;};
static inline uint8_t lcd_get_position_column() {return lcd_current_column;};

//User functions to write raw data from flash
#if LCD_INCLUDE_GRAPHIC_FUNCTIONS >= 1
  void lcd_draw_image_P(PGM_P progmem_image, uint8_t pages, uint8_t columns, uint8_t style);
  void lcd_draw_image_xy_P(PGM_P progmem_image, uint8_t x, uint8_t y, uint8_t pages, uint8_t columns, uint8_t style);
#endif

void lcd_clear_area(uint8_t pages, uint8_t columns, uint8_t style);
void lcd_clear_area_xy(uint8_t pages, uint8_t columns, uint8_t style, uint8_t col, uint8_t page);
  

//Move cursor
void lcd_moveto_xy  (uint8_t page, uint8_t column);
void lcd_move_xy    (int8_t pages, int16_t columns);



//Text functions are included in font.c / font.h

/*****************************************************************************
 * LCD Size, based on type selection above
 *****************************************************************************/
#if DISPLAY_TYPE == 132
  #define LCD_WIDTH     132    //width of the LCD
  #define LCD_HEIGHT    32     //height of the LCD
  #define LCD_RAM_PAGES 4      //size of LCD RAM
#endif

#if DISPLAY_TYPE == 128
  #define LCD_WIDTH     128    //width of the LCD
  #define LCD_HEIGHT    64     //height of the LCD
  #define LCD_RAM_PAGES 8      //size of LCD RAM
#endif

#if DISPLAY_TYPE == 102
  #define LCD_WIDTH     102    //width of the LCD
  #define LCD_HEIGHT    64     //height of the LCD
  #define LCD_RAM_PAGES 8      //size of LCD RAM
#endif


/*****************************************************************************
 * Makros to execute commands
 *****************************************************************************/

#define LCD_SWITCH_ON()              lcd_command(LCD_DISPLAY_ON)
#define LCD_SWITCH_OFF()             lcd_command(LCD_DISPLAY_OFF)
#define LCD_SET_FIRST_LINE(i)        lcd_command(LCD_START_LINE | ((i) & 0x3F))
#define LCD_SET_PAGE_ADDR(i)         lcd_command(LCD_PAGE_ADDRESS | ((i) & 0x0F))
#define LCD_SET_COLUMN_ADDR(i)       lcd_command(LCD_COL_ADDRESS_MSB | ((i>>4) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_LSB | ((i) & 0x0F))
#define LCD_GOTO_ADDRESS(page,col);  lcd_command(LCD_PAGE_ADDRESS | ((page) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_MSB | ((col>>4) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_LSB | ((col) & 0x0F));
                                     
#define LCD_SET_BOTTOM_VIEW()        lcd_command(LCD_BOTTOMVIEW)
#define LCD_SET_TOP_VIEW()           lcd_command(LCD_TOPVIEW)
#define LCD_SET_MODE_POSITIVE()      lcd_command(LCD_DISPLAY_POSITIVE)
#define LCD_SET_MODE_INVERTED()      lcd_command(LCD_DISPLAY_INVERTED)
#define LCD_SHOW_ALL_PIXELS_ON()     lcd_command(LCD_SHOW_ALL_POINTS)
#define LCD_SHOW_ALL_PIXELS_OFF()    lcd_command(LCD_SHOW_NORMAL)
#define LCD_SET_BIAS_RATIO_1_7()     lcd_command(LCD_BIAS_1_7)
#define LCD_SET_BIAS_RATIO_1_9()     lcd_command(LCD_BIAS_1_9)
#define LCD_SEND_RESET()             lcd_command(LCD_RESET_CMD)
#define LCD_ORIENTATION_NORMAL()     lcd_command(LCD_SCAN_DIR_NORMAL)
#define LCD_ORIENTATION_UPSIDEDOWN() lcd_command(LCD_SCAN_DIR_REVERSE)
#define LCD_SET_POWER_CONTROL(i)     lcd_command(LCD_POWER_CONTROL | ((i) & 0x07))
#define LCD_SET_LOW_POWER()          lcd_command(LCD_POWER_LOW_POWER)
#define LCD_SET_WIDE_RANGE()         lcd_command(LCD_POWER_WIDE_RANGE)
#define LCD_SET_LOW_VOLTAGE()        lcd_command(LCD_POWER_LOW_VOLTAGE)
#define LCD_SET_BIAS_VOLTAGE(i)      lcd_command(LCD_VOLTAGE | ((i) & 0x07))                                   
#define LCD_SET_VOLUME_MODE(i)       lcd_command(LCD_VOLUME_MODE_1); \
                                     lcd_command(LCD_VOLUME_MODE_2 | ((i) & 0x3F))
#if DISPLAY_TYPE == 128 || DISPLAY_TYPE == 132
  #define LCD_SET_INDICATOR_OFF()      lcd_command(LCD_INDICATOR_OFF); \
                                       lcd_command(LCD_INDICATOR_MODE_OFF)
  #define LCD_SET_INDICATOR_STATIC()   lcd_command(LCD_INDICATOR_ON); \
                                       lcd_command(LCD_INDICATOR_MODE_ON)
  #define LCD_SET_INDICATOR_1HZ()      lcd_command(LCD_INDICATOR_ON); \
                                       lcd_command(LCD_INDICATOR_MODE_1HZ)
  #define LCD_SET_INDICATOR_2HZ()      lcd_command(LCD_INDICATOR_ON); \
                                       lcd_command(LCD_INDICATOR_MODE_2HZ)
  #define LCD_SET_INDICATOR(i,j)       lcd_command(LCD_INDICATOR_OFF | ((i) & 1)); \
                                       lcd_command(((j) & 2))

  #define LCD_SLEEP_MODE               lcd_command(LCD_INDICATOR_OFF); \
                                       lcd_command(LCD_DISPLAY_OFF); \
                                       lcd_command(LCD_SHOW_ALL_POINTS)
  #define LCD_SET_BOOSTER_RATIO()		   lcd_command(LCD_BOOSTER_234)
#endif

#if DISPLAY_TYPE == 102
  #define LCD_TEMPCOMP_HIGH  0x80
  #define LCD_COLWRAP        0x02
  #define LCD_PAGEWRAP       0x01
  #define LCD_SET_ADV_PROG_CTRL(i)     lcd_command(LCD_ADV_PROG_CTRL); \
                                       lcd_command(LCD_ADV_PROG_CTRL2 & i)
#endif

/*****************************************************************************
 * Output pin controlling makros
 *****************************************************************************/

//Control A0 input of LCD
#define LCD_DRAM()            PORT_A0 |= _BV(PIN_A0)
#define LCD_CMD()             PORT_A0 &= ~_BV(PIN_A0)
#define LCD_SET_OUTPUT_A0()   DDR_A0  |= _BV(PIN_A0)

//Control reset input of LCD
#define LCD_RESET_OFF()       PORT_RST |= _BV(PIN_RST)
#define LCD_RESET_ON()        PORT_RST &= ~_BV(PIN_RST)
#define LCD_SET_OUTPUT_RST()  DDR_RST |= _BV(PIN_RST)
#define LCD_RESET()           LCD_RESET_ON();  _delay_ms(1); LCD_RESET_OFF()

//Control pin for Backlight
#if LCD_USE_BACKLIGHT == 1      //normal
  #define LCD_SET_OUTPUT_LED() DDR_LED |= _BV(PIN_LED)
  #define BACKLIGHT_ON()       PORT_LED |= (1<<PIN_LED)
  #define BACKLIGHT_OFF()      PORT_LED &= ~(1<<PIN_LED)
#elif LCD_USE_BACKLIGHT == 2    //inverted
  #define LCD_SET_OUTPUT_LED() DDR_LED |= _BV(PIN_LED)
  #define BACKLIGHT_ON()       PORT_LED &= ~(1<<PIN_LED)
  #define BACKLIGHT_OFF()      PORT_LED |= (1<<PIN_LED)
#else
  #define LCD_SET_OUTPUT_LED() 
  #define BACKLIGHT_ON()   
  #define BACKLIGHT_OFF()    
#endif

//Control pin for chip select
#if LCD_USE_CHIPSELECT == 1
  #define LCD_SET_OUTPUT_CS()  DDR_CS  |= _BV(PIN_CS)
  #define LCD_SELECT()         PORT_CS &= ~_BV(PIN_CS)
  #define LCD_UNSELECT()       spi_wait_for_idle(); PORT_CS |= _BV(PIN_CS)
#else
  #define LCD_SET_OUTPUT_CS()  
  #define LCD_SELECT()         spi_wait_for_idle();
  #define LCD_UNSELECT()   
#endif

//combined direction selection for all pins
#define LCD_SET_PIN_DIRECTIONS()   LCD_SET_OUTPUT_A0(); \
                                   LCD_SET_OUTPUT_RST(); \
                                   LCD_SET_OUTPUT_LED(); \
                                   LCD_SET_OUTPUT_CS()


//Bit positions for style settings
#define NORMAL      0
#define INVERT      4
#define INVERT_BIT  4


#endif

