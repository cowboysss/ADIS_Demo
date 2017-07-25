#include "driver_led.h"

void LED0_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO

	GPIO_SetBits(GPIOF,GPIO_Pin_9);//GPIOF9,F10设置高，灯灭
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//GPIOF9,F10设置高，灯灭
}

//LED0 low votage on
void LED0_On(void)
{
    GPIO_ResetBits(GPIOF,GPIO_Pin_9);
}
//LED0 high votage off
void LED0_Off(void)
{
    GPIO_SetBits(GPIOF,GPIO_Pin_9);
}

