/*********************************************************************************

Globale Variablen der Software

*********************************************************************************/

#include "globals.h"

//Variable die signalisiert ob der uC gerade aufgewacht ist
// =1  -> gerade aufgewacht
// !=1 -> keine Auswirkung
uint8_t wakeUp=0;

//Flags f�r die Buttons rechts und mitte
//gedr�ckt   -> =0
//ungedr�ckt -> =1
//werden in den ISRs f�r die beiden Buttons gesetzt.
//volatile uint8_t buttonRechts=1;
//volatile uint8_t buttonMitte=1;



