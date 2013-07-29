#include "twi.h"
#include "imu.h"
#include "mpuregs.h"
#include "avr.h"

static uint8_t fifoFlags = 0x0;

int imu_writeByte(uint8_t reg, uint8_t value)
{
        uint8_t buf[2] = { reg, value };
        return twi_writeTo(IMU_I2C_ADDRESS, buf, 2, 1);
}

uint8_t imu_readByte(uint8_t reg)
{
        int result;
        uint8_t data;

        // Write register to read from.
        result = twi_writeTo(IMU_I2C_ADDRESS, &reg, 1, 1);
        if (result!=0) return -1;

        // Read data
        twi_readFrom(IMU_I2C_ADDRESS, &data, 1);
        return data;
}

uint16_t imu_readWord(uint8_t reg)
{
        int result;
        uint8_t data[2];

        // Write register to read from.
        result = twi_writeTo(IMU_I2C_ADDRESS, &reg, 1, 1);
        if (result!=0) return -1;

        // Read data
        twi_readFrom(IMU_I2C_ADDRESS, data, 2);
        return (data[0]<<8) | data[1];
}

int imu_read(uint8_t reg, uint8_t * buf, uint16_t count)
{
        uint8_t result = count;

        // Write register
        result = twi_writeTo(IMU_I2C_ADDRESS, &reg, 1, 1);
        if (result!=0) return -1;

        // Read data
        result = twi_readFrom(IMU_I2C_ADDRESS, buf, count);
        return(result);
}

int imu_writeAuxByte(uint8_t reg, uint8_t value)
{
        uint8_t buf[2] = { reg, value };
        return twi_writeTo(IMU_I2C_AUX_ADDRESS, buf, 2, 1);
}

int imu_fifoEnable(uint8_t flags)
{
        fifoFlags = flags;
        return imu_writeByte(MPUREG_FIFO_EN1, flags);
}

int imu_fifoCount()
{
        return (int)imu_readWord(MPUREG_FIFO_COUNTH);
}

uint16_t imu_fifoReadWord()
{
        return imu_readWord(MPUREG_FIFO_R_W);
}

int imu_registerRead(imu_sensorData *data)
{
        data->temp = imu_readWord(MPUREG_TEMP_OUT_H);
        data->gyro_x = imu_readWord(MPUREG_GYRO_XOUT_H);
        data->gyro_y = imu_readWord(MPUREG_GYRO_YOUT_H);
        data->gyro_z = imu_readWord(MPUREG_GYRO_ZOUT_H);
        data->acc_z = imu_readWord(35);
        data->acc_z = imu_readWord(37);
        data->acc_z = imu_readWord(39);

        return 0;
}

static int bitcount(uint8_t value)
{
        uint8_t ret;
        for (ret=0; value; value>>=1) ret+=value&1;
        return ret;
}

int imu_fifoRead(imu_sensorData *data)
{
        int bufPos = 0, ret = 0;
        uint8_t buf[16];

        // Determine how many values to read from the fifo.
        // We do a bit count on the fifo enable flags to figure out how many outputs are turned on and subtract
        // one to account for the footer that we don't want to read.
        int valueCount = bitcount(fifoFlags) -1;

        // Read data from the FIFO.
        ret = imu_fifoFromFooter(buf, valueCount);
        if (ret<0) return ret;

        // Decode the buffer data
        if (fifoFlags & IMU_FIFO_TEMP) { data->temp = (buf[bufPos]<<8) | buf[bufPos+1]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_GYRO_X) { data->gyro_x = (buf[bufPos]<<8) | buf[bufPos+1]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_GYRO_Y) { data->gyro_y = (buf[bufPos]<<8) | buf[bufPos+1]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_GYRO_Z) { data->gyro_z = (buf[bufPos]<<8) | buf[bufPos+1]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_AUX_X) { data->acc_x = (buf[bufPos+1]<<8) | buf[bufPos]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_AUX_Y) { data->acc_y = (buf[bufPos+1]<<8) | buf[bufPos]; bufPos +=2; }
        if (fifoFlags & IMU_FIFO_AUX_Z) { data->acc_z = (buf[bufPos+1]<<8) | buf[bufPos]; bufPos +=2; }

        return ret;
}

int imu_fifoFromFooter(uint8_t * buf, int len)
{
        int i, fifoCount, read = 0;
        uint8_t scan[2];

        fifoCount = imu_fifoCount();

        // Fifo needs to hold at least 2 footers (==4 bytes) and len data words.
        if (fifoCount<4 + len*2) return -1;

        // Scan for footer
        for (i=0; i<fifoCount; i++)
        {
                scan[0] = scan[1]; scan[1] = imu_readByte(MPUREG_FIFO_R_W);
                if ((i>0) && (scan[0]==0x0C) && (scan[1]==0x2A)) break;
                fifoCount --;
        }

        // Make sure that after the first footer found, there's still at least len+2 words in the fifo
        if (fifoCount< 2 + len*2) return -2;

        // Read len bytes from the fifo
        for (i=0; i<len*2; i++)
                buf[i] = imu_readByte(MPUREG_FIFO_R_W);

        return len;
}

int imu_fifoClear()
{
        while (imu_fifoCount() > 0)
                imu_readByte(MPUREG_FIFO_R_W);

        return 0;
}


int imu_setDigitalLowPassFilter(uint8_t dlpf)
{
        uint8_t reg = imu_readByte(MPUREG_DLPF_FS_SYNC);
        return imu_writeByte(MPUREG_DLPF_FS_SYNC, (reg & 0b11111000) | dlpf);
}

int imu_setSamplingRateDivider(uint8_t divider)
{
        return imu_writeByte(MPUREG_SMPLRT_DIV, divider);
}

int imu_setGyroFullScale(uint8_t scale)
{
        uint8_t reg = imu_readByte(MPUREG_DLPF_FS_SYNC);
        return imu_writeByte(MPUREG_DLPF_FS_SYNC, (reg & 0b11000111) | (scale << 3));
}

static int imu_setPassThrough(uint8_t enable)
{
        uint8_t reg = imu_readByte(MPUREG_USER_CTRL);

        // Set the auxilary interface enable bit.
        if (enable)
                reg |= BIT_AUX_IF_EN;
        else
                reg &= ~BIT_AUX_IF_EN;

        // Always reset the auxilary interface.
        reg |= BIT_AUX_IF_RST;

        imu_writeByte(MPUREG_USER_CTRL, reg);

        return 0;
}

int imu_enablePassThrough()
{
        return imu_setPassThrough(0);
}

int imu_disablePassThrough()
{
        return imu_setPassThrough(1);
}
