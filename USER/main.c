#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "driver_led.h"
#include "includes.h"
#include "adis16488.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			60 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			59 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			58 
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

#define SPI_TASK_PRIO       			50 
//设置任务堆栈大小
#define SPI_STK_SIZE  					128
//任务堆栈
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//任务函数
void spi_task(void *pdata);


int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED0_Init();		        //初始化LED端口 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(spi_task,(void *)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO);						   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//LED0任务
void led0_task(void *pdata)
{	 	
	while(1)
	{
		LED0=0;
		delay_ms(500);
		LED0=1;
		delay_ms(500);
	};
}
u16 DeviceID=0;
u32 data;
void spi_task(void *pdata)
{
	/* Init Spi */
	ADISInit();
	uart_init(115200);
	/* Do spi transmission */
	while(1)
	{
		/* One time transmission */
//		DeviceID = getID();
		ADIS_Read9AxisData();
		delay_ms(500);
//		DeviceID=0;
	}
}



