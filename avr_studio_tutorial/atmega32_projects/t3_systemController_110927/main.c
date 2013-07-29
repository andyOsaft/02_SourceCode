//uint16_t steht f�r einen 16-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 65535. 
//uint8_t steht f�r einen 8-Bit Integer ohne Vorzeichen (unsigned int) mit einem Wertebereich von 0 bis 255 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/sleep.h>
#include <stdint.h>



// durch den #define expandiert das Makro EXTERN in globals.h zu
// einem Leerstring. Dadurch wird aus
// EXTERN int a;
// die Zeile
// int a;
// und das definiert die Variable a
#define EXTERN


//my includes
#include "lcdRoutines.h"
#include "timerInit.h"
#include "heatController.h"
#include "globals.h"
#include "debounce.h"
#include "pwmInit.h"
#include "adcInit.h"
#include "myInit.h"
#include "interrupt.h"
#include "menuScreens.h"




int main (void) {            
 
//-------------------------------------------------------------------------------------	
	//Variablen

	//PWM-Wert f�r Heizungsservos
	//MIN = 31 (1ms)
	//MAX = 63 (2ms)
	volatile uint16_t pwmWert=31;
	
	//System Modes
	volatile enum TrafficLightState
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
	volatile uint8_t heatControlFlag=0;
	
	//Flags f�r Buttons(Men�-Button)
	//gedr�ckt       -> =0
	//lange gedr�ckt -> =2
	//ungedr�ckt     -> =1
	volatile uint8_t buttonLinks=1;
	volatile uint8_t buttonMitte=1;
	volatile uint8_t buttonRechts=1;

	//Temperatur Variable f�r den Heat Control Auto Mode
	//Wert in �C
	//Uint16_t wegen Umwandlung in string mit utoa funktion
	volatile uint16_t tempSollwert=20;

	//Variablen f�r die beiden Batterie-Spannungen
	volatile uint16_t batt1Volt=0;
	volatile uint16_t batt2Volt=0;

	//Variablen f�r die beiden Temperatur-Sensor Spannungen
	//Umrechnung in Temperatur erfolgt in Display Routine.
	volatile uint16_t temp1Volt=0;
	volatile uint16_t temp2Volt=0;

	//Variable die im Manual Heizungs Mode die Anzahl der auf dem Display dargestellten
	//"Heiz-Punkte" einstellt.
	volatile uint8_t heizLevel=0;


//-------------------------------------------------------------------------------------	
	
	
	//Initialisierungen
	pwmWert=88;		
	myInit();
	adcInit();
	pwmInit(pwmWert);
	lcd_init();
	menuInit();

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

		pwmWert=99;
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
		

		//wenn buttonLinks einmal gedr�ckt wurde -> n�chster Mode
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
				currentMode +=1;
		}


		//Mode selector
		switch (currentMode)
		{
			//----------------------------------------------------------------
			case heatControlManualMode:
				//button links auf langen Tastendruck pr�fen
				//pr�fen -> kurz warten -> wenn immer noch gedr�ckt -> = lange gedr�ckt
				if ((debounce(PIND,PD0)))
				{
					_delay_ms(200);
					if ((debounce(PIND,PD0))) 
					{
						buttonLinks=2;
					}
				}
				//wenn button links lange gedr�ckt wurde -> wechsle in den anderen heating-mode	
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
					pwmInit(pwmWert);
					//pwmUpdate(pwmWert);
				}
				//button rechts -> heizungs-level erh�hen(auf display und servo)
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
					pwmInit(pwmWert);
					//pwmUpdate(pwmWert);
				}
				//wenn nichts gedr�ckt wurde aktuellen heizlevel auf display anzeigen
				else
				{
					screenHeatingManual(2, &heizLevel);
				}
				break;
			//----------------------------------------------------------------
			case heatControlAutoMode:
				//button links auf langen Tastendruck pr�fen
				//pr�fen -> kurz warten -> wenn immer noch gedr�ckt -> = lange gedr�ckt
				if ((debounce(PIND,PD0)))
				{
					_delay_ms(100);
					if ((debounce(PIND,PD0))) 
					{
						buttonLinks=2;
					}
				}
				//wenn button links lange gedr�ckt wurde -> wechsle in den anderen heating-mode	
				if (buttonLinks==2)
				{
					currentMode=heatControlManualMode;
					screenHeatingManual(2, &heizLevel);
				}
				//button Mitte -> temperatur Sollwert f�r Regler verringern
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
				//button rechts -> temperatur sollwert f�r regler erh�hen
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
			//---------------------------------------------------------------- 
			case voltageControlMode:
				//start adc single conversion for both batteries
				//both values are voltage*100
				batt1Volt = adcRead15(4);
				batt2Volt = adcRead15(5);
				screenVoltageControl(batt1Volt, batt2Volt);
				break;
			//---------------------------------------------------------------- 
			case tempInfoMode:
				//start adc single conversion for both temperature sensors voltages
				//both values are voltage*100
				temp1Volt = adcRead5(6);
				temp2Volt = adcRead5(7);
				screenTempInfo(temp1Volt,temp2Volt);
				break;
			//---------------------------------------------------------------- 
			case versionInfoMode:
				screenInfo();
				break;
			//---------------------------------------------------------------- 	
			//default:
			//	break;		
		}
		//Folgende routinen versetzen den controller in den sleep-mode wenn die z�ndungsspannung aus ist.
		// Z�ndung=ON  -> Transistor schaltet 0V -> PIND.6 = 0
		// Z�ndung=OFF -> Transistor sperrt      -> PIND.6 = 1 (interner Pull-Up)
		// ->Sleep ON
		/*if (PIND & (1<<BIT6))	
		{						
			screenGotoSleepmode();
			_delay_ms(2000);
			PORTC &= ~(1<<BIT0);//Display -> Spannungslos (Display-Masse wird abgeschaltet)
			PORTC |= (1<<BIT1); //DisplayBeleuchtung -> spannungslos (�ber Darlington 0V an TransistorBasis)
			sleep_enable();	   
			sleep_cpu();
		}*/				
		
		//Wenn uC gerade aufgewacht ist, dann willkommens und battVolt Screens darstellen.
		if (wakeUp==1)
		{
			menuInit();
			wakeUp=0;
		}


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
			
			else //buttonKalt=gedr�ckt
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
		
		if (PIND & (1<<BIT6))	// Z�ndung=ON  -> Transistor schaltet 0V -> PIND.6 = 0
		{						// Z�ndung=OFF -> Transistor sperrt      -> PIND.6 = 1 (interner Pull-Up)
			 					// '->Sleep ON
			PORTC &= ~(1<<BIT0);//Display -> Spannungslos (Display-Masse wird abgeschaltet)
			PORTC |= (1<<BIT1); //DisplayBeleuchtung -> spannungslos (�ber Darlington 0V an TransistorBasis)
			sleep_enable();	   
			sleep_cpu();
		}					   
   
   }	
   return 0;                 
}*/
