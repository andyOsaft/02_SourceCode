/*********************************************************************************

Globale Variablen der Software

*********************************************************************************/

#include "globals.h"

//Variable die signalisiert ob der uC gerade aufgewacht ist
// =1  -> gerade aufgewacht
// !=1 -> keine Auswirkung
uint8_t wakeUp=0;

//Flags für die Buttons rechts und mitte
//gedrückt   -> =0
//ungedrückt -> =1
//werden in den ISRs für die beiden Buttons gesetzt.
//volatile uint8_t buttonRechts=1;
//volatile uint8_t buttonMitte=1;



