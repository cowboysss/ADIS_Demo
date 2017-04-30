#ifndef __INTERFACE__H__
#define __INTERFACE__H__ __INTERFACE__H__

#include "stm32f4xx.h"

/* IMU_Data is a struct after scale conversion, which is a true value of sensors */
typedef struct
{                   // unit
    double gyro[3]; // degree/sec
    double accl[3]; // mg
    double magn[3]; // mguass
    double baro;    // micro bar
    double temp;    // degree centigrade
}IMU_Data;

/* IMU_Data_Raw is a struct, which is a raw data from IMU. Values can multi a scale factor */
typedef struct
{                           // unit
    int16_t gyro_out[3];    // 0.02 degree/sec
    u16 gyro_low[3];        //
    int16_t accl_out[3];    // 0.8 mg
    u16 accl_low[3];        //
    int16_t magn_out[3];    // 0.1 mguass
    int16_t baro_out;       // 40 micro bar
    u16 baro_low;
    int16_t temp;           // degree centigrade
}IMU_Data_Raw;


#endif //__INTERFACE__H__
