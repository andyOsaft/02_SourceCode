
#ifndef __MENUSCREENS_H
#define __MENUSCREENS_H

#define welcome 	     1
#define heatingManual   10
#define heatingAuto     11
#define voltageControl  20
#define voltageThres    21
#define gotoSleepmode   30
#define wakeupSleepmode 40
#define infoScreen		50
#define warningLightOn	60
#define warningBatt1	70
#define warningBatt2	71
#define tempInfo		80

#include <stdint.h>
#include <avr/io.h>
#include "lcdRoutines.h"
#include <util/delay.h>
#include "globals.h"
#include <stdlib.h>
#include "adcInit.h"

//Function prototypes
void menuInit();
void screenWelcome();
void screenHeatingManual(uint8_t heizStatus, uint8_t *heizLevel);
void screenHeatingAuto(uint16_t targetTemp);
void screenVoltageControl(uint16_t batt1Volt, uint16_t batt2Volt);
void screenVoltageThreshold(uint16_t batt1Thres, uint16_t batt2Thres);
void screenGotoSleepmode();
void screenWakeupSleepmode();
void screenInfo();
void screenLightWarning();
void screenWarningBatt1();
void screenWarningBatt2();
void screenTempInfo(uint16_t tempInside, uint16_t tempMotor);

#endif
