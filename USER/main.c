#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "driver_led.h"
#include "includes.h"
#include "adis16488.h"

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			60 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			59 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);


//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			58 
//���������ջ��С
#define LED1_STK_SIZE  					64
//�����ջ
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

#define SPI_TASK_PRIO       			50 
//���������ջ��С
#define SPI_STK_SIZE  					128
//�����ջ
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//������
void spi_task(void *pdata);


int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED0_Init();		        //��ʼ��LED�˿� 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(spi_task,(void *)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO);						   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
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



