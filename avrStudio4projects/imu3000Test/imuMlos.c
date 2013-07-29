/*******************************************************************************
 * Copyright (c) 2010 InvenSense Corporation, All Rights Reserved.
 * See license.txt for license.
 ******************************************************************************/

/**
 *   @defgroup MLOS
 *   @brief OS Interface.
 *
 *   @{
 *       @file imuMlos.c
 *       @brief OS Interface.
 **/

/* ------------- */
/* - Includes. - */
/* ------------- */

#include "imuMlos.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "avr.h"

/* -------------- */
/* - Functions. - */
/* -------------- */

/**
 *  @brief  Allocate space
 *  @param  numBytes  number of bytes
 *  @return pointer to allocated space
 **/
void *MLOSMalloc(unsigned int numBytes)
{
        // TODO implement
        // Not used ?
        return NULL;
}

/**
 *  @brief  Free allocated space
 *  @param  ptr pointer to space to deallocate
 *  @return error code.
 */
unsigned char MLOSFree(void *ptr)
{
        // TODO implement
        // Not used ?
        return MLOS_SUCCESS;
}

/**
 *  @brief  Sleep function.
 */
void MLOSSleep(int mSecs)
{
        avr_delay(mSecs);
        return;
}

/**
 *  @brief  get system's internal tick count in milliseconds.
 *          Used for time reference.
 *  @return current tick count.
 **/
unsigned long MLOSGetTickCount()
{
        return (long)avr_millis();
}

/*********************/
/** \}*//* defgroup */
/*********************/
