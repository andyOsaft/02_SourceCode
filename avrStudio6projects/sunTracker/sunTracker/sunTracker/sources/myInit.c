
#include "myInit.h"

/*--------------------------------------------------------------------------------------

Funktion:
			- führt Initialisierungen durch
			- wird bei Systemstart ausgeführt

--------------------------------------------------------------------------------------*/
void myInit()
{
    
    leftSleepingTimeMs = 0;
    sleepingActive = FALSE;
    
    /* configure controller pins as outputs and switch off (low level) */
    DDRB |= ((1<<SERVO_SWITCH) | (1<<POWERTRANSFER_SWITCH));
    
	
}







