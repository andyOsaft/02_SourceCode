#ifndef INVENSENSE_IMU_MLOS_H__
#define INVENSENSE_IMU_MLOS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* - Error Codes. - */
#define MLOS_SUCCESS 0
#define MLOS_ERROR   1

void *MLOSMalloc(unsigned int numBytes);
unsigned char MLOSFree(void *ptr);
void MLOSSleep(int mSecs);
unsigned long MLOSGetTickCount();

#ifdef __cplusplus
}
#endif

#endif // INVENSENSE_IMU_MLOS_H__

