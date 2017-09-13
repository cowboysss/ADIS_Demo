#include "driver_led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0);//置高，灯灭
	GPIO_SetBits(GPIOC,GPIO_Pin_1);//置高，灯灭
	GPIO_SetBits(GPIOC,GPIO_Pin_2);//置高，灯灭
	GPIO_SetBits(GPIOC,GPIO_Pin_3);//置高，灯灭
}

//LED0 low votage on
void LED0_On(void)
{
//    GPIO_ResetBits(GPIOC,GPIO_Pin_3);
}
//LED0 high votage off
void LED0_Off(void)
{
//    GPIO_SetBits(GPIOC,GPIO_Pin_3);
}

