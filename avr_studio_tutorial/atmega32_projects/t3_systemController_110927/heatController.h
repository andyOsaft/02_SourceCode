
#ifndef __HEATCONTROLLER_H
#define __HEATCONTROLLER_H


#include <avr/io.h>
#include "lcdRoutines.h"
#include <util/delay.h>
#include "pwmInit.h"
#include "adcInit.h"

//Funktions-Prototypen

void updateHeatController(uint8_t targetTemp);




#endif




/*
void heizInit();
void motorRechtslauf(uint8_t anzahlSchritte);
void motorLinkslauf(uint8_t anzahlSchritte);
void displayUpdate(uint8_t heizStufe);
void lcdHeizung(uint8_t dispPos);
void lcdKuehlung(uint8_t dispPos);
void lcdAusschalten();
*/
