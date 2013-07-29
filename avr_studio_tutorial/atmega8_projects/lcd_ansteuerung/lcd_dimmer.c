//--------------------------------------------------------------------------------------------------------------
//	lcd_dimmer(...) - legt die Helligkeit der Hintergrundbeleuchtung fest
//	PE: light = 0..255 Helligkeit der Anzeige
//--------------------------------------------------------------------------------------------------------------

void lcd_dimmer(char light)
{	
	//init
	if (!lcd_dimmer_is_init)
	{
		DDRB |= (1<<BIT1);		//Port B.1 als Ausgang
		TCCR1A=0x81;			//PWM Mode 1, Channel A
		TCCR1B=0x02;			//PWM Frequenz CLK/8
		lcd_dimmer_is_init=true;
	}
	OCR1A=light;		//PWM Vergleichswert
}
//--------------------------------------------------------------------------------------------------------------
