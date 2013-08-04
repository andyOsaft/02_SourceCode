#ifndef SENSORTASK_H_
#define SENSORTASK_H_

#include "communicationTask.h"

#include "portmacro.h"

void startSensorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *statQueue );

typedef struct SENSOR_TASK_PARAMETERS
{
    xQueueHandle statusQueue;					/*< The queue to transfer status messages to the system status task. */
} sensorTaskParameters;

#endif /* SENSORTASK_H_ */