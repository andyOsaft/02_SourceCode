	

#ifndef __PWMINIT_H
#define __PWMINIT_H

#include "globals.h"

//Funktionsprototypen
void pwmInit(void);
void pwmSetDutyCycle(uint8_t compare);
void sleepDeeplyMs(void);
void disablePwm(void);
void enablePwm(void);


//Register TCCR0A -> Timer/Counter 0 Control Register A
#define WGM00	0  //Bit 0 - 1 -> Waveform Generation Mode
#define WGM01	1
#define COM0B0	4  //Bit 4 - 7 -> Compare Output Mode
#define	COM0B1	5
#define COM0A0	6
#define COM0A1	7


//Register TCCR1B – Timer/Counter 1 Control Register B
#define CS00	0	//Bit 0 - 2 -> Clock Select
#define	CS01	1
#define CS02	2
#define WGM02	3	//Bit 3 - 4 -> Waveform Generation Mode
#define FOC0B   6   //Force Output Compare B
#define	FOC0A	7	//Force Output Compare A


//Register TIMSK0 - Timeer Interrupt Mask Register
#define TOIE0   1   //Timer Overflow Interrupt Enable 
#define OCIE0A  2   //Timer Output Compare Match A Interrupt Enable
#define OCIE0B  3   //Timer Output Compare Match B Interrupt Enable




#endif
