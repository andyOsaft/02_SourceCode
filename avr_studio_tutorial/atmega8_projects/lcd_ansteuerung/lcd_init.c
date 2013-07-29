//--------------------------------------------------------------------------------------------------------------
//	lcd_init(...) - Initialisiert das myAVR LCD 16x2, 4 Bit Modus
//	PE: keine
//	PA: keine
//--------------------------------------------------------------------------------------------------------------
void lcd_init()
{
	//Port D = Ausgang
	DDRD = 0x0ff;
	PORTD = 0;
	
	//warten bis LCD-Controller gebootet
	wait_ms(50);

	//4-BitModus einschalten
	PORTD = 0x20;

	//Schreibsignal Enable-Impuls
	PORTD |= (1<<BIT3);
	PORTD &= ~(1<<BIT3);

	//Zeit zum Umschalten lassen
	wait_ms(5);

	//ab hier im 4-Bit Modus
	//Funktions-Set: 2 Zeilen, 5x7 Matrix, 4 Bit
	lcd_cmd(0x28);
	lcd_off();
	lcd_clear();

	//Entry Mode
	lcd_cmd(0x06);
	lcd_on();
}
//--------------------------------------------------------------------------------------------------------------
