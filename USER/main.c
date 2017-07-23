#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "driver_led.h"
#include "includes.h"
#include "adis16488.h"
#include "interface.h"
#include "dma.h"
#include "sdio_sdcard.h" 
#include "ff.h"  

u8 exf_getfree(u8 *drv,u32 *total,u32 *free);

/////////////////////////UCOSII任务设置///////////////////////////////////
/**************************************************/
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			6 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
/**************************************************/               			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			10 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		128
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);
/**************************************************/
#define SPI_TASK_PRIO       			8 
//设置任务堆栈大小
#define SPI_STK_SIZE  					256
//任务堆栈
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//任务函数
void spi_task(void *pdata);
/**************************************************/
#define SDWRITE_TASK_PRIO       			40 
//设置任务堆栈大小
#define SDWRITE_STK_SIZE  					256
//任务堆栈
OS_STK SDWRITE_TASK_STK[SDWRITE_STK_SIZE];
//任务函数
void sd_write_task(void *pdata);

#define SEND_BUF_SIZE 200
u8 SendBuff[SEND_BUF_SIZE]={0};

int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED0_Init();		        //初始化LED端口 
	uart_init(921600);
//	MYDMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff, SEND_BUF_SIZE);
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
		/* FIXME: change print to a peoceduree, add the message in a queue tail. */
		printf("%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",\
			OSTime,testImuTrue.gyro[0],testImuTrue.gyro[1],testImuTrue.gyro[2],testImuTrue.accl[0],testImuTrue.accl[1],\
			testImuTrue.accl[2],testImuTrue.magn[0],testImuTrue.magn[1],testImuTrue.magn[2],testImuTrue.baro,testImuTrue.temp);
		OSTimeDly(49);
	}
}

void sd_write_task(void *pdata)
{
	/* Init SD card module */
	//Fatfs object
	FATFS FatFs;
	//File object
	FIL fil;
	//Free and total space
	uint32_t total, free;
	SD_Init();
	//Mount drive
	if (f_mount(&FatFs, "0:", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		
		//Try to open file
		if (f_open(&fil, "0:/1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) 
		{
			//File opened, turn off RED and turn on GREEN led
			
			//If we put more than 0 characters (everything OK)
			if (f_puts("First string in my file\n", &fil) > 0) 
			{
				if (exf_getfree((u8 *)"0",&total, &free) == FR_OK) 
				{
					//Data for drive size are valid
				}
				//Turn on both leds
			}
			
			//Close file, don't forget this!
			f_close(&fil);
		}
		
		//Unmount drive, don't forget this!
		f_mount(0, "", 1);
	}
	while(1)
	{
		/* using a queue, when queue is not empty always write */
		
	}
}


u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	


