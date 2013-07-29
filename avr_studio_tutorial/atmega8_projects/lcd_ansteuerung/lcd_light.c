//--------------------------------------------------------------------------------------------------------------
//	lcd_light(...) - schaltet die LCD Hindergrundbeleuchtung ein oder aus
// 	PE: on_off=0...1 = Beleuchtung Aus- bzw. Einschalten
//--------------------------------------------------------------------------------------------------------------

void lcd_light(bool on_off)
{
	//wenn dimmer an -> PWM deinit
	if (lcd_dimmer_is_init)
	{
		TCCR1A=0;
		lcd_dimmer_is_init=false;
	}

	DDRB |= (1<<BIT1);		//PortB1 als Ausgang
	if(on_off)
		PORTB |= (1<<BIT1);	//an

	else
		PORTB &= ~(1<<BIT1);//aus
}
//--------------------------------------------------------------------------------------------------------------
