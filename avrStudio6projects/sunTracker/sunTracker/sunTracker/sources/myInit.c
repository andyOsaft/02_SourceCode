
#include "myInit.h"

/*--------------------------------------------------------------------------------------

Funktion:
			- f�hrt Initialisierungen durch
			- wird bei Systemstart ausgef�hrt

--------------------------------------------------------------------------------------*/
void myInit()
{
    
    leftSleepingTimeMs = 0;
    sleepingActive = FALSE;
    
    /* configure controller pins as outputs and switch off (low level) */
    DDRB |= ((1<<SERVO_SWITCH) | (1<<POWERTRANSFER_SWITCH));
    
	
}







