/*
 * Adapted from IMU.c/h by S.O.H. Madgwick
 * http://code.google.com/p/imumargalgorithm30042010sohm/
 */

#ifndef __fusion_h_
#define __fusion_h_

void fusion_reset();
void fusion_update(float gx, float gy, float gz, float ax, float ay, float az);
float fusion_getPitch();
float fusion_getYaw();
float fusion_getRoll();

#endif
