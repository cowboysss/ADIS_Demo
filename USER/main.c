#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "driver_led.h"
#include "includes.h"
#include "adis16488.h"
#include "interface.h"
#include "dma.h"
/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			6 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			10 
//���������ջ��С
#define LED0_STK_SIZE  		    		128
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);


//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			50 
//���������ջ��С
#define LED1_STK_SIZE  					128
//�����ջ
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

#define SPI_TASK_PRIO       			8 
//���������ջ��С
#define SPI_STK_SIZE  					256
//�����ջ
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//������
void spi_task(void *pdata);
#define SEND_BUF_SIZE 200
u8 SendBuff[SEND_BUF_SIZE]={0};

int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED0_Init();		        //��ʼ��LED�˿� 
	uart_init(921600);
//	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff, SEND_BUF_SIZE);
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
		OSTimeDly(5000);
		LED0=1;
		OSTimeDly(5000);
	};
}
u16 DeviceID=0;
u32 data;
void spi_task(void *pdata)
{
	/* Init Spi */
	IMU_Data_Raw testImu;
	IMU_Data testImuTrue;
	ADISInit();
	/* Do spi transmission */
	while(1)
	{
		/* One time transmission */
		ADIS_Read11AxisData(&testImu);
		ADIS_Raw2Data(&testImuTrue, &testImu);
		printf("%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",\
			OSTime,testImuTrue.gyro[0],testImuTrue.gyro[1],testImuTrue.gyro[2],testImuTrue.accl[0],testImuTrue.accl[1],\
			testImuTrue.accl[2],testImuTrue.magn[0],testImuTrue.magn[1],testImuTrue.magn[2],testImuTrue.baro,testImuTrue.temp);
		OSTimeDly(50);
	}
}



