#ifndef NAVIGATORTASK_H_
#define NAVIGATORTASK_H_

#include "communicationTask.h"
#include "portmacro.h"

void startNavigatorTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *statQueue );

typedef struct NAVIGATOR_TASK_PARAMETERS
{
    xQueueHandle statusQueue;					/*< The queue to transfer status messages to the system status task. */
} navigatorTaskParameters;

#endif /* NAVIGATORTASK_H_ */