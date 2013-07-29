/*******************************************************************************
 * Copyright (c) 2010 InvenSense Corporation, All Rights Reserved.
 * See license.txt for license.
 ******************************************************************************/

#include "imuMlsl.h"
#include <stdio.h>
#include "imuSetup.h"
#include "imuMldl.h"
#include "imuMlos.h"

#include "avr.h"
#include "twi.h"

/**
 *   @defgroup MLSL
 *   @brief ML Serial Layer Interface.
 *
 *   @{
 *       @file imuMlsl.c
 *       @brief ML Serial Layer Interface.
**/

tWriteBurst WriteBurst = &MLSLSerialWriteBurst;
tReadBurst ReadBurst = &MLSLSerialReadBurst;

tMLError IMUserialOpen(int_fast8_t port)
{
        twi_init();
        return ML_SUCCESS;
}

tMLError IMUserialClose(void)
{
        return ML_SUCCESS;
}

/**
 *  @brief  used to reset any buffering the driver may be doing
 *  @return Zero if the command is successful, an error code otherwise.
 */
tMLError MLSLSerialReset( void )
{
        twi_init();
    return ML_SUCCESS;
}

/**
 *  @brief  used to write a single byte of data.
 *          It is called by the MPL to write a single byte of data to the IMU. 
 *          This should be sent by I2C or SPI.
 *
 *  @param  slaveAddr       I2C slave address of device.
 *  @param  registerAddr    Register address to write.
 *  @param  data            Single byte of data to write.
 *
 *  @return Zero if the command is successful, an error code otherwise.
 */
tMLError MLSLSerialWriteSingle(unsigned char slaveAddr, unsigned char registerAddr, unsigned char data)
{
        // avr_serialPrintf("WriteSingle(%x, 0x%x (%d), %d)\n", slaveAddr, registerAddr, registerAddr, data);

        unsigned char buf[2] = { registerAddr, data };
        int result = twi_writeTo(slaveAddr, buf, 2, 1);
        if (result) return ML_ERROR;

        return ML_ERROR;
}

/**
 *  @brief  used to write multiple bytes of data.
 *          This should be sent by I2C or SPI.
 *
 *  @param  slaveAddr       I2C slave address of device.
 *  @param  registerAddr    Register address to write.
 *  @param  length          Length of burst data.
 *  @param  data            Pointer to block of data.
 *
 *  @return Zero if the command is successful, an error code otherwise.
 */
tMLError MLSLSerialWriteBurst(unsigned char slaveAddr, unsigned char registerAddr, unsigned short length, const unsigned char *data)
{
        // avr_serialPrintf("WriteBurst(%x, 0x%x (%d), %d)\n", slaveAddr, registerAddr, registerAddr, length);

        // Write target register
        int result = twi_writeTo(slaveAddr, &registerAddr, 1, 1);
        if (result) return ML_ERROR;

        // Write data
        result = twi_writeTo((uint8_t)slaveAddr, (uint8_t*)&data, length, 1);
        if (result) return ML_ERROR;

        return ML_SUCCESS;
}

/**
 *  @brief  used to read a single byte of data.
 *          This should be sent by I2C or SPI.
 *
 *  @param  slaveAddr       I2C slave address of device.
 *  @param  registerAddr    Register address to read.
 *  @param  data            Single byte of data to read.
 *
 *  @return Zero if the command is successful, an error code otherwise.
 */
tMLError MLSLSerialReadSingle(unsigned char slaveAddr, unsigned char registerAddr, unsigned char *data)
{
        // avr_serialPrintf("ReadSingle(%x, 0x%x (%d))\n", slaveAddr, registerAddr, registerAddr);

        // Write target register
        int result = twi_writeTo(slaveAddr, &registerAddr, 1, 1);
        if (result) return ML_ERROR;

        // Read data
        result = twi_readFrom(slaveAddr, data, 1);
        if (result!=1) return ML_ERROR;

        return ML_SUCCESS;
}



/**
 *  @brief  used to read multiple bytes of data.
 *          This should be sent by I2C or SPI.
 *
 *  @param  slaveAddr       I2C slave address of device.
 *  @param  registerAddr    Register address to read.
 *  @param  length          Length of burst data.
 *  @param  data            Pointer to block of data.
 *
 *  @return Zero if the command is successful; an error code otherwise
 */
tMLError MLSLSerialReadBurst(unsigned char slaveAddr, unsigned char registerAddr, unsigned short length, unsigned char *data)
{
        // avr_serialPrintf("ReadBurst(%x, 0x%x (%d), %d)\n", slaveAddr, registerAddr, registerAddr, length);

        // Write target register
        int result = twi_writeTo(slaveAddr, &registerAddr, 1, 1);
        if (result) return ML_ERROR;

        // Read data
        result = twi_readFrom(slaveAddr, data, length);
        if (result!=length) return ML_ERROR;

        return ML_SUCCESS;
}
/**
 *  @brief  used to handle the interrupt.
 *          MLSLIntHandler is the Motion Library Interrupt Handler.
 *          It should be called when the system kernel/operating system has
 *          detected the motion processing interrupt from the interrupt
 *          output pin on the IMU (pin 12).
 *          This could also be from a timer interrupt used to trigger the
 *          motion library. The reason for the interrupt is passed in the
 *          'intSource' argument.
 *          The valid values for this are as follows:
 *          - INTSRC_IMU,
 *          - INTSRC_AUX1,
 *          - INTSRC_AUX2,
 *          - INTSRC_TIMER, and
 *          - INTSRC_UNKNOWN.
 *
 *          Note that this routine should not be called directly from the ISR
 *          (interrupt service routine).
 *
 *  @param  intSource  Interrupt source.
 *
 *  @return Zero if the command is successful, an error code otherwise.
 */
tMLError MLSLIntHandler( unsigned char intSource )
{
    return ML_SUCCESS;
}


  /*********************/
 /** \}*/ /* defgroup */
/*********************/
