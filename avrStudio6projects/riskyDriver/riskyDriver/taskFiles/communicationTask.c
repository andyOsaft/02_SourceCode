#include "communicationTask.h"
#include "adc.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* The size of the task stack specified as the number of variables the stack can hold. */
#define sysCommunicationTaskSTACK_SIZE			configMINIMAL_STACK_SIZE

/* The period between executions of the system status task. */
#define COMMUNICATION_TASK_CYCLE	( ( uint32_t ) 500 )

static portTASK_FUNCTION_PROTO( executeCommunicationTask, pvParameters );

/* This will be set to pdFALSE if we detect an error.
   Used to signal an error condition inside this task
*/
static portBASE_TYPE xCommunicationTaskStatus = pdPASS;


void startCommunicationTask( unsigned portBASE_TYPE uxPriority, xQueueHandle* measQueue )
{  
    adcInit();
        
    //Set high side driver pins to outputs
    DDRC |= ( (1<<PC6) | (1<<PC7) );    
    //switch on high side driver
    PORTC |=  ( (1<<PC6) | (1<<PC7) );       
        
    //create task parameter
    communicationTaskParameters *taskParams;
    taskParams = ( communicationTaskParameters * ) pvPortMalloc( sizeof( communicationTaskParameters ) );
    taskParams->measurementQueue = *measQueue;
    //taskParams->statusQueue = *statQueue;
    
    xTaskCreate( executeCommunicationTask, ( signed char * ) "commTask", sysCommunicationTaskSTACK_SIZE,  ( void * ) taskParams , uxPriority, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION( executeCommunicationTask, pvParameters )
{
    uint32_t xLastWakeUpTime;
    
    uint16_t batteryVoltage_mVolt;
    uint16_t consumerCurrent_mAmps;
    uint16_t solarChargeCurrent_mAmps;
    
    communicationTaskParameters *taskParams;
    
    measurementQueueMsg *measurementMsg;
    //statusQueueMsg *statusMsg;
    
    uint8_t successFlag=0;
    
    taskParams = ( communicationTaskParameters * ) pvParameters;
        
    //Initialize the wake up time with the current time
    xLastWakeUpTime = xTaskGetTickCount();    
    
    for(;;)
    {
        vTaskDelayUntil( &xLastWakeUpTime, COMMUNICATION_TASK_CYCLE );
                
        //Execute this task each 50ms
        batteryVoltage_mVolt = measureBattVoltage();
        //solarChargeCurrent_mAmps = measureSolarChargeCurrent();
        //consumerCurrent_mAmps = measureConsumerCurrent();
        //!!measure also solar voltage!!!
        
        //send new battery voltage value via measurement queue
        //first: prepare message
        measurementMsg->valueId = ID_BATTERY_VOLTAGE;
        measurementMsg->value = batteryVoltage_mVolt;
           
        //second: send queue
        successFlag = xQueueSend( taskParams->measurementQueue, &measurementMsg, ( portTickType ) 10  ) ;
    }
}

/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xIsCommunicationTaskStillRunning( void )
{
    portBASE_TYPE xReturn;

	/* Evaluate task status and other relevant variables to indicate the task´s health */
	if( xCommunicationTaskStatus == pdFAIL )
	{
		xReturn = pdFAIL;
	}
	else
	{
		xReturn = pdPASS;
	}

	return xReturn;
}