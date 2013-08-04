#include "operatorTask.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"


/* The size of the task stack specified as the number o
f variables the stack can hold. */
#define sysOperatorTaskSTACK_SIZE			configMINIMAL_STACK_SIZE

/* The period between executions of the system status task. */
#define OPERATOR_TASK_CYCLE	( ( uint32_t ) 15 )

static portTASK_FUNCTION_PROTO( executeOperatorTask, pvParameters );

/* This will be set to pdFALSE if we detect an error.
   Used to signal an error condition inside this task
*/
static portBASE_TYPE xOperatorTaskStatus = pdPASS;

void startOperatorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *measQueue )
{
    operatorTaskParameters *taskParams;
    taskParams = ( operatorTaskParameters * ) pvPortMalloc( sizeof( operatorTaskParameters ) );
    taskParams->measurementQueue = *measQueue;
    
    xTaskCreate( executeOperatorTask, ( signed char * ) "opTask", sysOperatorTaskSTACK_SIZE, ( void * ) taskParams, uxPriority, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION( executeOperatorTask, pvParameters )
{
    uint32_t xLastWakeUpTime;
    
      
    uint8_t receivedNewMessage = 0;
    uint8_t measurementValueUpdated = 0;
    
    
    //variable to store one item of the measurement queue
    measurementQueueMsg *measurementMsg;
    
    operatorTaskParameters *taskParams;
    
    taskParams = ( operatorTaskParameters * ) pvParameters;
    
    //Initialize the wake up time with the current time
    xLastWakeUpTime = xTaskGetTickCount();
    
    for(;;)
    {
        
        //Execute this task each 15ms
        vTaskDelayUntil( &xLastWakeUpTime, OPERATOR_TASK_CYCLE );
                
        //check if there are new items to read in the measurement value queue
        //receivedNewMessage = xQueueReceive( taskParams->measurementQueue, &( measurementMsg ), ( portTickType ) 10 );
        
        //if a queue element has been successfully received -> evaluate message type and update display value      
        //if( receivedNewMessage )
        //{
            //if( measurementMsg->valueId == ID_BATTERY_VOLTAGE )
            //{
                //batteryVoltage = measurementMsg->value;
            //}
            //else if( measurementMsg->valueId == ID_SOLAR_VOLTAGE )
            //{
                //solarVoltage = measurementMsg->value;
            //}
            //else if( measurementMsg->valueId == ID_SOLAR_CURRENT )
            //{
                //solarCurrent = measurementMsg->value;
            //}
            //else if( measurementMsg->valueId == ID_CONSUMER_CURRENT )
            //{
                //consumerCurrent = measurementMsg->value;
            //}
            //measurementValueUpdated = 1;
        //}
    }
}

/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xIsOperatorTaskStillRunning( void )
{
    portBASE_TYPE xReturn;

    /* Evaluate task status and other relevant variables to indicate the task´s health */
    if( xOperatorTaskStatus == pdFAIL )
    {
        xReturn = pdFAIL;
    }
    else
    {
        xReturn = pdPASS;
    }

    return xReturn;
}