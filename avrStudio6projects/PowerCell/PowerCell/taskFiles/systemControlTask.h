#ifndef SYSTEMCONTROLTASK_H_
#define SYSTEMCONTROLTASK_H_

#include "FreeRTOS.h"
#include "portmacro.h"
#include "queue.h"


void startSystemControlTask( unsigned portBASE_TYPE uxPriority, xQueueHandle* measQueue, xQueueHandle* statQueue );

portBASE_TYPE xIsSystemControlTaskStillRunning( void );


typedef enum MEASUREMENT_VALUE_ID
{
    ID_BATTERY_VOLTAGE = 0,
    ID_SOLAR_VOLTAGE = 1,
    ID_SOLAR_CURRENT = 2,
    ID_CONSUMER_CURRENT = 3
} measurementValueId;

typedef enum STATUS_MSG_ID
{
    ID_TEMPERATURE_ERROR = 0,
    ID_TEMPERATURE_OK = 1,
    ID_SOLAR_CURRENT_ERROR = 2,
    ID_SOLART_CURRENT_OK = 3,
    ID_CONSUMER_CURRENT_ERROR = 4,
    ID_CONSUMER_CURRENT_OK = 5
} statusMsgId;

/* Structure used to pass parameters to the blocking queue tasks. */
typedef struct SYSTEM_CONTROL_TASK_PARAMETERS
{
    xQueueHandle measurementQueue;				/*< The queue to transfer measurement values to the interface task. */
    xQueueHandle statusQueue;					/*< The queue to transfer status messages to the system status task. */
} systemControlTaskParameters;


typedef struct MEASUREMENT_QUEUE_MESSAGE
{
    measurementValueId valueId;
    uint16_t value;
} measurementQueueMsg;


typedef struct STATUS_QUEUE_MESSAGE
{
    statusMsgId msgId;
    uint16_t value;
} statusQueueMsg;






#endif /* SYSTEMCONTROLTASK_H_ */