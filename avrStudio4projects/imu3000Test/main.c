#include <stdint.h>

#include "imuSetup.h"
#include "imuMldl.h"

#include "twi.h"
#include "avr.h"
#include "imu.h"
#include "fusion.h"

#include "mpuregs.h"

void imu_configKionix()
{
        int result = 0;

        imu_enablePassThrough();

        result += imu_writeAuxByte(0x1d, 0xcd); // RAM reset
        avr_delay(10);
        result += imu_writeAuxByte(0x1b, 0x42); // Wake up
        result += imu_writeAuxByte(0x1b, 0xc2); // Normal operation
        avr_delay(50);
        result += imu_writeAuxByte(0x1e, 0x14); // INT_CTRL_REG1: Configure non-latching wake-up
        result += imu_writeAuxByte(0x5a, 0x00); // WUF_THRESH:    wake-up threshold (on motion)
        result += imu_writeAuxByte(0x21, 0x06); // DATA_CTRL_REG: output data rate
        result += imu_writeAuxByte(0x29, 0x02); // WUF_TIMER:     wake-up timer

        imu_disablePassThrough();

        return result;
}

#define SAMPLE_FREQUENCY 200

void imu_init()
{
        // Reset IMU3000
        imu_writeByte(MPUREG_PWR_MGM, BIT_H_RESET);

        // Defaults for the IMU3000 evaluation board
        imu_writeByte(MPUREG_AUX_SLV_ADDR, KIONIX_AUX_SLAVEADDR);
        imu_writeByte(MPUREG_ACCEL_BURST_ADDR, 0x86);

        // Configure external accelerometer
        imu_configKionix();

        // Defaults can be changed later
        imu_setGyroFullScale(MPUFS_2000DPS);
        imu_setDigitalLowPassFilter(MPUFILTER_5HZ);
        imu_setSamplingRateDivider(19);

        // Enable the IMU fifo
        imu_fifoEnable(IMU_FIFO_ALL);
        imu_writeByte(MPUREG_USER_CTRL, 0b01101011);

        // Clear out the fifo
        imu_fifoClear();
}

//#define MEDIAN_FILTER_WINDOW 5
//static uint16_t filter

int main()
{
        imu_sensorData sensorData;

        // Init avr timers
        avr_timerInit();

        // Init serial port
        //avr_serialInit(57600);

        // Init I2C (two-wire interface)
        twi_init();

        // Blink
        DDRB = 0xff;
        PORTB = 0xff;

        uint8_t buf[100];
        int i;
        long time = 0;

        imu_init();

        // Divider is 4, so sample rate is 1KHz/5 = 200Hz
        // float gyroScale = (3.141592 / 100.0f) / 180.0f;

        float gyroScale = 16.4;

        fusion_reset();

        long startTime = avr_millis();

        while (1)
        {

                // wait for sample
                while (imu_fifoCount()<=2);

                int result = imu_fifoRead(&sensorData);
                if (result>0)
                {
                        // avr_serialPrintf("\rgx=%6d gy=%6d gz=%6d ax=%6d ay=%6d az=%6d temp=%6d %d %d\n ", sensorData.gyro_x, sensorData.gyro_y, sensorData.gyro_z, sensorData.acc_x, sensorData.acc_y, sensorData.acc_z, sensorData.temp, imu_fifoCount(), result);

                        /*
                        float gx = ((sensorData.gyro_x / gyroScale) * 3.141592 / 180.0f) / 100.0f;
                        float gy = ((sensorData.gyro_y / gyroScale) * 3.141592 / 180.0f) / 100.0f;
                        float gz = ((sensorData.gyro_z / gyroScale) * 3.141592 / 180.0f) / 100.0f;

                        float ax = sensorData.acc_x / 16384.0f;
                        float ay = sensorData.acc_y / 16384.0f;
                        float az = sensorData.acc_z / 16384.0f;
                        */

                        /*
                        gx = 0;
                        gy = 0;
                        gz = 0;

                        ax = 0;
                        ay = 0;
                        az = 0;
                        */

                        /*
                        fusion_update(gx, gy, gz, ax, ay, az);

                        if ((time%10)==0)
                        {
                                // avr_serialPrintf("G %f %f %f\n", gx, gy, gz);
                                // avr_serialPrintf("A %f %f %f\n", ax, ay, az);
                                // avr_serialPrintf("\rpitch=%f roll=%f yaw=%f\n", fusion_getPitch(), fusion_getRoll(), fusion_getYaw() );
                                avr_serialPrintf("%f %f %f\n", fusion_getPitch(), fusion_getRoll(), fusion_getYaw() );
                                // avr_serialPrintf("%f\n", (time * 1000) / (float)(avr_millis() - startTime));
                        }
                        */

                        uint8_t buf[128];

                        buf[0] = 0x12;
                        buf[1] = 0x34;
                        buf[2] = 0x56;
                        buf[3] = 0x78;

                        //for (i=0; i<4; i++)
                        //        avr_serialWrite(buf[i]);

                        buf[0] = time >> 24;
                        buf[1] = (time >> 16) & 0xff;
                        buf[2] = (time >> 8) & 0xff;
                        buf[3] = time & 0xff;

                        long millis = avr_millis();
                        buf[4] = millis >> 24;
                        buf[5] = (millis >> 16) & 0xff;
                        buf[6] = (millis >> 8) & 0xff;
                        buf[7] = millis & 0xff;

                        buf[8] = sensorData.acc_x >> 8;
                        buf[9] = sensorData.acc_x & 0xff;
                        buf[10] = sensorData.acc_y >> 8;
                        buf[11] = sensorData.acc_y & 0xff;
                        buf[12] = sensorData.acc_z >> 8;
                        buf[13] = sensorData.acc_z & 0xff;

                        buf[14] = sensorData.gyro_x >> 8;
                        buf[15] = sensorData.gyro_x & 0xff;
                        buf[16] = sensorData.gyro_y >> 8;
                        buf[17] = sensorData.gyro_y & 0xff;
                        buf[18] = sensorData.gyro_z >> 8;
                        buf[19] = sensorData.gyro_z & 0xff;

                        //for (i=0; i<20; i++)
                        //        avr_serialWrite(buf[i]);

                        time ++;

                }


        }

        return 0;
}
