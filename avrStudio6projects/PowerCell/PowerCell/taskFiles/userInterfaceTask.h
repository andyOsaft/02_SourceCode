#ifndef USERINTERFACETASK_H_
#define USERINTERFACETASK_H_

#include "systemControlTask.h"
#include "portmacro.h"

void startUserInterfaceTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *measQueue );

portBASE_TYPE xIsUserInterfaceTaskStillRunning( void );

/* Structure used to pass parameters to the blocking queue tasks. */
typedef struct INTERFACE_TASK_PARAMETERS
{
    xQueueHandle measurementQueue;				/*< The queue to transfer measurement values to the interface task. */
} interfaceTaskParameters;

#endif /* USERINTERFACETASK_H_ */