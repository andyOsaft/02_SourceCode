#include "navigatorTask.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"


/* The size of the task stack specified as the number of variables the stack can hold. */
#define sysNavigatorTaskSTACK_SIZE			configMINIMAL_STACK_SIZE

/* The period between executions of the system status task. */
#define NAVIGATOR_TASK_CYCLE	( ( uint32_t ) 100 )


static portTASK_FUNCTION_PROTO( executeNavigatorTask, pvParameters );

/* This will be set to pdFALSE if we detect an error.
   Used to signal an error condition inside this task
*/
static portBASE_TYPE xNavigatorTaskStatus = pdPASS;

void startNavigatorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *statQueue )
{
    //set pins PB0 (green LED) & PB1 (red LED) to outputs
    DDRB |= ( (1<<PB0) | (1<<PB1) );
    
    navigatorTaskParameters *taskParams;
    taskParams = ( navigatorTaskParameters * ) pvPortMalloc( sizeof( navigatorTaskParameters ) );
    taskParams->statusQueue = *statQueue;

    xTaskCreate( executeNavigatorTask, ( signed char * ) "NavTask", sysNavigatorTaskSTACK_SIZE, ( void * ) taskParams, uxPriority, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION( executeNavigatorTask, pvParameters )
{
    uint32_t xLastWakeUpTime;

    //Initialize the wake up time with the current time
    xLastWakeUpTime = xTaskGetTickCount();

    for(;;)
    {
        //Execute this task each 50ms
        vTaskDelayUntil( &xLastWakeUpTime, NAVIGATOR_TASK_CYCLE );
      
    }
}

/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xIsNavigatorTaskStillRunning( void )
{
    portBASE_TYPE xReturn;

    /* Evaluate task status and other relevant variables to indicate the task´s health */
    if( xNavigatorTaskStatus == pdFAIL )
    {
        xReturn = pdFAIL;
    }
    else
    {
        xReturn = pdPASS;
    }

    return xReturn;
}