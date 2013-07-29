//--------------------------------------------------------------------------------------------------------------
//	lcd_goto(...) - setzt die Cursorposition
//	PE: 	row = Zeile 1/2
//			col = Spalte 1..16
//--------------------------------------------------------------------------------------------------------------
void lcd_goto(int row, int col)
{
	row--;					//Null-basierend
	row &= 0x01;			//sicherheitshalber
	row *=40;				//Zeile nach Bit 6 bringen
	col--;					//Null-basierend
	col &= 0x0f;			// sicherheitshalber
	char tmp = row | col;	//Adresse bilden
	tmp |= 0x80;			//Cursor setzen
	lcd_cmd(tmp);			//senden
}
//--------------------------------------------------------------------------------------------------------------
