/*********************************************************************************************

Interrupt-Vektoren sind definiert in Header-File "iom32.h"


**********************************************************************************************/


#include "globals.h"


/* Interrupt Service Routine für Timer0 Overflow Interrupt 
    -> wird verwendet um den Controller aus dem Sleep Modus
       wieder aufzuwecken
   
   Wenn der SleepMode als aktiv gesetzt ist, dann werden bei
   jedem ISR Aufruf von der SleepTime 33ms abgezogen.
   (Der Overflow tritt jeweils nach 33ms auf)    
*/
ISR(TIM0_OVF_vect)
{
    if (sleepingActive==TRUE && leftSleepingTimeMs>33)
    {
        leftSleepingTimeMs -= 33;
    }
}

