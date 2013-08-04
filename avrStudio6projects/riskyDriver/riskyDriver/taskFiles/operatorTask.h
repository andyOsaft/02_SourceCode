#ifndef OPERATORTASK_H_
#define OPERATORTASK_H_

#include "communicationTask.h"
#include "portmacro.h"

void startOperatorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *measQueue );

portBASE_TYPE xIsOperatorTaskStillRunning( void );

/* Structure used to pass parameters to the blocking queue tasks. */
typedef struct OPERATOR_TASK_PARAMETERS
{
    xQueueHandle measurementQueue;				/*< The queue to transfer measurement values to the interface task. */
} operatorTaskParameters;

#endif /* OPERATORTASK_H_ */