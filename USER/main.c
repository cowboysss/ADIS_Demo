#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "driver_led.h"
#include "includes.h"
#include "adis16488.h"
#include "interface.h"
#include "dma.h"
// sdio fatfs
#include "sdio_sdcard.h" 
#include "ff.h"  

// queue
#include "queue.h"

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
extern char fifo_buffer[QUEUE_SIZE][MSG_LENGTH];
extern Queue_Info fifo_info;
OS_EVENT * fifo_lock;

int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED0_Init();		        //初始化LED端口 
	uart_init(921600);
	fifo_lock = OSSemCreate(1);
	queue_init(&fifo_info);
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
// 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
// 	OSTaskCreate(spi_task,(void *)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO);		
	OSTaskCreate(sd_write_task,(void *)0,(OS_STK*)&SDWRITE_TASK_STK[LED0_STK_SIZE-1],SDWRITE_TASK_PRIO);		
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
	INT8U error;
	u32 currentTime;
	IMU_Data_Raw testImu;
	IMU_Data testImuTrue;
	ADISInit();
	
	/* Do spi transmission */
	while(1)
	{
		/* One time transmission */
		currentTime = OSTime;
		ADIS_Read11AxisData(&testImu);
		ADIS_Raw2Data(&testImuTrue, &testImu);
		/* add the message in a queue tail. */		
		// 1' 字符串 输入到buffer
		sprintf(fifo_buffer[fifo_info.tail],"%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",\
			currentTime,testImuTrue.gyro[0],testImuTrue.gyro[1],testImuTrue.gyro[2],testImuTrue.accl[0],testImuTrue.accl[1],\
			testImuTrue.accl[2],testImuTrue.magn[0],testImuTrue.magn[1],testImuTrue.magn[2],testImuTrue.baro,testImuTrue.temp);
		// 2.0' 信号量 开始锁住
		OSSemPend(fifo_lock,0,&error);
		// 2' 队列加入一个数
		queue_in(&fifo_info);
		// 3' 信号量开始解锁
		OSSemPost(fifo_lock);
		OSTimeDly(49);
	}
}

void sd_write_task(void *pdata)
{
	/* Init SD card module */
	INT8U error;
	//Fatfs object
	FATFS FatFs;
	//File object
	FIL fil;
	//Free and total space
//	uint32_t total, free;	
	int key_val;
	FRESULT res;
	u32 currentTime;
	
	while(1)
	{
		/* using a queue, when queue is not empty always write */
		// TODO: 1' 当按键按下时，开始挂载SD卡
		switch (key_val)
		{
			case 0: // 按键没有按下
				OSTimeDly(200); // 再经过20 ms进行按键检测
				break;
			case 1: // 按键已经按下了
				// 初始化SD卡
				SD_Init();
				// 挂载SD卡
				res = f_mount(&FatFs, "0:", 1);
				if (res == FR_OK) 
				{
					// FIXME: 挂载 OK， 加入信号提示
					// FIXME: 打开文件(读写模式，写入的时候会覆盖原始的数据)
					// 进行文件的检测，创建一个新的文件名，避免覆盖之前的文件
					res = f_open(&fil, "0:/001.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
					if (res == FR_OK) 
					{
						// FIXME: 文件打开，给一个信号指示
						// 开始写入数据				
						while(1)
						{
							/* 进行数据的写入 */
							// 写之前进行 检测，队列是否为空, 若队列为空，则进行5ms等待
							while (queue_is_empty(&fifo_info)){OSTimeDly(50);};
							// 写数据
							currentTime = OSTime;
							f_printf(&fil,"%d---%s",currentTime,fifo_buffer[fifo_info.head]);
							// 0' 信号量 锁定检测，并锁定信号量
							OSSemPend(fifo_lock,0,&error);
							queue_out(&fifo_info);
							OSSemPost(fifo_lock);
							// 9' 信号量解锁
							// TODO: 进行按键的检测
							if (key_val==1)
							{// 若是按键按下则退出while(1)循环
								break;
							}
							
						}
						//Close file, don't forget this!
						f_close(&fil);
					}
					
					//Unmount drive, don't forget this!
					f_mount(0, "0:", 1);
				}
				break;
			default:
				break;
		}
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


