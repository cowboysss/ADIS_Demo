#ifndef __KEY_PRESS__H_
#define __KEY_PRESS__H_

#include "sys.h"
#define KEY1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) //PB12
#define KEY2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) //PB12
#define KEY3 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) //PB14
#define KEY4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) //PB15
void key_sdWrite(void);

#endif
