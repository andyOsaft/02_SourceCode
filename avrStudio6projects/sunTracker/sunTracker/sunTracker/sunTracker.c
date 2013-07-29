/*
 * sunTracker.c
 *
 * Created: 03.11.2012 13:01:53
 *  Author: xxxburns
 *
 * 
 *
 */ 


#include "globals.h"


#define STATE_START                    0
#define STATE_DETECT_BEST_SERVO_POS    1
#define STATE_LOGIC_START              2
#define STATE_SUPPLY_ON                3
#define STATE_SUPPLY_OFF               4


#define SERVO_POS_MAX_LEFT             2
#define SERVO_POS_MAX_RIGHT           22 


#define SERVO_SWITCH_OFF            (PORTB &= ~(1<<SERVO_SWITCH))
#define SERVO_SWITCH_ON             (PORTB |= (1<<SERVO_SWITCH))

#define POWERTRANSFER_SWITCH_OFF    (PORTB &= ~(1<<POWERTRANSFER_SWITCH))
#define POWERTRANSFER_SWITCH_ON     (PORTB |= (1<<POWERTRANSFER_SWITCH))

#define SWITCHES_OFF                (PORTB &= ~((1<<SERVO_SWITCH) | (1<<POWERTRANSFER_SWITCH))) 
#define SWITCHES_ON                 (PORTB |= ((1<<SERVO_SWITCH) | (1<<POWERTRANSFER_SWITCH)))


#define MIN_SOLAR_VOLTAGE           2500

#define TIME_5_MIN                  300000
#define TIME_10_SEC                 10000  
#define TIME_3_SEC                  3000  
#define TIME_1_SEC                  1000                   



/******************************************************************************
Global variables:



******************************************************************************/


/* Function Prototype */
void gotoServoPos(uint8_t servoCurrentPosition, uint8_t servoTargetPosition);



/******************************************************************************
Main loop
******************************************************************************/
void main(void)
{
    volatile uint8_t mainState = STATE_START;
    
    /* holds the current voltage output of the solar panel */
    volatile uint16_t uSolarNow;
    
    /* holds the current voltage of the battery of the device connected to the
       sun tracker */
    volatile uint16_t uBatteryNow;
       
    /* holds the current servo position 
       possible values: 
       -> 9  for max left
       -> 19 for max right */
    volatile uint8_t servoPosNow;
    volatile uint8_t servoPosBefore;
    
    
    /* holds the current brightness sensor voltage  */
    volatile uint16_t uSensorNow;
    
    /* holds the lowest sensor voltage */
    volatile uint16_t uSensorLowest;
    
    /* holds the servo position regarding to the lowest sensor voltage */ 
    volatile uint8_t servoPosSensorLow;
    
    /* holds the variable for the elapsed time [seconds]
       -> used to measure the time in state supply on */
    volatile uint32_t elapsedTimeS=0;
    
    /* controller initialization */
    myInit();
    adcInit();
    pwmInit();
    
    sei();
    
    //while(1)
    //{
        //uBatteryNow = adcRead(BATTERY_VOLTAGE);
        //uSensorNow = adcRead(SENSOR_VOLTAGE);
        //uSolarNow = adcRead(SOLAR_VOLTAGE);
        //
        //SERVO_SWITCH_ON;
        //asm ("nop");
        ////SERVO_SWITCH_OFF;
        //asm ("nop");
        ////POWERTRANSFER_SWITCH_ON;
        //asm ("nop");
        ////POWERTRANSFER_SWITCH_OFF;
        //asm ("nop");
        ////SWITCHES_ON;
        //asm ("nop");
        //SWITCHES_OFF;
        //
        //pwmSetDutyCycle(7);
        //pwmSetDutyCycle(17);
    //}

    //while(1)
    //{
        //leftSleepingTimeMs = 1000;
        //SERVO_SWITCH_ON;
        //sleepDeeplyMs();
        //SERVO_SWITCH_OFF;
        //leftSleepingTimeMs = 1000;
        //sleepDeeplyMs();
    //}
    //gotoServoPos(17, 2);
    //gotoServoPos(2, 22);
    //while(1)
    //{
         //
    //}
    
    
    /* execute state machine */
    while(1)
    {
        /* check if a waiting time needs to be applied before the state machine is executed again */
        sleepDeeplyMs();
        
        
        
        /*****************************************************************************************/
        if (mainState == STATE_START)
        {
            /* state actions */
            SWITCHES_OFF;
            uSolarNow = 0;
            uSensorLowest = 65535;
            servoPosSensorLow = 0;
            servoPosNow = SERVO_POS_MAX_LEFT;
            /* assume that the servo is currently in the max right position
                -> worst case assumption */
            gotoServoPos(SERVO_POS_MAX_RIGHT, servoPosNow);
            leftSleepingTimeMs = 0;
                            
                            
            /* state transitions */    
            mainState = STATE_DETECT_BEST_SERVO_POS;         
        }
        /*****************************************************************************************/
        else if (mainState == STATE_DETECT_BEST_SERVO_POS)
        {
            /* state actions */

            /* the higher the brightness - the lower the sensor output voltage */
            uSensorNow = adcRead(SENSOR_VOLTAGE);
                    
            /* store lowest sensor voltage and linked servo position*/
            if (uSensorNow < uSensorLowest)
            {
                uSensorLowest = uSensorNow;
                servoPosSensorLow = servoPosNow;
            }
                
            
            servoPosBefore = servoPosNow;
            servoPosNow += 1;
                                                        
            gotoServoPos(servoPosBefore, servoPosNow);                                        
            
            leftSleepingTimeMs = 500;
                
           
            /* state transitions */ 
            if (servoPosNow > SERVO_POS_MAX_RIGHT)
            {
                /* all possible servo position have been checked for max brightness 
                    -> move servo to brightest position */
                gotoServoPos(servoPosBefore, servoPosSensorLow);
                servoPosNow = servoPosSensorLow;
                    
                /* measure solar output voltage at brightest servo position 
                    -> if resulting voltage is below threshold -> wait 5 min and restart state machine */
                uSolarNow = adcRead(SOLAR_VOLTAGE);
                    
                if (uSolarNow < MIN_SOLAR_VOLTAGE)
                {   
                    mainState = STATE_SUPPLY_OFF;                    
                }
                else
                {
                    leftSleepingTimeMs = 0;
                    mainState = STATE_LOGIC_START;    
                }                   
            }                 
        }
        /*****************************************************************************************/
        else if (mainState == STATE_LOGIC_START)
        {
            /* state actions */
            /* measure the solar voltage and the battery voltage */
            uSolarNow = adcRead(SOLAR_VOLTAGE);
            uBatteryNow = adcRead(BATTERY_VOLTAGE);
            leftSleepingTimeMs = 0;                
                
            /* state transitions */
            if (uSolarNow > uBatteryNow)
            {
                /* connect the solar voltage to the device */
                mainState = STATE_SUPPLY_ON;
            }                               
            else
            {
                /* leave the solar voltage and the device disconnected 
                    -> but monitor the situation for 5min */
                mainState = STATE_SUPPLY_OFF;                    
            }  
        }                  
        /*****************************************************************************************/
        else if (mainState == STATE_SUPPLY_ON)
        {
            /* state actions */
            elapsedTimeS = 0;
            /* measure the solar voltage and the battery voltage */
            uSolarNow = adcRead(SOLAR_VOLTAGE);
            uBatteryNow = adcRead(BATTERY_VOLTAGE);
            POWERTRANSFER_SWITCH_ON;
            leftSleepingTimeMs = 0;
                
                  
            /* state transitions */
            /* if the solar voltage falls below the battery voltage 
                -> goto state supply off*/
            if (uSolarNow < uBatteryNow)
            {
                mainState = STATE_SUPPLY_OFF;                   
            }
            else
            {               
                /* the solar voltage is still greater than the battery voltage 
                    -> sleep for one second and then check again */ 
                leftSleepingTimeMs = TIME_1_SEC;                  
            }
        }              
        /*****************************************************************************************/
        else if (mainState == STATE_SUPPLY_OFF) 
        {
            /* state actions */
            POWERTRANSFER_SWITCH_OFF;
            /* measure the solar voltage and the battery voltage */
            uSolarNow = adcRead(SOLAR_VOLTAGE);
            uBatteryNow = adcRead(BATTERY_VOLTAGE);
   
            leftSleepingTimeMs = 0;
                     
            /* state transitions */
            /* if the solar voltage is below the battery voltage
                -> analyze situation again each 10s 
                   
                if the situation last for more than 5 min 
                    -> find again the best servo position 
                        
                if the sun comes back and the solar voltage rises 
                to a value greater than the battery voltage
                    -> connect the solar voltage to the battery*/
            if (uSolarNow > uBatteryNow)
            {
                mainState = STATE_SUPPLY_ON;
            }
            else
            {
                if (elapsedTimeS > TIME_5_MIN)
                {
                    elapsedTimeS = 0;
                    mainState = STATE_START;
                }
                else
                {
                    leftSleepingTimeMs = TIME_10_SEC;
                    /* if the time measurement is active -> count time up  */
                    elapsedTimeS += TIME_10_SEC;
                }                
            }
        }              	
    } 
}


#define SUBSTATE_START              0
#define SUBSTATE_CHARGING           1
#define SUBSTATE_VOLTAGE_READY      2
#define SUBSTATE_DRIVE_SERVO        3
#define SUBSTATE_TARGETPOS_REACHED  4

#define VOLT_5V                     5000

#define SERVO_DRIVE_CLOCKWISE       0
#define SERVO_DRIVE_ANTI_CLOCKWISE  1



void gotoServoPos(uint8_t servoCurrentPos, uint8_t servoTargetPos)
{
    /* holds the current state of the goto servo position state machine */
    volatile uint8_t subState = SUBSTATE_START;
 
    volatile uint16_t uSolarNow;
    volatile uint16_t uSolarBase;
    
    volatile uint8_t stateMachineExecuting = 1;
    
    volatile uint8_t servoDriveDirection;
    
    volatile uint8_t servoCurrentPosition = servoCurrentPos;
    
    
    /* determine servo drive direction */
    if (servoTargetPos >= servoCurrentPosition)
    {
        servoDriveDirection = SERVO_DRIVE_CLOCKWISE;
    }
    else
    {
        servoDriveDirection = SERVO_DRIVE_ANTI_CLOCKWISE;
    }
    
    enablePwm();
    
    while (stateMachineExecuting == 1)
    {
        /* check if a waiting time needs to be applied before the state machine is executed again */
        sleepDeeplyMs();
        SERVO_SWITCH_OFF;
        
        
        /*****************************************************************************************/
        if (subState == SUBSTATE_START)
        {
            /* state actions */
            SERVO_SWITCH_OFF;
            uSolarNow = adcRead(SOLAR_VOLTAGE);
            uSolarBase = uSolarNow;
            leftSleepingTimeMs = 0;
            
            /* state transitions */
            /* if the solar voltage is greater than 5V than it is high enough to drive the
                servo immediately 
                -> otherwise it needs to be checked if the capacitors are still charging up */
            if (servoCurrentPosition == servoTargetPos)
            {
                stateMachineExecuting = 0;
            }
            else if (uSolarNow > VOLT_5V)
            {
                subState = SUBSTATE_VOLTAGE_READY;
            }
            else
            {
                leftSleepingTimeMs = 100;
                subState = SUBSTATE_CHARGING;
            }
        }
        /*****************************************************************************************/
        else if (subState == SUBSTATE_CHARGING)
        {
            /* state actions */
            uSolarNow = adcRead(SOLAR_VOLTAGE);
            leftSleepingTimeMs = 0;
            
            /* state transitions */
            /* if the charging process is still ongoing -> wait and execute same state again */
            if (uSolarNow > uSolarBase)
            {
                leftSleepingTimeMs = 200;
                uSolarBase = uSolarNow;
                subState = SUBSTATE_CHARGING;
            }
            else
            {
                /* the charging process is stagnating
                    -> try to drive the servo, even if the solar voltage is too low */
                subState = SUBSTATE_VOLTAGE_READY;
            }
        }
        /*****************************************************************************************/        
        else if (subState == SUBSTATE_VOLTAGE_READY)
        {
            /* state actions */
            leftSleepingTimeMs = 0;
            
            /* state transitions */
            /* analyze the current servo position and determine if the target position
                has already been reached */
            if ((servoDriveDirection == SERVO_DRIVE_CLOCKWISE && servoCurrentPosition >= servoTargetPos) ||
                (servoDriveDirection == SERVO_DRIVE_ANTI_CLOCKWISE && servoCurrentPosition <= servoTargetPos))
            {
                subState = SUBSTATE_TARGETPOS_REACHED;
            }                    
            else
            {
                subState = SUBSTATE_DRIVE_SERVO;                    
            }                    
        }
        /*****************************************************************************************/
        else if (subState == SUBSTATE_DRIVE_SERVO)
        {
            /* state actions */
            leftSleepingTimeMs = 200;
            /* drive the servo 1 step (20 degrees) into the target position direction */
            if (servoDriveDirection == SERVO_DRIVE_CLOCKWISE)
            {
                servoCurrentPosition += 1;   
            }
            else
            {
                servoCurrentPosition -= 1;
            }
            pwmSetDutyCycle(servoCurrentPosition);
            SERVO_SWITCH_ON;
            
            /* state transitions */
            /* check if target position has already been reached 
                -> if not start again from the beginning of the state machine */
            if ((servoDriveDirection == SERVO_DRIVE_CLOCKWISE && servoCurrentPosition >= servoTargetPos) ||
            (servoDriveDirection == SERVO_DRIVE_ANTI_CLOCKWISE && servoCurrentPosition <= servoTargetPos))
            {
                subState = SUBSTATE_TARGETPOS_REACHED;
            }
            else
            {
                subState = SUBSTATE_START;
            }
        }
        /*****************************************************************************************/
        else if (subState == SUBSTATE_TARGETPOS_REACHED)
        {
            /* state actions */
            leftSleepingTimeMs = 0;
            
            /* state transitions */
            /* stop the state machine execution*/
            stateMachineExecuting = 0;
            SERVO_SWITCH_OFF;
        }
    }   
    
    disablePwm();
}