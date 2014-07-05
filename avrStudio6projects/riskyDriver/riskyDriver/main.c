
#define F_CPU   8000000UL

#include "portmacro.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <stdlib.h>
#include <string.h>

#define GCC_MEGA_AVR

#define F_CPU 8000000UL

#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h> 
#endif

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "communicationTask.h"
#include "navigatorTask.h"
#include "operatorTask.h"
#include "sensorTask.h"
#include "globals.h"


/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainCOMMUNICATION_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
#define mainNAVIGATOR_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )
#define mainOPERATOR_TASK_PRIORITY ( tskIDLE_PRIORITY + 3 )
#define mainSENSOR_TASK_PRIORITY ( tskIDLE_PRIORITY + 3 )

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS			( ( void * ) 0x50 )

/*
 * The task function for the "Check" task.
 */
//static void vErrorChecks( void *pvParameters );

/*
 * Called on boot to increment a count stored in the EEPROM.  This is used to 
 * ensure the CPU does not reset unexpectedly.
 */
static void prvIncrementResetCount( void );

/*
 * The idle hook is used to scheduler co-routines.
 */
void vApplicationIdleHook( void );

/*-----------------------------------------------------------*/

int main( void )
{  
    //set the length of the queues -> how many items the queue can hold
    const unsigned portBASE_TYPE measurementQueueLength = 5;
    //const unsigned portBASE_TYPE statusQueueLength = 5;
    
    xQueueHandle measurementQueue;
    //xQueueHandle statusQueue;
   
	// ...for temperature and pressure sensor
	double temperature = 0;
	int32_t pressure = 0;
    double altitude = 0;

   
    
    //configure time measurment pin as output
    //DDRD |= (1<<PB7);
    

    bmp085_init();
        
    while( 1 )
    {
        temperature = bmp085_gettemperature();
        //pressure = bmp085_getpressure();
        //altitude = bmp085_getaltitude();
        //altitude = bmp085CalcAltitude ( pressure );
        //_delay_ms(100);
    }

	//prvIncrementResetCount();
    
    //create queues for intertask communication
    measurementQueue = xQueueCreate( measurementQueueLength, ( unsigned portBASE_TYPE ) sizeof( measurementQueueMsg * ) );
    //statusQueue = xQueueCreate( statusQueueLength, ( unsigned portBASE_TYPE ) sizeof( statusQueueMsg * ) );
        
	/* Create the tasks. */
    startCommunicationTask( mainCOMMUNICATION_TASK_PRIORITY, &measurementQueue );
    startNavigatorTask( mainNAVIGATOR_TASK_PRIORITY, &measurementQueue );
    startOperatorTask( mainOPERATOR_TASK_PRIORITY, &measurementQueue );
	startSensorTask( mainSENSOR_TASK_PRIORITY, &measurementQueue);
	
	/* In this port, to use preemptive scheduler define configUSE_PREEMPTION 
	as 1 in portmacro.h.  To use the cooperative scheduler define 
	configUSE_PREEMPTION as 0. */
	vTaskStartScheduler();

	return 0;
}
/*-----------------------------------------------------------*/

static void prvIncrementResetCount( void )
{
unsigned char ucCount;

	eeprom_read_block( &ucCount, mainRESET_COUNT_ADDRESS, sizeof( ucCount ) );
	ucCount++;
	eeprom_write_byte( mainRESET_COUNT_ADDRESS, ucCount );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	//vCoRoutineSchedule();
}

