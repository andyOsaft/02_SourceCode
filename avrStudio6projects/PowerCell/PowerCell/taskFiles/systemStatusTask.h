#ifndef SYSTEMSTATUSTASK_H_
#define SYSTEMSTATUSTASK_H_

#include "systemControlTask.h"
#include "portmacro.h"

void startSystemStatusTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *statQueue );

typedef struct SYSTEM_STATUS_TASK_PARAMETERS
{
    xQueueHandle statusQueue;					/*< The queue to transfer status messages to the system status task. */
} systemStatusTaskParameters;

#endif /* SYSTEMSTATUSTASK_H_ */