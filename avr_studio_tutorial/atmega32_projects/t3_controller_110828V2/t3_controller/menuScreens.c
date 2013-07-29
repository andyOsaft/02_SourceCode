/********************************************************************************

Diese Datei enthält alle vorhandenen Menü-Anzeigen für das LCD-Display.

Das System enthält verschiedene "modes" mit jeweils passenden Display Anzeigen.

Die verschiedenen Screens sind im Header-File "menuScreens.h" gelistet.





*********************************************************************************/



#include "menuScreens.h"
/*--------------------------------------------------------------------------------
Funktion: void menuInit()

Aufgabe:
Beim Starten und Aufwachen (SleepMode) des Systems soll erst der Willkommens
Bildschirm und dann der Batterie-Spannungs Bildschirm angezeigt werden.

--------------------------------------------------------------------------------*/
void menuInit()
{
	uint16_t batt1Volt=0;
	uint16_t batt2Volt=0;	

	screenWelcome();
	_delay_ms(2000);
	
	
	batt1Volt = adcRead15(4);
	batt2Volt = adcRead15(5);
	screenVoltageControl(batt1Volt, batt2Volt);
	_delay_ms(3000);
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenWelcome()

Aufgabe: 
Wird beim Systemstart angezeigt.

--------------------------------------------------------------------------------*/
void screenWelcome()
{
	lcd_clear();
    lcd_string("Welcome!");
	set_cursor(0,2);
	lcd_string("Starting system.");
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenHeatingManual()

Aufgabe: 
Stellt den Inhalt des HeatingManualMode auf dem LCD display dar.

Der übergebene Parameter "heizStatus" regelt, ob die Heizstufe erhöht oder 
abgesenkt wird.
0 -> absenken
1 -> erhöhen
2 -> gleichbleibend
Die Heizstufe wird über über die Anzahl der "Heizblöcke" dargestellt. Ein Heiz-
block ist eine Displayzelle in der alle Pixel gesetzt sind.
Die aktuelle Anzahl der Heizblöcke (und damit die ServoPosition) wird in der 
globalen Variable "heizLevel" gespeichert.

- 16 Heizlevel Stufen

--------------------------------------------------------------------------------*/
void screenHeatingManual(uint8_t heizStatus, uint8_t *heizLevel)
{
   lcd_clear();
   lcd_home();
   lcd_string("cold        warm");
   
   if ((heizStatus==1) && ((*heizLevel) != 15)) //mehr heizen
   {
   	   //setze nächsten Heizblock
   	   (*heizLevel)++;
	   set_cursor((*heizLevel),2);
   	
	   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
	   //High Teil des Zeichens ausgeben 			
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
	   LCD_PORT |= 0xF0;                //das high nibble von PORTD wird auf 0b1111 gesetzt
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   //Low Teil des Zeichens ausgeben
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
	   LCD_PORT |= 0xF0;                //das high nibble von temp2 wird PORTD zugewiesen
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   _delay_us(42);
	}
	else if ((heizStatus==0) && ((*heizLevel)!=0)) //weniger heizen
	{
	   //lösche aktuellen Heizblock
	   set_cursor((*heizLevel),2);
   	
	   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
	   //High Teil des Zeichens ausgeben 			
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
	   LCD_PORT |= 0x80;                //das high nibble von PORTD wird auf 0b1111 gesetzt
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   //Low Teil des Zeichens ausgeben
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
	   LCD_PORT |= 0x80;                //das high nibble von temp2 wird PORTD zugewiesen
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   _delay_us(42);
	   (*heizLevel)--;
	}
	else if (heizStatus==2) //gleichbleibend heizen
	{
	   set_cursor((*heizLevel),2);
   	
	   LCD_PORT |= (1<<LCD_RS);        	// RS auf 1 setzen
 
	   //High Teil des Zeichens ausgeben 			
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt				
	   LCD_PORT |= 0xF0;                //das high nibble von PORTD wird auf 0b1111 gesetzt
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   //Low Teil des Zeichens ausgeben
	   LCD_PORT &= 0x0F;				//das high nibble in PORTD wird auf null gesetzt
	   LCD_PORT |= 0xF0;                //das high nibble von temp2 wird PORTD zugewiesen
	   lcd_enable();					//E wird kurz gesetzt und dann wieder zurückgesetzt
   
	   _delay_us(42);
	}
	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenHeatingAuto(uint16_t targetTemp)

Aufgabe:
Stellt den Heating Auto Mode auf dem LCD-display dar.Der parameter targetTemp
beinhaltet die Ziel-Temperatur des Heizungs-Reglers.


--------------------------------------------------------------------------------*/
void screenHeatingAuto(uint16_t targetTemp)
{
	char buffer[4];
	
	lcd_clear();
    lcd_string("Temperature:");
	set_cursor(0,2);
	utoa(targetTemp, buffer, 10);
	lcd_string(buffer);	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenVoltageControl(uint16_t batt1Volt, uint16_t batt2Volt)

Aufgabe:
Stellt den Voltage Control Mode auf dem LCD-display dar. Die Parameter 
beinhalten die jeweiligen Spannungen der beiden Batterien im VW-Bus.

Spannungen sind Komma-Werte x.xxV mal 100 genommmen um float operationen
zu vermeiden; z.B. 12.5 -> 1250


--------------------------------------------------------------------------------*/
void screenVoltageControl(uint16_t batt1Volt, uint16_t batt2Volt)
{
	char buffer[4];
	uint16_t voltageBuffer;	

	lcd_clear();
	
	//Daten von Batterie1
    lcd_string("Batt1: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = (batt1Volt+(uint16_t)(100>>1))/(uint16_t)100;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = batt1Volt%(uint16_t)100;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" V");

	//Daten von Batterie2	
	set_cursor(0,2);
	lcd_string("Batt2: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = (batt2Volt+(uint16_t)(100>>1))/(uint16_t)100;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = batt2Volt%(uint16_t)100;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" V");
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenVoltageThreshold(uint16_t batt1Thres, uint16_t batt2Thres)

Aufgabe:
Stellt den zugehörigen screen auf dem lcd-Display dar, wenn der user die 
Grenzwerte für den Batterie-Warner veränder will. (Innerhalb des Voltage
Control Mode.)

!!!!die alarme bei unterschreiten der batterie grenzwerte sind noch nicht implementiert!!!

--------------------------------------------------------------------------------*/
void screenVoltageThreshold(uint16_t batt1Thres, uint16_t batt2Thres)
{
	char buffer[4];
	uint16_t voltageBuffer;	

	lcd_clear();
	
	//Daten von Batterie1
    lcd_string("Threshold1: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = batt1Thres/(uint16_t)10;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = batt1Thres%(uint16_t)10;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" V");

	//Daten von Batterie2	
	set_cursor(0,2);
	lcd_string("Threshold2: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = batt2Thres/(uint16_t)10;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = batt2Thres%(uint16_t)10;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" V");
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenGotoSleepmode()

Aufgabe:
Wenn der uC in den SleepMode wechselt wird dieser Screen dargestellt.


--------------------------------------------------------------------------------*/
void screenGotoSleepmode()
{
	lcd_clear();
    lcd_string("Entering");
	set_cursor(0,2);
	lcd_string("Sleepmode...");	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenWakeupSleepmode()

Aufgabe:
Wenn der uC aus dem SleepMode aufwacht wird dieser Screen dargestellt.


--------------------------------------------------------------------------------*/
void screenWakeupSleepmode()
{
	lcd_clear();
    lcd_string("Wake up from");
	set_cursor(0,2);
	lcd_string("Sleepmode...");	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenInfo()

Aufgabe:
Stellt Informationen über die aktuelle Firmware und das Datum zur Firmware dar.
Wird im InfoMode gecallt.


--------------------------------------------------------------------------------*/
void screenInfo()
{
	lcd_clear();
    lcd_string("Firmware: V0.3");
	set_cursor(0,2);
	lcd_string("Date: 10.05.11");	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenLightWarning()

Aufgabe:
Diese Nachricht wird auf dem Lcd angezeigt wenn die Zündung aus und das 
Abblendlicht noch an ist.

Wird beim Ausschalten des Abblendlichtes automatisch zurückgesetzt.
--------------------------------------------------------------------------------*/
void screenLightWarning()
{
	lcd_clear();
    lcd_string("Warning!");
	set_cursor(0,2);
	lcd_string("Light still on!");
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenWarningBatt1()

Aufgabe:
Wird automatisch angezeigt wenn die Spannung von Batterie 1 unter die 
eingestellte "Threshold" Schwelle sinkt.

--------------------------------------------------------------------------------*/
void screenWarningBatt1()
{
	lcd_clear();
    lcd_string("Battery 1");
	set_cursor(0,2);
	lcd_string("below threshold!");	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenWarningBatt2()

Aufgabe:
Wird automatisch angezeigt wenn die Spannung von Batterie 2 unter die 
eingestellte "Threshold" Schwelle sinkt.

--------------------------------------------------------------------------------*/
void screenWarningBatt2()
{
	lcd_clear();
    lcd_string("Battery 2");
	set_cursor(0,2);
	lcd_string("below threshold!");	
}
//--------------------------------------------------------------------------------
/*--------------------------------------------------------------------------------

Funktion: void screenTempInfo(uint16_t tempInside, uint16_t tempMotor)

Aufgabe:
Stellt die Temperatur-Werte vom Bus-Innenraum und vom Bus-Motorraum auf 
dem LCD-display dar.

Die beiden Parameter sind Spannungen*100.

Mit der Formel der Ausgleichsgerade werden jetzt die Spannungswerte in
Temperaturwerte umgerechnet. 
Spannung ist *100: 
- 144,94 -> 145
- 143,77 -> 14377

Rechnung:

temp = 145 * volt - 14377

-> temp ist Temperatur*100

-> Ergebnis der Multiplikation passt noch in 16 bit register.


--------------------------------------------------------------------------------*/
void screenTempInfo(uint16_t voltInside, uint16_t voltMotor)
{
	char buffer[4];
	uint16_t voltageBuffer;	
	uint16_t tempInside=0;
	uint16_t tempMotor=0;

	//lcd_clear();
	//utoa(voltageBuffer, buffer,10);
	//lcd_string(buffer);

	tempInside = (voltInside*(uint16_t)145)-14377;
	tempMotor = (voltMotor*(uint16_t)145)-14377;
	

	lcd_clear();
	
	//Daten vom Temperatur-Sensor im Innenraum
    lcd_string("T inside: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = (tempInside+(uint16_t)(100>>1))/(uint16_t)100;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = tempInside%(uint16_t)100;
	//Nur eine Nachkommastelle berücksichtigen
	voltageBuffer = (voltageBuffer+(uint16_t)(100>>1))/(uint16_t)10;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" C");

/*
	//Daten vom Temp-Sensor im Motorraum
	set_cursor(0,2);
	lcd_string("T motor: ");
	//Vorkomma-Stelle ausgeben
	voltageBuffer = tempMotor/100;	
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(",");
	//Nachkomma-Stelle ausgeben
	voltageBuffer = tempMotor%100;
	//Nur eine Nachkomma-Stelle berücksichtigen
	voltageBuffer = voltageBuffer/10;
	utoa(voltageBuffer, buffer, 10);
	lcd_string(buffer);
	lcd_string(" C");
*/
}
























