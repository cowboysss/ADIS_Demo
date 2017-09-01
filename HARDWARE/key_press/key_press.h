#ifndef __KEY_PRESS__H_
#define __KEY_PRESS__H_

#include "sys.h"
#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) //PE3

void key_sdWrite(void);

#endif
