//uint16_t steht für einen 16-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 65535. 
//uint8_t steht für einen 8-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 255 


#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/sleep.h>
#include <stdint.h>


//my includes
#include "lcdRoutines.h"
#include "timerInit.h"
#include "heatController.h"
#include "debounce.h"
#include "pwmInit.h"
#include "adcInit.h"
#include "myInit.h"
#include "interrupt.h"
#include "menuScreens.h"
#include "global.h"


// durch den #define expandiert das Makro EXTERN in globals.h zu
// einem Leerstring. Dadurch wird aus
// EXTERN int a;
// die Zeile
// int a;
// und das definiert die Variable a
#define EXTERN

int main (void) {            
 
//-------------------------------------------------------------------------------------	
	//Variablen

	//PWM-Wert für Heizungsservos
	//MIN = 31 (1ms)
	//MAX = 63 (2ms)
	//uint16_t pwmWert=32;
	uint16_t pwmWert=62;	

	//System Modes
	enum ControllerStates
	{ 
		heatControlManualMode,
		heatControlAutoMode, 
		voltageControlMode, 
		tempInfoMode, 
		versionInfoMode
	} currentMode;
	
	//flag um zu speichern ob heat- manual oder auto mode zuletzt aktiv war
	//=1 -> Manual Mode
	//=2 -> Auto Mode
	uint8_t heatControlFlag=0;
	
	//Flags für Buttons(Menü-Button)
	//gedrückt       -> =0
	//lange gedrückt -> =2
	//ungedrückt     -> =1
	uint8_t buttonLinks=1;
	uint8_t buttonMitte=1;
	uint8_t buttonRechts=1;

	//Temperatur Variable für den Heat Control Auto Mode
	//Wert in °C
	//Uint16_t wegen Umwandlung in string mit utoa funktion
	uint16_t tempSollwert=20;

	//Variablen für die beiden Batterie-Spannungen
	uint16_t batt1Volt=0;
	uint16_t batt2Volt=0;

	//Variablen für die beiden Temperatur-Sensor Spannungen
	//Umrechnung in Temperatur erfolgt in Display Routine.
	uint16_t temp1Volt=0;
	uint16_t temp2Volt=0;

	//Variable die im Manual Heizungs Mode die Anzahl der auf dem Display dargestellten
	//"Heiz-Punkte" einstellt.
	uint8_t heizLevel=0;


//-------------------------------------------------------------------------------------	
	

	for(;;);
		
	//Initialisierungen
	sleep_disable();
		
	myInit();
	adcInit();
	pwmInit(pwmWert);
	pwmUpdate(pwmWert);
	lcd_init();
	menuInit();
	
	//set system led
	PORTD |= (1<<BIT7);

	//Start system in heat control auto mode
	//display regarding screen -> gleichbleibend heizen
	currentMode = heatControlManualMode;
	//screenHeatingManual(2, &heizLevel);
	//_delay_ms(3000);

	//enable interrupts for wakeup sleepmode
	sei();

	
//-------------------------------------------------------------------------------------
	

	while(1)  
	{   
		_delay_ms(100);


		//button links lesen
 	 	//nutzt das Debounce-Makro (debounce.h)
		if ((debounce(PIND,PD0))) 
		{
			buttonLinks=0;
		}
		else
		{
			buttonLinks=1;
		}

		//button mitte lesen
		if ((debounce(PIND,PD3)))
		{
			buttonMitte=0;
		}
		else
		{
			buttonMitte=1;
		}

		//button rechts lesen
		if ((debounce(PIND,PD2)))
		{
			buttonRechts=0;
		}
		else
		{
			buttonRechts=1;
		}
		

		//wenn buttonLinks einmal gedrückt wurde -> nächster Mode
		//heatControlFlag abfragen um zu wissen welcher heatControlMode
		//zuletzt aktiv war.
		if (buttonLinks==0)
		{	
			if (currentMode==versionInfoMode)
				if (heatControlFlag==2)
					currentMode=heatControlAutoMode;
				else 
					currentMode=heatControlManualMode;
			else
			{	
				currentMode +=1;
			}
		}


		//Mode selector
		switch (currentMode)
		{
			//---------------------------------------------------------------------------------------------
			case heatControlManualMode:
				//button links auf langen Tastendruck prüfen
				//prüfen -> kurz warten -> wenn immer noch gedrückt -> = lange gedrückt
				if ((debounce(PIND,PD0)))
				{
					_delay_ms(200);
					if ((debounce(PIND,PD0))) 
					{
						buttonLinks=2;
					}
				}
				//wenn button links lange gedrückt wurde -> wechsle in den anderen heating-mode	
				if (buttonLinks==2)
				{
					currentMode=heatControlAutoMode;
					screenHeatingAuto(tempSollwert);
				}
				//button mitte -> heizungs-level verringern (auf display und servo)
				else if (buttonMitte==0)
				{
					screenHeatingManual(0, &heizLevel);
					if (pwmWert>64)
					{
						pwmWert -= 2;
					}
					else
					{
						pwmWert=62;
					}
					pwmUpdate(pwmWert);
				}
				//button rechts -> heizungs-level erhöhen(auf display und servo)
				else if (buttonRechts==0)
				{
					screenHeatingManual(1, &heizLevel);
					if (pwmWert<118)
					{
						pwmWert += 2;
					}
					else
					{
						pwmWert=120;
					}
					pwmUpdate(pwmWert);
				}
				//wenn nichts gedrückt wurde aktuellen heizlevel auf display anzeigen
				else
				{
					screenHeatingManual(2, &heizLevel);
				}
				break;
			//---------------------------------------------------------------------------------------------
			case heatControlAutoMode:
				//button links auf langen Tastendruck prüfen
				//prüfen -> kurz warten -> wenn immer noch gedrückt -> = lange gedrückt
				if ((debounce(PIND,PD0)))
				{
					_delay_ms(100);
					if ((debounce(PIND,PD0))) 
					{
						buttonLinks=2;
					}
				}
				//wenn button links lange gedrückt wurde -> wechsle in den anderen heating-mode	
				if (buttonLinks==2)
				{
					currentMode=heatControlManualMode;
					screenHeatingManual(2, &heizLevel);
				}
				//button Mitte -> temperatur Sollwert für Regler verringern
				else if (buttonMitte==0)
				{
					if (tempSollwert>17)
					{
						tempSollwert -= 1;
					}
					else
					{
						tempSollwert=16;
					}
					screenHeatingAuto(tempSollwert);
				}
				//button rechts -> temperatur sollwert für regler erhöhen
				else if (buttonRechts==0)
				{
					if (tempSollwert<25)
					{
						tempSollwert += 1;
					}
					else
					{
						tempSollwert=26;
					}
					screenHeatingAuto(tempSollwert);
				}
				break;
			//---------------------------------------------------------------------------------------------
			case voltageControlMode:
				//start adc single conversion for both batteries
				//both values are voltage*100
				batt1Volt = adcRead15(4);
				batt2Volt = adcRead15(5);
				screenVoltageControl(batt1Volt, batt2Volt);
				break;
			//---------------------------------------------------------------------------------------------
			case tempInfoMode:
				//start adc single conversion for both temperature sensors voltages
				//both values are voltage*100
				temp1Volt = adcRead5(6);
				temp2Volt = adcRead5(7);
				screenTempInfo(temp1Volt,temp2Volt);
				break;
			//---------------------------------------------------------------------------------------------
			case versionInfoMode:
				screenInfo();
				break;
			//---------------------------------------------------------------------------------------------
			default:
				break;		
		}
		

		//Folgende routinen versetzen den controller in den sleep-mode wenn die zündungsspannung aus ist.
		// Zündung=ON  -> Transistor schaltet 0V -> PIND.6 = 0
		// Zündung=OFF -> Transistor sperrt      -> PIND.6 = 1 (interner Pull-Up)
		// ->Sleep ON
		/*
		if (PIND & (1<<BIT6))	
		{						
			screenGotoSleepmode();
			_delay_ms(2000);
			lcd_clear();
			PORTC &= ~(1<<BIT0);//Display -> Spannungslos (Display-Masse wird abgeschaltet)
			PORTC |= (1<<BIT1); //DisplayBeleuchtung -> spannungslos (über Darlington 0V an TransistorBasis)
			sleep_enable();	   
			sleep_cpu();
		}			
		
		//Wenn uC gerade aufgewacht ist, dann willkommens und battVolt Screens darstellen.
		if (wakeUp==1)
		{
			menuInit();
			wakeUp=0;
		}
		*/

	}
}

  /*

		if (lcdIni==1)
		{
			lcd_init();	
			lcd_init();
			lcd_home();
			lcd_string("kalt        warm");
			for(tempDispPos=0;tempDispPos<=dispPos;tempDispPos++)
			{
				lcdHeizung(tempDispPos);
			}
			lcdIni=0;
		}

		if (heizungUpdate)
		{
			if (buttonWarm)
			{
				if (heizStufe<=maxHeizStufe)
				{	
					dispPos++;
					lcdHeizung(dispPos);
					dispPos++;
					lcdHeizung(dispPos);
					heizStufe++;
					buttonWarm=0;
					motorRechtslauf(anzahlSchritte);
				}
			}
			
			else //buttonKalt=gedrückt
			{
				if (heizStufe!=0)
				{
					lcdKuehlung(dispPos);
					dispPos--;
					lcdKuehlung(dispPos);
					dispPos--;
					heizStufe--;
					motorLinkslauf(anzahlSchritte);
					
				}
			}
			heizungUpdate=0;
		}
		
		if (PIND & (1<<BIT6))	// Zündung=ON  -> Transistor schaltet 0V -> PIND.6 = 0
		{						// Zündung=OFF -> Transistor sperrt      -> PIND.6 = 1 (interner Pull-Up)
			 					// '->Sleep ON
			PORTC &= ~(1<<BIT0);//Display -> Spannungslos (Display-Masse wird abgeschaltet)
			PORTC |= (1<<BIT1); //DisplayBeleuchtung -> spannungslos (über Darlington 0V an TransistorBasis)
			sleep_enable();	   
			sleep_cpu();
		}					   
   
   }	
   return 0;                 
}*/
