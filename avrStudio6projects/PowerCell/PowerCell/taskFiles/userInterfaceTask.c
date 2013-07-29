#include "userInterfaceTask.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#include "font.h"
#include "dogm-graphic.h"

//defines for the different overview menu items
#define TOPVIEW_STARTUP_SCREEN             0
#define TOPVIEW_CURRENT_MONITOR            10
#define DETAILVIEW_CURRENT_MONITOR         11
#define TOPVIEW_VOLTAGE_MONITOR            20
#define DETAILVIEW_VOLTAGE_MONITOR         21
#define TOPVIEW_TEMPERATURE_MONITOR        30
#define DETAILVIEW_TEMPERATURE_MONITOR     31
#define TOPVIEW_SYSTEM_STATUS              40
#define DETAILVIEW_SYSTEM_STATUS           41
#define TOPVIEW_LOGGING                    50
#define DETAILVIEW_LOGGING                 51 
#define TOPVIEW_SWITCH_OFF                 60
#define VIEW_UNDEFINED                     255

//if current view is a detail view -> the remainder is true (not 0)
#define IS_TOP_VIEW( currentView )      ( ( ~(currentView % 10)) & 0x01 ) 


#define BUTTON_PRESSED  0
#define BUTTON_RELEASED 1

#define READ_VIEWBUTTON ( (PIND & (1<<PIND2)) >> 2 )
#define READ_ACTIONBUTTON ( (PIND & (1<<PIND3)) >> 3 )


/* The size of the task stack specified as the number o
f variables the stack can hold. */
#define uiTaskSTACK_SIZE			configMINIMAL_STACK_SIZE

/* The period between executions of the system status task. */
#define USER_INTERFACE_TASK_CYCLE	( ( uint32_t ) 15 )

static portTASK_FUNCTION_PROTO( executeUserInterfaceTask, pvParameters );

/* This will be set to pdFALSE if we detect an error.
   Used to signal an error condition inside this task
*/
static portBASE_TYPE xUiTaskStatus = pdPASS;

void startUserInterfaceTask( unsigned portBASE_TYPE uxPriority, xQueueHandle *measQueue )
{
    interfaceTaskParameters *taskParams;
    taskParams = ( interfaceTaskParameters * ) pvPortMalloc( sizeof( interfaceTaskParameters ) );
    taskParams->measurementQueue = *measQueue;
    
    xTaskCreate( executeUserInterfaceTask, ( signed char * ) "UiTask", uiTaskSTACK_SIZE, ( void * ) taskParams, uxPriority, ( xTaskHandle * ) NULL );
}


static portTASK_FUNCTION( executeUserInterfaceTask, pvParameters )
{
    uint32_t xLastWakeUpTime;
    uint8_t currentView = TOPVIEW_CURRENT_MONITOR;
    uint8_t lastView = VIEW_UNDEFINED;
    
    uint8_t viewButton;
    uint8_t viewButtonDebouncer = 0;
    uint8_t viewButtonPressEvent = 0;
    uint8_t viewButtonHoldEvent = 0;
    
    uint8_t actionButton;
    uint8_t actionButtonDebouncer = 0;
    uint8_t actionButtonPressEvent = 0;
    uint8_t actionButtonHoldEvent = 0;
    
    uint8_t receivedNewMessage = 0;
    uint8_t measurementValueUpdated = 0;
    
    //the measurement values to be displayed
    uint16_t batteryVoltage = 0;
    uint16_t solarVoltage = 0;
    uint16_t solarCurrent = 0;
    uint16_t consumerCurrent = 0;
    
    //variable to store one item of the measurement queue
    measurementQueueMsg *measurementMsg;
    
    systemControlTaskParameters *taskParams;
    
    //set pins PD2 (action button) & PD3 (view switch button) to inputs 
    //(external pull-ups connected)
    DDRD &= ~( (1<<PD2) | (1<<PD3) );
    
    taskParams = ( interfaceTaskParameters * ) pvParameters;
    
    //Initialize the wake up time with the current time
    xLastWakeUpTime = xTaskGetTickCount();
    
    for(;;)
    {
        
        //Execute this task each 15ms
        vTaskDelayUntil( &xLastWakeUpTime, USER_INTERFACE_TASK_CYCLE );
        
        PORTD ^= (1<<PIND7);
        
        //check if there are new items to read in the measurement value queue
        receivedNewMessage = xQueueReceive( taskParams->measurementQueue, &( measurementMsg ), ( portTickType ) 10 );
        
        //if a queue element has been successfully received -> evaluate message type and update display value      
        if( receivedNewMessage )
        {
            if( measurementMsg->valueId == ID_BATTERY_VOLTAGE )
            {
                batteryVoltage = measurementMsg->value;
            }
            else if( measurementMsg->valueId == ID_SOLAR_VOLTAGE )
            {
                solarVoltage = measurementMsg->value;
            }
            else if( measurementMsg->valueId == ID_SOLAR_CURRENT )
            {
                solarCurrent = measurementMsg->value;
            }
            else if( measurementMsg->valueId == ID_CONSUMER_CURRENT )
            {
                consumerCurrent = measurementMsg->value;
            }
            measurementValueUpdated = 1;
        }
       
        //read state of input buttons
        viewButton = READ_VIEWBUTTON;
        actionButton = READ_ACTIONBUTTON;
        
        //************************************************************************************************
        //-> debounce handler for view button
        //debounce buttons for ~45ms (3 task calls)
        if( viewButton == BUTTON_PRESSED && viewButtonDebouncer < 3 )
        {
            //button pressed -> debounce in progress
            viewButtonDebouncer++;
            viewButtonPressEvent = 0;
        }
        else if( viewButton == BUTTON_PRESSED && viewButtonDebouncer == 3 )
        {
            //button still pressed and debouncing finished
            viewButtonDebouncer++;
            viewButtonPressEvent = 1;                            
        }                    
        else if( viewButton == BUTTON_PRESSED && viewButtonDebouncer >=3 && viewButtonDebouncer < 134 )
        {
            //single press event finished -> check for button hold
            viewButtonDebouncer++;
            viewButtonPressEvent = 0;
        }
        else if( viewButton == BUTTON_PRESSED && viewButtonDebouncer >= 134 )
        {
            viewButtonHoldEvent = 1;
        }        
        else if( viewButton == BUTTON_RELEASED )
        {
            //button has been released or bounced
            viewButtonDebouncer = 0;
            viewButtonPressEvent = 0;
            viewButtonHoldEvent = 0;
        }
        

        //************************************************************************************************
        //-> debounce handler for action button   
        //debounce buttons for ~45ms (3 task calls)
        if( actionButton == BUTTON_PRESSED && actionButtonDebouncer < 3 )
        {
            //button pressed -> debounce in progress
            actionButtonDebouncer++;
            actionButtonPressEvent = 0;
        }
        else if( actionButton == BUTTON_PRESSED && actionButtonDebouncer == 3 )
        {
            //button still pressed and debouncing finished
            actionButtonDebouncer++;
            actionButtonPressEvent = 1;
        }
        else if( actionButton == BUTTON_PRESSED && actionButtonDebouncer >=3 && actionButtonDebouncer < 134 )
        {
            //single press event finished -> check for button hold
            actionButtonDebouncer++;
            actionButtonPressEvent = 0;
        }
        else if( actionButton == BUTTON_PRESSED && actionButtonDebouncer >= 134 )
        {
            actionButtonHoldEvent = 1;
        }
        else if( actionButton == BUTTON_RELEASED )
        {
            //button has been released or bounced
            actionButtonDebouncer = 0;
            actionButtonPressEvent = 0;
            actionButtonHoldEvent = 0;
        }
        
        
        //************************************************************************************************
        //-> display screen handler       
                                        
        //evaluate button events and update display state
        if( viewButtonPressEvent  )
        {
            if( IS_TOP_VIEW( currentView ) )
            {
                // special handling for switchOff top view
                //-> send controller into sleep mode
                if( currentView == TOPVIEW_SWITCH_OFF )
                {
                    //TODO: enable sleep mode
                }
                else
                {
                    currentView++;
                }                
            }
            else
            {
                currentView--;
            }
            lcd_clear_all();
        }
        
        if( actionButtonPressEvent )
        {
            if( IS_TOP_VIEW( currentView ) )
            {
                if( currentView == TOPVIEW_SWITCH_OFF )
                {
                    currentView = TOPVIEW_CURRENT_MONITOR;
                }
                else
                {
                    currentView += 10;
                }                
            }
        }        
                        
        //draw this only if the current view is a top view and the last view was a detail view
        if( ( currentView % 10 == 0 && lastView % 10 != 0) || ( lastView == TOPVIEW_SWITCH_OFF && currentView != TOPVIEW_SWITCH_OFF ) )
        {
            lcd_set_font( FONT_FIXED_8, NORMAL );
            lcd_moveto_xy  ( 0, 80 );
            lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("* next-> "));
            lcd_moveto_xy  ( 1, 80 );
            lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("*        "));
            lcd_moveto_xy  ( 2, 80 );
            lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("*        "));
            lcd_moveto_xy  ( 3, 80 );
            lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("* detail "));
        }
       
       
        //manage display output for current view
        switch( currentView )
        {
            case TOPVIEW_STARTUP_SCREEN:
            {
                break;
            }
            case TOPVIEW_CURRENT_MONITOR:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("CurrentMon"));
                }     
                break;                
            }     
            case DETAILVIEW_CURRENT_MONITOR:
            {
                if( currentView != lastView || measurementValueUpdated )
                {
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 0, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Current Monitor:"));
                    lcd_moveto_xy  ( 2, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Solar: "));
                    lcd_put_uint( solarCurrent );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR(" mA"));
                    lcd_moveto_xy  ( 3, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Consumer: "));
                    lcd_put_uint( consumerCurrent );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR(" mA"));
                }
                break;              
            }            
            case TOPVIEW_VOLTAGE_MONITOR:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("VoltageMon"));
                } 
                break;                               
            }         
            case DETAILVIEW_VOLTAGE_MONITOR:
            {
                if( currentView != lastView || measurementValueUpdated )
                {
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 0, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Voltage Monitor:"));
                    lcd_moveto_xy  ( 2, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Battery: "));
                    lcd_put_uint( batteryVoltage );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR(" mV"));
                    lcd_moveto_xy  ( 3, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Solar: "));
                    lcd_put_uint( solarVoltage );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR(" mV"));
                }                
                break;                
            }
            case TOPVIEW_TEMPERATURE_MONITOR:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("TempMon   "));
                }   
                break;                             
            }
            case DETAILVIEW_TEMPERATURE_MONITOR:
            {
                if( currentView != lastView || measurementValueUpdated )
                {
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 0, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Temperature Monitor:"));
                }                
                break;                
            }                 
            case TOPVIEW_SYSTEM_STATUS:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("SysStatus "));
                }               
                break;                 
            }           
            case DETAILVIEW_SYSTEM_STATUS:
            {
                if( currentView != lastView || measurementValueUpdated )
                {
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 0, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("System Status:"));
                }                
                break;                
            }
            case TOPVIEW_LOGGING:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("Logging    "));
                }      
                break;                              
            }                 
            case DETAILVIEW_LOGGING:
            {
                if( currentView != lastView || measurementValueUpdated)
                {
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 0, 0 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("Logging:"));
                } 
                break;                
            }            
            case TOPVIEW_SWITCH_OFF:
            {
                if( currentView != lastView )
                {
                    lcd_set_font( FONT_PROP_16, NORMAL );
                    lcd_moveto_xy  ( 1, 0 );
                    lcd_put_string_P( FONT_PROP_16, NORMAL, PSTR("SwitchOff "));
                    lcd_set_font( FONT_FIXED_8, NORMAL );
                    lcd_moveto_xy  ( 3, 80 );
                    lcd_put_string_P( FONT_FIXED_8, NORMAL, PSTR("* do it "));
                }                
                break;
            }        
            default:
            {
                
            }      
        }
        
        lastView = currentView;
        measurementValueUpdated = 0;
    }
}

/* This is called to check that all the created tasks are still running. */
portBASE_TYPE xIsUserInterfaceTaskStillRunning( void )
{
    portBASE_TYPE xReturn;

    /* Evaluate task status and other relevant variables to indicate the task´s health */
    if( xUiTaskStatus == pdFAIL )
    {
        xReturn = pdFAIL;
    }
    else
    {
        xReturn = pdPASS;
    }

    return xReturn;
}