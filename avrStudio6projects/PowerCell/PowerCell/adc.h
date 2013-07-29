

#ifndef	__ADCINIT_H
#define __ADCINIT_H


#include <stdint.h>

#define BATTERY_VOLTAGE                 0
#define TEMPERATURE_SENSOR_VOLTAGE      1
#define SOLAR_REGULATOR_CURRENT         2
#define WIND_REGULATOR_CURRENT          3
#define CONSUMER_CURRENT                4

#define SOLAR_REGULATOR_SHUNT_mOHM       750
#define CONSUMER_CURRENT_SHUNT_mOHM      750

//Funktionsprototypen
void adcInit();
uint16_t adcRead(uint8_t channel);
uint16_t measureBattVoltage();
uint16_t measureSolarChargeCurrent();
uint16_t measureConsumerCurrent();


#endif
