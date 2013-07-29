#include "timer.h"


/************************************************************************************************	

	Dieses c-File stellt die PWM-Funktionalität für den Attiny13 zur Verfügung
	
    Genutzt wird der 8Bit Timer0
	
	Der PWM-Ausgabe-Pin ist bei KanalA -> PB0 (OC0A - Pin 5)
							


	Mit Übergabe des Parameters "compare" wird der Duty-Cycle eingestellt.
	

	Um den Duty-Cycle aus dem laufenden Betrieb zu modifizieren muss,
	das folgende Register mit einem Wert zwischen 0 & 255 beschrieben werden.

	Kanal A -> OCR0A

		
	Ziel:	-> PWM zur Ansteuerung von Servos erzeugen, d.h. - Periode ca 20ms 
															 - High pulse zwischen 1ms und 2ms
															 - 1ms = 0% ; 2ms = 100%
    
    Als Pwm Modus wird Fast Pwm genutzt. Hierbei zählt der Timer wiederholt
    von 0 bis 255.
    
    - Beim Erreichen der 255 wird der AusgangsPin auf High Level gesetzt.
    - Beim Erreichen des Wertes von OCR0A wird der Pin auf Low gesetzt.
    
    
	Berechnung:
    
    Periode -> Ziel 20ms 

	Prozessortakt: 8MHz/Prescaler1024 = 7,813kHz -> T=127,9us

	Periode: Register ICR1 legt den TOP Value des Timers fest.
			 256 * 127,9us = 32,74ms  -> 30,5Hz
             
	
	156 Zählschritte mit 127,9us pro Schritt -> 1 PWM-Periode = 19,95ms

	Duty Cycle:

	1ms Pulse -> Compare Value = (1ms/127,9us) = 7,82 = 8
	2ms Pulse -> Compare Value = (2ms/127,9us) = 15,64 = 16

*********************************************************************************************** */


void pwmInit(void)
{
	DDRB |= (1<<DDB1);		
    
    //Startwert laden -> Servo in Stellung max links
    OCR0B=22;	//Vergleichswert laden 
    
    
    //Prescaler = 1024
    TCCR0B |= ((1<<CS00) | (1<<CS02));  
    
    
	/*Fast PWM Modus
        -> Clear OC0B on Compare Match
        -> Set OC0B at TOP (255)
	*/
    TCCR0A |= ((1<<WGM00) | (1<<WGM01));	
	//TCCR0A &= ~(1<<COM0A0);	
									
    //TCCR0B &= ~((1<<WGM02) | (1<<CS01));
    
    
    //Timer overflow interrupt enablen
    TIMSK |= (1<<TOIE0);

	
}

/* For servo use the compare value has to be inside 
   the boundary of 7 and 17 (Duty: 1ms and 2ms) */
void pwmSetDutyCycle(uint8_t compare)
{   
    OCR0B = compare;	
}



/* The following two functions aim to make the software more
   energy-efficient by disabling the pwm output when not needed */
void disablePwm(void)
{
    TCCR0A &= ~(1<<COM0B1);
}

void enablePwm(void)
{
    TCCR0A |= (1<<COM0B1);
}


/* Accesses the global variables 
    - leftSleepingTimeMs (contains the remaining sleeping time in ms 
    - sleepingActive (information if sleeping mode should be active

   The sleep mode is switched ON inside this function.
   With the IDLE sleeping mode the timer 0 continues to run.
   
   During each timer 0 overflow interrupt (always after 27ms)
   the controller wakes up and the global variable leftSleepingTime 
   is decreased inside the ISR.
   
   As long the leftSleepingTime is > 0 this functions continues to
   enter the sleeping mode.   

*/
void sleepDeeplyMs()
{   
    set_sleep_mode(SLEEP_MODE_IDLE);  
    
    /* disable interrupts */
    cli();
    
    sleepingActive = TRUE;
    
    while(leftSleepingTimeMs > 33)
    {   
        /* the variable leftSleepingTimeMs is now decremented each 33ms in the 
           timer0 overflow interrupt -> see interrupt.c for details */
        sleep_enable();
        /* enable interrupts */
        sei();
        sleep_cpu();
        sleep_disable();        
    }
    sleepingActive = FALSE;   
    sei();
}