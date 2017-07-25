#ifndef _DRIVER_LED_H
#define _DRIVER_LED_H

#include "stm32f4xx.h"
#include "sys.h"
//#define LED0 PEout(2)
#define LED0 PFout(9)
#define LED1 PFout(10)

void LED0_Init(void);
void LED0_On(void);
void LED0_Off(void);

#endif //_DRIVER_LED_H
