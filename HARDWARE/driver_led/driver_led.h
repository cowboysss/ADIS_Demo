#ifndef _DRIVER_LED_H
#define _DRIVER_LED_H

#include "stm32f4xx.h"
#include "sys.h"
#define LED0 PCout(0) // mount/unmount
#define LED1 PCout(1) // open/close
#define LED2 PCout(2) // reserved
#define LEDRUN PCout(3) // program run


void LED_Init(void);
//void LED0_On(void);
//void LED0_Off(void);

#endif //_DRIVER_LED_H
