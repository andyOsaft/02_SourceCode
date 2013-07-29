//--------------------------------------------------------------------------------------------------------------
//	lcd_send(...) - sendet ein Byte an LCD im 4-Bit-Modus
//	RS muss vorher richtig gesetzt sein
//	PE:	data= zu sendendes Byte
//--------------------------------------------------------------------------------------------------------------
void lcd_send(char data)
{
	//aktuelles RS ermitteln
	char rs=PORTD;
	rs &= 4;

	//High-Teil senden
	char tmp=data;
	tmp &= 0xf0;
	tmp |= rs;
	PORTD =tmp;

	//Schreibsignal
	PORTD |= (1<<BIT3);
	PORTD &= ~(1<<BIT3);

	//Low-Teil senden
	tmp=data;
	tmp &= 0x0f;
	tmp *= 16;
	tmp |= rs;
	PORTD=tmp;

	//Schreibsignal
	PORTD |= (1<<BIT3);
	PORTD &= ~(1<<BIT3);

	//verarbeiten lassen
	wait_ms(1);
}
//--------------------------------------------------------------------------------------------------------------
