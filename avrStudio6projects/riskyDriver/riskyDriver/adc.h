

#ifndef	__ADCINIT_H
#define __ADCINIT_H


#include <stdint.h>

#define BATTERY_VOLTAGE                 0

//Funktionsprototypen
void adcInit();
uint16_t adcRead(uint8_t channel);
uint16_t measureBattVoltage();


#endif
