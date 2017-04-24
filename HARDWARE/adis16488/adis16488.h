#ifndef ADIS_16488__H_
#define ADIS_16488__H_ ADIS_16488__H_

#include "sys.h"
/**
 * ADIS 16488 register address
 */
// #ifdef  ADIS_16488
//      Name            Address     Default Register Description Format PAGE_ID R/W Flash
#define PAGE_ID         0x00        //0x00 Page identifier N/A 0x00 R/W No
//#define Reserved      0x02 to 0x04//N/A Reserved N/A 0x00 N/A N/A
#define SEQ_CNT         0x06        //N/A Sequence counter Table 56 0x00 R No
#define SYS_E_FLAG      0x08        //0x0000 Output, system error flags Table 47 0x00 R No
#define DIAG_STS        0x0A        //0x0000 Output, self-test error flags Table 48 0x00 R No
#define ALM_STS         0x0C        //0x0000 Output, alarm error flags Table 49 0x00 R No
#define TEMP_OUT        0x0E        //N/A Output, temperature Table 45 0x00 R No
#define X_GYRO_LOW      0x10        //N/A Output, x-axis gyroscope, low word Table 14 0x00 R No
#define X_GYRO_OUT      0x12        //N/A Output, x-axis gyroscope, high word Table 10 0x00 R No
#define Y_GYRO_LOW      0x14        //N/A Output, y-axis gyroscope, low word Table 15 0x00 R No
#define Y_GYRO_OUT      0x16        //N/A Output, y-axis gyroscope, high word Table 11 0x00 R No
#define Z_GYRO_LOW      0x18        //N/A Output, z-axis gyroscope, low word Table 16 0x00 R No
#define Z_GYRO_OUT      0x1A        //N/A Output, z-axis gyroscope, high word Table 12 0x00 R No
#define X_ACCL_LOW      0x1C        //N/A Output, x-axis accelerometer, low word Table 21 0x00 R No
#define X_ACCL_OUT      0x1E        //N/A Output, x-axis accelerometer, high word Table 17 0x00 R No
#define Y_ACCL_LOW      0x20        //N/A Output, y-axis accelerometer, low word Table 22 0x00 R No
#define Y_ACCL_OUT      0x22        //N/A Output, y-axis accelerometer, high word Table 18 0x00 R No
#define Z_ACCL_LOW      0x24        //N/A Output, z-axis accelerometer, low word Table 23 0x00 R No
#define Z_ACCL_OUT      0x26        //N/A Output, z-axis accelerometer, high word Table 19 0x00 R No
#define X_MAGN_OUT      0x28        //N/A Output, x-axis magnetometer, high word Table 38 0x00 R No
#define Y_MAGN_OUT      0x2A        //N/A Output, y-axis magnetometer, high word Table 39 0x00 R No
#define Z_MAGN_OUT      0x2C        //N/A Output, z-axis magnetometer, high word Table 40 0x00 R No
#define BAROM_LOW       0x2E        //N/A Output, barometer, low word Table 44 0x00 R No
#define BAROM_OUT       0x30        //N/A Output, barometer, high word Table 42 0x00 R No
//#define Reserved      0x32 to 0x3E//N/A Reserved N/A 0x00 N/A N/A
#define X_DELTANG_LOW   0x40        //N/A Output, x-axis delta angle, low word Table 28 0x00 R No
#define X_DELTANG_OUT   0x42        //N/A Output, x-axis delta angle, high word Table 24 0x00 R No
#define Y_DELTANG_LOW   0x44        //N/A Output, y-axis delta angle, low word Table 29 0x00 R No
#define Y_DELTANG_OUT   0x46        //N/A Output, y-axis delta angle, high word Table 25 0x00 R No
#define Z_DELTANG_LOW   0x48        //N/A Output, z-axis delta angle, low word Table 30 0x00 R No
#define Z_DELTANG_OUT   0x4A        //N/A Output, z-axis delta angle, high word Table 26 0x00 R No
#define X_DELTVEL_LOW   0x4C        //N/A Output, x-axis delta velocity, low word Table 35 0x00 R No
#define X_DELTVEL_OUT   0x4E        //N/A Output, x-axis delta velocity, high word Table 31 0x00 R No
#define Y_DELTVEL_LOW   0x50        //N/A Output, y-axis delta velocity, low word Table 36 0x00 R No
#define Y_DELTVEL_OUT   0x52        //N/A Output, y-axis delta velocity, high word Table 32 0x00 R No
#define Z_DELTVEL_LOW   0x54        //N/A Output, z-axis delta velocity, low word Table 37 0x00 R No
#define Z_DELTVEL_OUT   0x56        //N/A Output, z-axis delta velocity, high word Table 33 0x00 R No
//#define Reserved      0x58 to 0x76//N/A Reserved N/A 0x00 N/A N/A
#define TIME_MS_OUT     0x78        //N/A Factory configuration time: minutes/seconds Table 124 0x00 R/W Yes
#define TIME_DH_OUT     0x7A        //N/A Factory configuration date/time: day/hour Table 125 0x00 R/W Yes
#define TIME_YM_OUT     0x7C        //N/A Factory configuration date: year/month Table 126 0x00 R/W Yes
#define PROD_ID         0x7E        //0x4068 Output, product identification -16,488 Table 53 0x00 R Yes
//#define Reserved      0x00 to 0x7E//N/A Reserved N/A 0x01 N/A N/A
#define PAGE_ID         0x00        //0x00 Page identifier N/A 0x02 R/W No
//#define Reserved      0x02        //N/A Reserved N/A 0x02 N/A N/A
#define X_GYRO_SCALE    0x04        //0x0000 Calibration, scale, x-axis gyroscope Table 71 0x02 R/W Yes
#define Y_GYRO_SCALE    0x06        //0x0000 Calibration, scale, y-axis gyroscope Table 72 0x02 R/W Yes
#define Z_GYRO_SCALE    0x08        //0x0000 Calibration, scale, z-axis gyroscope Table 73 0x02 R/W Yes
#define X_ACCL_SCALE    0x0A        //0x0000 Calibration, scale, x-axis accelerometer Table 81 0x02 R/W Yes
#define Y_ACCL_SCALE    0x0C        //0x0000 Calibration, scale, y-axis accelerometer Table 82 0x02 R/W Yes
#define Z_ACCL_SCALE    0x0E        //0x0000 Calibration, scale, z-axis accelerometer Table 83 0x02 R/W Yes
#define XG_BIAS_LOW     0x10        //0x0000 Calibration, offset, gyroscope, x-axis, low word Table 67 0x02 R/W Yes
#define XG_BIAS_HIGH    0x12        //0x0000 Calibration, offset, gyroscope, x-axis, high word Table 64 0x02 R/W Yes
#define YG_BIAS_LOW     0x14        //0x0000 Calibration, offset, gyroscope, y-axis, low word Table 68 0x02 R/W Yes
#define YG_BIAS_HIGH    0x16        //0x0000 Calibration, offset, gyroscope, y-axis, high word Table 65 0x02 R/W Yes
#define ZG_BIAS_LOW     0x18        //0x0000 Calibration, offset, gyroscope, z-axis, low word Table 69 0x02 R/W Yes
#define ZG_BIAS_HIGH    0x1A        //0x0000 Calibration, offset, gyroscope, z-axis, high word Table 66 0x02 R/W Yes
#define XA_BIAS_LOW     0x1C        //0x0000 Calibration, offset, accelerometer, x-axis, low word Table 78 0x02 R/W Yes
#define XA_BIAS_HIGH    0x1E        //0x0000 Calibration, offset, accelerometer, x-axis, high word Table 75 0x02 R/W Yes
#define YA_BIAS_LOW     0x20        //0x0000 Calibration, offset, accelerometer, y-axis, low word Table 79 0x02 R/W Yes
#define YA_BIAS_HIGH    0x22        //0x0000 Calibration, offset, accelerometer, y-axis, high word Table 76 0x02 R/W Yes
#define ZA_BIAS_LOW     0x24        //0x0000 Calibration, offset, accelerometer, z-axis, low word Table 80 0x02 R/W Yes
#define ZA_BIAS_HIGH    0x26        //0x0000 Calibration, offset, accelerometer, z-axis, high word Table 77 0x02 R/W Yes
#define HARD_IRON_X     0x28        //0x0000 Calibration, hard iron, magnetometer, x-axis Table 84 0x02 R/W Yes
#define HARD_IRON_Y     0x2A        //0x0000 Calibration, hard iron, magnetometer, y-axis Table 85 0x02 R/W Yes
#define HARD_IRON_Z     0x2C        //0x0000 Calibration, hard iron, magnetometer, z-axis Table 86 0x02 R/W Yes
#define SOFT_IRON_S11   0x2E        //0x0000 Calibration, soft iron, magnetometer, S11 Table 88 0x02 R/W Yes
#define SOFT_IRON_S12   0x30        //0x0000 Calibration, soft iron, magnetometer, S12 Table 89 0x02 R/W Yes
#define SOFT_IRON_S13   0x32        //0x0000 Calibration, soft iron, magnetometer, S13 Table 90 0x02 R/W Yes
#define SOFT_IRON_S21   0x34        //0x0000 Calibration, soft iron, magnetometer, S21 Table 91 0x02 R/W Yes
#define SOFT_IRON_S22   0x36        //0x0000 Calibration, soft iron, magnetometer, S22 Table 92 0x02 R/W Yes
#define SOFT_IRON_S23   0x38        //0x0000 Calibration, soft iron, magnetometer, S23 Table 93 0x02 R/W Yes
#define SOFT_IRON_S31   0x3A        //0x0000 Calibration, soft iron, magnetometer, S31 Table 94 0x02 R/W Yes
#define SOFT_IRON_S32   0x3C        //0x0000 Calibration, soft iron, magnetometer, S32 Table 95 0x02 R/W Yes
#define SOFT_IRON_S33   0x3E        //0x0000 Calibration, soft iron, magnetometer, S33 Table 96 0x02 R/W Yes
#define BR_BIAS_LOW     0x40        //0x0000 Calibration, offset, barometer, low word Table 99 0x02 R/W Yes
#define BR_BIAS_HIGH    0x42        //0x0000 Calibration, offset, barometer, high word Table 98 0x02 R/W Yes
//#define Reserved      0x44 to 0x72//N/A Reserved N/A 0x02 N/A N/A
#define USER_SCR_1      0x74        //0x0000 User Scratch Register 1 Table 120 0x02 R/W Yes
#define USER_SCR_2      0x76        //0x0000 User Scratch Register 2 Table 121 0x02 R/W Yes
#define USER_SCR_3      0x78        //0x0000 User Scratch Register 3 Table 122 0x02 R/W Yes
#define USER_SCR_4      0x7A        //0x0000 User Scratch Register 4 Table 123 0x02 R/W Yes
#define FLSHCNT_LOW     0x7C        //N/A Diagnostic, flash memory count, low word Table 115 0x02 R Yes
#define FLSHCNT_HIGH    0x7E        //N/A Diagnostic, flash memory count, high word Table 116 0x02 R Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x03 R/W No
#define GLOB_CMD        0x02        //N/A Control, global commands Table 114 0x03 W No
//#define Reserved      0x04        //N/A Reserved N/A 0x03 N/A N/A
#define FNCTIO_CTRL     0x06        //0x000D Control, I/O pins, functional definitions Table 117 0x03 R/W Yes
#define GPIO_CTRL       0x08        //0x00X0 1 Control, I/O pins, general purpose Table 118 0x03 R/W Yes
#define CONFIG          0x0A        //0x00C0 Control, clock, and miscellaneous correction Table 74 0x03 R/W Yes
#define DEC_RATE        0x0C        //0x0000 Control, output sample rate decimation Table 55 0x03 R/W Yes
#define NULL_CNFG       0x0E        //0x070A Control, automatic bias correction configuration Table 70 0x03 R/W Yes
#define SLP_CNT         0x10        //N/A Control, power-down/sleep mode Table 119 0x03 R/W No
//#define Reserved      0x12 to 0x14//N/A Reserved N/A 0x03 N/A N/A
#define FILTR_BNK_0     0x16        //0x0000 Filter selection Table 57 0x03 R/W Yes
#define FILTR_BNK_1     0x18        //0x0000 Filter selection Table 58 0x03 R/W Yes
//#define Reserved      0x1A to 0x1E//N/A Reserved N/A Reserved N/A 0x03 N/A N/A
#define ALM_CNFG_0      0x20        //0x0000 Alarm configuration Table 110 0x03 R/W Yes
#define ALM_CNFG_1      0x22        //0x0000 Alarm configuration Table 111 0x03 R/W Yes
#define ALM_CNFG_2      0x24        //0x0000 Alarm configuration Table 112 0x03 R/W Yes
//#define Reserved      0x26        //N/A Reserved N/A 0x03 N/A N/A
#define XG_ALM_MAGN     0x28        //0x0000 Alarm, x-axis gyroscope threshold setting Table 100 0x03 R/W Yes
#define YG_ALM_MAGN     0x2A        //0x0000 Alarm, y-axis gyroscope threshold setting Table 101 0x03 R/W Yes
#define ZG_ALM_MAGN     0x2C        //0x0000 Alarm, z-axis gyroscope threshold setting Table 102 0x03 R/W Yes
#define XA_ALM_MAGN     0x2E        //0x0000 Alarm, x-axis accelerometer threshold Table 103 0x03 R/W Yes
#define YA_ALM_MAGN     0x30        //0x0000 Alarm, y-axis accelerometer threshold Table 104 0x03 R/W Yes
#define ZA_ALM_MAGN     0x32        //0x0000 Alarm, z-axis accelerometer threshold Table 105 0x03 R/W Yes
#define XM_ALM_MAGN     0x34        //0x0000 Alarm, x-axis magnetometer threshold Table 106 0x03 R/W Yes
#define YM_ALM_MAGN     0x36        //0x0000 Alarm, y-axis magnetometer threshold Table 107 0x03 R/W Yes
#define ZM_ALM_MAGN     0x38        //0x0000 Alarm, z-axis magnetometer threshold Table 108 0x03 R/W Yes
#define BR_ALM_MAGN     0x3A        //0x0000 Alarm, barometer threshold setting Table 109 0x03 R/W Yes
//#define Reserved      0x3C to 0x76//N/A Reserved N/A Reserved N/A 0x03 N/A N/A
#define FIRM_REV        0x78        //N/A Firmware revision Table 50 0x03 R Yes
#define FIRM_DM         0x7A        //N/A Firmware programming date: day/month Table 51 0x03 R Yes
#define FIRM_Y          0x7C        //N/A Firmware programming date: year Table 52 0x03 R Yes
//#define Reserved      0x7E        //N/A Reserved N/A 0x03 N/A N/A
//#define Reserved      0x00 to 0x18//N/A Reserved N/A 0x04 N/A N/A
#define SERIAL_NUM      0x20        //N/A Serial number Table 54 0x04 R Yes
//#define Reserved      0x22 to 0x7F//N/A Reserved N/A 0x04 N/A N/A
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x05 R/W No
//#define FIR_COEF_Axxx     0x02 to 0x7E//N/A FIR Filter Bank A, Coefficients 0 through 59 Table 59 0x05 R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x06 R/W No
//#define FIR_COEF_Axxx     0x02 to 0x7E//N/A FIR Filter Bank A, Coefficients 60 through 119 Table 59 0x06 R/W Yes
#define PAGE_ID         0x00        //0x0000 0x02 to 0x7E identifier N/A 0x07 R/W No
//#define FIR_COEF_Bxxx     0x02 to 0x7E//N/A FIR Filter Bank B, Coefficients 0 through 59 Table 60 0x07 R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x08 R/W No
//#define FIR_COEF_Bxxx     0x02 to 0x7E//N/A FIR Filter Bank B, Coefficients 60 through 119 Table 60 0x08 R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x09 R/W No
//#define FIR_COEF_Cxxx     0x02 to 0x7E//N/A FIR Filter Bank C, Coefficients 0 through 59 Table 61 0x09 R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x0A R/W No
//#define FIR_COEF_Cxxx     0x02 to 0x7E//N/A FIR Filter Bank C, Coefficients 60 through 119 Table 61 0x0A R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x0B R/W No
//#define FIR_COEF_Dxxx     0x02 to 0x7E//N/A FIR Filter Bank D, Coefficients 0 through 59 Table 62 0x0B R/W Yes
#define PAGE_ID         0x00        //0x0000 Page identifier N/A 0x0C R/W No
//#define FIR_COEF_Dxxx     0x02 to 0x7E//N/A FIR Filter Bank D, Coefficients 60 through 119 Table 62 0x0C R/W Yes 
// #endif


/* function declaration */
void ADISInit(void);
void enableADIS(void);
void disableADIS(void);
u16 ADISRead(u8 addr);
u16 getID(void);
u16 ADIS_ReadWriteByte(u16 TxData);
//u32 ADIS_ReadData(void);
int16_t ADIS_ReadDataOut(u16 addr);
int16_t complement2Tureform(u16 code);
void ADIS_Read9AxisData(void);

#endif //ADIS_16488__H_
