#include "sensorTask.h"

#include "operatorTask.h"
#include "navigatorTask.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"


/* The size of the task stack specified as the number of variables the stack can hold. */
#define sysSensorTaskSTACK_SIZE			configMINIMAL_STACK_SIZE

/* The period between executions of the system status task. */
#define SENSOR_TASK_CYCLE	( ( uint32_t ) 100 )

#define GREEN_LED_TOGGLE_CYCLE		( 500 )

static portTASK_FUNCTION_PROTO( executeSensorTask, pvParameters );

static portBASE_TYPE prvCheckOtherTasksAreStillRunning( void );


void startSensorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *statQueue )
{
    //set pin PA7 (green LED) to be an output
    DDRA |= (1<<PA7);
    
    sensorTaskParameters *taskParams;
    taskParams = ( sensorTaskParameters * ) pvPortMalloc( sizeof( sensorTaskParameters ) );
    taskParams->statusQueue = *statQueue;

    xTaskCreate( executeSensorTask, ( signed char * ) "SensorTask", sysSensorTaskSTACK_SIZE, ( void * ) taskParams, uxPriority, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION( executeSensorTask, pvParameters )
{
    uint32_t xLastWakeUpTime;
    uint16_t ledCycleCounter = 0;
    
    //Initialize the wake up time with the current time
    xLastWakeUpTime = xTaskGetTickCount();

    for(;;)
    {
        //Execute this task each 50ms
        vTaskDelayUntil( &xLastWakeUpTime, SENSOR_TASK_CYCLE );

        ledCycleCounter += 50;

        if( prvCheckOtherTasksAreStillRunning() )
        {
            //signal that everything is ok
        }
        else
        {
            //signal error condition
        }

        //toogle the green led each 500ms
        if( ledCycleCounter % GREEN_LED_TOGGLE_CYCLE == 0 )
        {
            PORTA ^= (1<<PA7);
        }
        
        if( ledCycleCounter > 65000 )
        {
            ledCycleCounter = 0;
        }
       
    }
}

/*
 * Checks the unique counts of other tasks to ensure they are still operational.
 */
static portBASE_TYPE prvCheckOtherTasksAreStillRunning( void )
{
    static portBASE_TYPE xErrorHasOccurred = pdFALSE;

	if( xIsCommunicationTaskStillRunning() != pdTRUE )
	{
		xErrorHasOccurred = pdTRUE;
	}

	if( xIsOperatorTaskStillRunning() != pdTRUE )
	{
		xErrorHasOccurred = pdTRUE;
	}

	if( xIsNavigatorTaskStillRunning() != pdTRUE )
	{
    	xErrorHasOccurred = pdTRUE;
	}

	if( xErrorHasOccurred == pdFALSE )
	{
        //signal that everything is ok
	}

    return ( xErrorHasOccurred );
}