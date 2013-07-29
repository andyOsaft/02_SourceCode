// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
void lcd_data(unsigned char temp1);
void lcd_string(char *data);
void lcd_command(unsigned char temp1);
void lcd_enable(void);
void lcd_init(void);
void lcd_home(void);
void lcd_clear(void);
void set_cursor(uint8_t x, uint8_t y);
 
// Hier die verwendete Taktfrequenz in Hz eintragen, wichtig!
 
//#define F_CPU 4000000			//modifiziert
 
// LCD Befehle
 
#define CLEAR_DISPLAY 0x01
#define CURSOR_HOME   0x02
 
// Pinbelegung für das LCD, an verwendete Pins anpassen
 
#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_RS        PD2		//modifiziert
#define LCD_EN        PD3		//modifiziert

// DB4 bis DB7 des LCD sind mit PD0 bis PD3 des AVR verbunden 


#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7
