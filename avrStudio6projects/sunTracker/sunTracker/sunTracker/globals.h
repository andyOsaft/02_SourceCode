#ifndef	GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

//avr includes
#include <avr/io.h>
//#include <stdlib.h>
//#include <util/delay.h>
#include <avr/interrupt.h> 
#include <avr/sleep.h>
//#include <stdio.h>
//#include <math.h>


//my includes
//#include "font.h"
//#include "dogm-graphic.h"
#include "adc.h"
#include "timer.h"
#include "interrupt.h"
#include "myInit.h"


/* Global variables */
extern volatile uint32_t leftSleepingTimeMs;
extern volatile uint8_t  sleepingActive;



#define	TRUE	1
#define	FALSE	0


#endif

