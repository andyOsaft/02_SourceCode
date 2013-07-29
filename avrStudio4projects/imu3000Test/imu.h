#ifndef __imu_h__
#define __imu_h__

#include <stdint.h>

#define IMU_I2C_ADDRESS 0x68
#define IMU_I2C_AUX_ADDRESS 0x0F

#define IMU_FIFO_FOOTER 0x01
#define IMU_FIFO_AUX_Z 0x02
#define IMU_FIFO_AUX_Y 0x04
#define IMU_FIFO_AUX_X 0x08
#define IMU_FIFO_GYRO_Z 0x10
#define IMU_FIFO_GYRO_Y 0x20
#define IMU_FIFO_GYRO_X 0x40
#define IMU_FIFO_TEMP 0x80
#define IMU_FIFO_ALL 0xFF

typedef struct _imu_sensorData
{
        int16_t gyro_x, gyro_y, gyro_z;
        int16_t acc_x, acc_y, acc_z;
        int16_t temp;
} imu_sensorData;

int imu_writeByte(uint8_t reg, uint8_t value);
uint8_t imu_readByte(uint8_t reg);
uint16_t imu_readWord(uint8_t reg);
int imu_read(uint8_t reg, uint8_t * buf, uint16_t count);

int imu_writeAuxByte(uint8_t reg, uint8_t value);

int imu_fifoEnable(uint8_t flags);
int imu_fifoCount();
uint16_t imu_fifoReadWord();
int imu_registerRead(imu_sensorData *data);
int imu_fifoRead(imu_sensorData *data);
int imu_fifoFromFooter(uint8_t * buf, int len);
int imu_fifoClear();

int imu_setDigitalLowPassFilter(uint8_t dlpf);
int imu_setSamplingRateDivider(uint8_t divider);
int imu_setGyroFullScale(uint8_t scale);

int imu_enablePassThrough();
int imu_disablePassThrough();

#endif

