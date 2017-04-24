#include "adis16488.h"
#include "spi.h"
#include "usart.h"
void ADISInit(void)
{
	// CS Pin
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	// CS = 1
	PBout(14)=1;
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	// SPI Pin
	SPI1_Init();
}

void enableADIS(void)
{
	PBout(14)=0;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void disableADIS(void)
{
	PBout(14)=1;
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

u16 ADISRead(u8 addr)
{
	u16 dataSend=0;
	u16 dataReceieve=0;
	dataSend = addr<<8;
	enableADIS();
	dataReceieve = ADIS_ReadWriteByte(dataSend);
	disableADIS();
  return dataReceieve;
}


u16 getID(void)
{
	u16 id=0;
	id = ADISRead(PROD_ID);
	return id;
}

u16 ADIS_ReadWriteByte(u16 TxData)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	 		    
}

//u32 ADIS_ReadData(void)
//{
//	u16 recevData=0;
//	u32 tmpdata=0;
//	recevData = ADISRead(Z_ACCL_OUT);
//	tmpdata = recevData<<16;
//	recevData = ADISRead(Z_ACCL_LOW);
//	tmpdata += recevData;
//	return tmpdata;
//}

int16_t ADIS_ReadDataOut(u16 addr)
{
	u16 data=0;
	data = ADISRead(addr);
	return complement2Tureform(data);
}


int16_t complement2Tureform(u16 code)
{
	int16_t result;
	if (code <= 0x7fff)
	{
		// positive number
		result = code;
	}
	else
	{
		//nagetive number
		result = code - 65536;
	}
	return result;
}

void ADIS_Read9AxisData(void)
{
	int16_t gyro[3]={0}; // 0-x 1-y 2-z
	int16_t accl[3]={0};
	int16_t magn[3]={0};
	int16_t baro=0,temp=0;
	gyro[0]=ADIS_ReadDataOut(X_GYRO_OUT);
	gyro[0]=ADIS_ReadDataOut(Y_GYRO_OUT);
	gyro[1]=ADIS_ReadDataOut(Z_GYRO_OUT);

	gyro[2]=ADIS_ReadDataOut(X_ACCL_OUT);
	accl[0]=ADIS_ReadDataOut(Y_ACCL_OUT);
	accl[1]=ADIS_ReadDataOut(Z_ACCL_OUT);
	
	accl[2]=ADIS_ReadDataOut(X_MAGN_OUT);
	magn[0]=ADIS_ReadDataOut(Y_MAGN_OUT);
	magn[1]=ADIS_ReadDataOut(Z_MAGN_OUT);
	
	magn[2]=ADIS_ReadDataOut(BAROM_OUT);
	
	baro = ADIS_ReadDataOut(TEMP_OUT);
	
	// 
	enableADIS();
	temp = ADIS_ReadWriteByte(0x8000);
	disableADIS();
	
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f\r\n",gyro[0],gyro[1],gyro[2],accl[0],accl[1],accl[2],magn[0],magn[1],magn[2],baro,temp*0.00565+25);
}

// end of this file
