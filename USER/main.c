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
// key
#include "key_press.h"


u8 exf_getfree(u8 *drv,u32 *total,u32 *free);

/////////////////////////UCOSII任务设置///////////////////////////////////
/**************************************************/
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			60 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
/**************************************************/               			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			8 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		4096
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);
/**************************************************/
#define SPI_TASK_PRIO       			6 
//设置任务堆栈大小
#define SPI_STK_SIZE  					4096
//任务堆栈
OS_STK SPI_TASK_STK[SPI_STK_SIZE];
//任务函数
void spi_task(void *pdata);
/**************************************************/
#define SDWRITE_TASK_PRIO       			11 
//设置任务堆栈大小
#define SDWRITE_STK_SIZE  					4096
//任务堆栈
OS_STK SDWRITE_TASK_STK[SDWRITE_STK_SIZE];
//任务函数
void sd_write_task(void *pdata);
/**************************************************/
#define GPSSDWRITE_TASK_PRIO       			15
//设置任务堆栈大小
#define GPSSDWRITE_STK_SIZE  				4096
//任务堆栈
OS_STK GPSSDWRITE_TASK_STK[GPSSDWRITE_STK_SIZE];
//任务函数
void gps_sd_write_task(void *pdata);
/**************************************************/
#define KEYSCAN_TASK_PRIO       			7
//设置任务堆栈大小
#define KEYSCAN_STK_SIZE  				256
//任务堆栈
OS_STK KEYSCAN_TASK_STK[KEYSCAN_STK_SIZE];
//任务函数
void keyscan_task(void *pdata);

typedef enum
{
	UNPRESSED,
	PRESSED,
	CLOSED,
	PRESSED_OPEN,
	IMU_USED_OPEN,
	GPS_USED_OPEN,
	OPENED,
	PRESSED_CLOSE,
	IMU_USED_CLOSE,
	GPS_USED_CLOSE
}KEY_STATUS;

KEY_STATUS key_value=CLOSED;
volatile u8 imu_use_flag = 0;
volatile u8 gps_use_flag = 0;


#define SEND_BUF_SIZE 200
u8 SendBuff[SEND_BUF_SIZE]={0};
extern char fifo_buffer[QUEUE_SIZE][MSG_LENGTH];
extern Queue_Info fifo_info;
OS_EVENT * fifo_lock;

int main(void)
{ 
	key_sdWrite();
	delay_init(168);		  //初始化延时函数
	LED0_Init();		        //初始化LED端口 
	uart_init(115200);
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
 	OSTaskCreate(spi_task,(void *)0,(OS_STK*)&SPI_TASK_STK[SPI_STK_SIZE-1],SPI_TASK_PRIO);		
	OSTaskCreate(sd_write_task,(void *)0,(OS_STK*)&SDWRITE_TASK_STK[SDWRITE_STK_SIZE-1],SDWRITE_TASK_PRIO);		
	OSTaskCreate(gps_sd_write_task,(void *)0,(OS_STK*)&GPSSDWRITE_TASK_STK[GPSSDWRITE_STK_SIZE-1],GPSSDWRITE_TASK_PRIO);		
	OSTaskCreate(keyscan_task,(void *)0,(OS_STK*)&KEYSCAN_TASK_STK[KEYSCAN_STK_SIZE-1],KEYSCAN_TASK_PRIO);		

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
u8 key_flag=0;
void spi_task(void *pdata)
{
	/* Init Spi */
	INT8U error;
	u32 currentTime;
	IMU_Data_Raw testImu;
	IMU_Data testImuTrue;
	ADISInit();
	PBout(14)=0;
	/* Do spi transmission */
	while(1)
	{
		/* One time transmission */
		currentTime = OSTime;
		ADIS_Read11AxisData(&testImu);
		ADIS_Raw2Data(&testImuTrue, &testImu);
		/* add the message in a queue tail. */		
		// 1' 字符串 输入到buffer
		if (key_flag)
		{
			sprintf(fifo_buffer[fifo_info.tail],"%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n",\
			currentTime,testImuTrue.gyro[0],testImuTrue.gyro[1],testImuTrue.gyro[2],testImuTrue.accl[0],testImuTrue.accl[1],\
			testImuTrue.accl[2],testImuTrue.magn[0],testImuTrue.magn[1],testImuTrue.magn[2],testImuTrue.baro,testImuTrue.temp);
			// 2.0' 信号量 开始锁住
			OSSemPend(fifo_lock,0,&error);
			// 2' 队列加入一个数
			queue_in(&fifo_info);
			// 3' 信号量开始解锁
			OSSemPost(fifo_lock);
		}
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
	FILINFO t_filinfo;
	//Free and total space
//	uint32_t total, free;	
	int key_val;
	char savename[10];
	FRESULT res;
	u32 currentTime;
	u8 file_index=0;
	SD_Error sdError;
	
	while(1)
	{
		/* using a queue, when queue is not empty always write */
		// TODO: 1' 当按键按下时，开始挂载SD卡
		if (key_value == PRESSED_OPEN || key_value == GPS_USED_OPEN)
		{
//			LED1 = 1;
			// 按键按下，需要IMU进行运算处理
			imu_use_flag = 1;
			OSTimeDly(2000);
			
			// detecte the file is exist or not.
			while(1)
			{
				sprintf(savename,"0:/%03d-IMU.txt",file_index++);
				res = f_stat(savename,&t_filinfo);
				if (res==FR_NO_FILE)
				{
					break;
				}
			}
			// open file
			res = f_open(&fil, savename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
			if (res == FR_OK) 
			{
				LED0=0;
				// FIXME: 文件打开，给一个信号指示
				// 开始写入数据			
				queue_init(&fifo_info);
				key_flag=1; // 用于SPI同步采集数据
				
				while(1)
				{
					/* 进行数据的写入 */
					// 写之前进行 检测，队列是否为空, 若队列为空，则进行5ms等待
					while (queue_is_empty(&fifo_info)){OSTimeDly(20);};
					// 写数据
					currentTime = OSTime;
					f_printf(&fil,"%d---%s",fifo_info.count,fifo_buffer[fifo_info.head]);
					// 0' 信号量 锁定检测，并锁定信号量
					OSSemPend(fifo_lock,0,&error);
					queue_out(&fifo_info);
					OSSemPost(fifo_lock);
					// 9' 信号量解锁
					// 退出写模式检测
					if (key_value == PRESSED_CLOSE || key_value == GPS_USED_CLOSE)
					{
						// 若是按键按下则退出while(1)循环
						imu_use_flag = 1;
						break;
					}
					OSTimeDly(1);
				}
				key_flag=0;
				LED0=1;
				//Close file, don't forget this!
				f_close(&fil);
			}
			OSTimeDly(2000);
		}
		OSTimeDly(200); // 再经过20 ms进行按键检测
	}
}

void gps_sd_write_task(void *pdata)
{
	u16 len = 0;
	u16 last_pack_len = 0;
	u16 t = 0;
	UINT reallen;
	char command[22]="LOG RANGEA ONTIME 1\r\n";
	u8 comCnt = 0;
	/* Init SD card module */
	INT8U error;

	//File object
	FIL fil;
	FILINFO t_filinfo;
	//Free and total space
//	uint32_t total, free;	
	char savename[10];
	FRESULT res;
	u32 currentTime;
	u8 file_index=0;
	SD_Error sdError;	
	
	// 延时10秒，等待GPS初始化完成
//	OSTimeDly(100000);
	for (comCnt = 0; comCnt <22;++comCnt)
	{
		USART_SendData(USART1, command[comCnt]); //向串口 1 发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	
	}
	
	while(1)
	{
		/* using a queue, when queue is not empty always write */
		// TODO: 1' 当按键按下时，开始挂载SD卡
		if (key_value == PRESSED_OPEN || key_value == IMU_USED_OPEN)
		{
			// 按键按下，需要IMU进行运算处理
			gps_use_flag = 1;
			OSTimeDly(2000);
			
			// detecte the file is exist or not.
			while(1)
			{
				sprintf(savename,"0:/%03d-GPS.txt",file_index++);
				res = f_stat(savename,&t_filinfo);
				if (res==FR_NO_FILE)
				{
					break;
				}
			}
			// open file
			res = f_open(&fil, savename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
			if (res == FR_OK) //FR_OK
			{
				LED1 = 1;
				// FIXME: 文件打开，给一个信号指示
				// 开始写入数据				
				while(1)
				{
					OSTimeDly(2000); 
//					// 开始判断是否接收完成
//					// 写GPS数据时每写多少个字节就要OSTimeDly(1)一次
//					if(USART_RX_STA&0x8000)
//					{
//						// GPS接收完成
//						// 读取数据长度
//						len=USART_RX_STA&0x3fff;
//						last_pack_len = len%100;
//						len -= last_pack_len; 
//						for(t=0;t<len;t+=100)
//						{
//							// 写10个字节，目的防止该任务一直占用资源，每写10个字节就Dly0.1ms
//							// 缺点：会增加GPS数据写的时间。
//							f_write(&fil,USART_RX_BUF+t,100,&reallen); 
//							OSTimeDly(1);
//						}
//						f_write(&fil,USART_RX_BUF+t,last_pack_len,&reallen); 
//						USART_RX_STA=0;
//					}
					// 退出写模式检测
					if (key_value == PRESSED_CLOSE || key_value == IMU_USED_CLOSE)
					{// 若是按键按下则退出while(1)循环
						gps_use_flag = 1;
						break;
					}
					OSTimeDly(1);
				}
				LED1 = 0;
				//Close file, don't forget this!
				f_close(&fil);
			}
			OSTimeDly(2000);
		}
		OSTimeDly(200); // 再经过20 ms进行按键检测
	}
}

// key scan task	
FATFS FatFs;

void keyscan_task(void *pdata)
{
	int value,value2;
	SD_Error sdError;
	FRESULT res;
	KEY_STATUS key2 = UNPRESSED;
	while(1)
	{
		value2 = KEY1;
		if (value2 == 0 && key2 == UNPRESSED)
		{
			sdError = SD_Init();
			// 挂载SD卡
			res = f_mount(&FatFs, "0:", 1);
			if (res == FR_OK) 
			{
				// 挂载 OK亮灯
				LED1 = 0;
				key2 = PRESSED;
			}
			OSTimeDly(2000); //延时20ms
		}
		value2 = KEY1;
		if (value2 == 0 && key2 == PRESSED)
		{
			//Unmount drive, don't forget this!
			f_mount(0, "0:", 1);
			// turn off the light
			LED1 = 1;
			key2 = UNPRESSED;
			key_value = CLOSED;
			OSTimeDly(2000); //延时20ms
		}
		
		switch(key_value)
		{
			case CLOSED:
				value = KEY0;
				if (!value) // key is pressed
				{
					key_value = PRESSED_OPEN;
				}
				break;
			case PRESSED_OPEN:
				if (imu_use_flag)
				{
					key_value = IMU_USED_OPEN; //IMU_USED
					imu_use_flag = 0;
					break;
				}
				if (gps_use_flag)
				{
					key_value = GPS_USED_OPEN; //GPS_USED
					gps_use_flag = 0;
					break;
				}
				break;
				
			case IMU_USED_OPEN:
				if (gps_use_flag)
				{
					key_value = OPENED;
					gps_use_flag = 0;
				}
				break;
			case GPS_USED_OPEN:
				if (imu_use_flag)
				{
					key_value = OPENED;
					imu_use_flag = 0;
				}
				break;
			case OPENED:
				value = KEY0;
				if (!value) // key is pressed
				{
					key_value = PRESSED_CLOSE;
				}
				break;
			case PRESSED_CLOSE:
				if (imu_use_flag)
				{
					key_value = IMU_USED_CLOSE; //IMU_USED
					imu_use_flag = 0;
					break;
				}
				if (gps_use_flag)
				{
					key_value = GPS_USED_CLOSE; //GPS_USED
					gps_use_flag = 0;
					break;
				}
				break;
				
			case IMU_USED_CLOSE:
				if (gps_use_flag)
				{
					key_value = CLOSED;
					gps_use_flag = 0;
				}
				break;
			case GPS_USED_CLOSE:
				if (imu_use_flag)
				{
					key_value = CLOSED;
					imu_use_flag = 0;
				}
				break;
			default:
				break;
		}
		OSTimeDly(200); //延时20ms
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


