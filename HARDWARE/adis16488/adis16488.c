#include "adis16488.h"
#include "spi.h"
#include "usart.h"
void ADISInit(void)
{
	// CS Pin
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	// CS = 1
	PBout(6)=1;
//	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	// SPI Pin
	SPI1_Init();
}

void enableADIS(void)
{
	PBout(6)=0;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void disableADIS(void)
{
	PBout(6)=1;
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

void ADIS_Read11AxisData( IMU_Data_Raw *raw )
{
	raw->gyro_out[0]=ADIS_ReadDataOut(X_GYRO_OUT);
	raw->gyro_out[0]=ADIS_ReadDataOut(X_GYRO_LOW);
	raw->gyro_low[0]=ADIS_ReadDataOut(Y_GYRO_OUT);
	raw->gyro_out[1]=ADIS_ReadDataOut(Y_GYRO_LOW);
	raw->gyro_low[1]=ADIS_ReadDataOut(Z_GYRO_OUT);
	raw->gyro_out[2]=ADIS_ReadDataOut(Z_GYRO_LOW);
	
	raw->gyro_low[2]=ADIS_ReadDataOut(X_ACCL_OUT);
	raw->accl_out[0]=ADIS_ReadDataOut(X_ACCL_LOW);
	raw->accl_low[0]=ADIS_ReadDataOut(Y_ACCL_OUT);
	raw->accl_out[1]=ADIS_ReadDataOut(Y_ACCL_LOW);
	raw->accl_low[1]=ADIS_ReadDataOut(Z_ACCL_OUT);
	raw->accl_out[2]=ADIS_ReadDataOut(Z_ACCL_LOW);
	
	raw->accl_low[2]=ADIS_ReadDataOut(X_MAGN_OUT);
	raw->magn_out[0]=ADIS_ReadDataOut(Y_MAGN_OUT);
	raw->magn_out[1]=ADIS_ReadDataOut(Z_MAGN_OUT);
	
	raw->magn_out[2]=ADIS_ReadDataOut(BAROM_OUT);
	raw->baro_out =ADIS_ReadDataOut(BAROM_LOW);
	
	raw->baro_low = ADIS_ReadDataOut(TEMP_OUT);
	
	/* last write 00 into 0x00, in order to get the final data */
	enableADIS();
	raw->temp = ADIS_ReadWriteByte(0x8000);
	disableADIS();
	
//	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.2f\r\n",raw->gyro_out[0],raw->gyro_out[1],raw->gyro_out[2],raw->accl_out[0],raw->accl_out[1],raw->accl_out[2],raw->magn_out[0],raw->magn_out[1],raw->magn_out[2],raw->baro_out,raw->temp*0.00565+25);
}

void ADIS_Raw2Data(IMU_Data *dstData, IMU_Data_Raw *srcRawData)
{
	/* change the raw data to true data*/

	dstData->gyro[0] = srcRawData->gyro_out[0]*0.02+srcRawData->gyro_low[0]*0.02/65536.0;
	dstData->gyro[1] = srcRawData->gyro_out[1]*0.02+srcRawData->gyro_low[1]*0.02/65536.0;
	dstData->gyro[2] = srcRawData->gyro_out[2]*0.02+srcRawData->gyro_low[2]*0.02/65536.0;

	dstData->accl[0] = srcRawData->accl_out[0]*0.8+srcRawData->accl_low[0]*0.8/65536.0;
	dstData->accl[1] = srcRawData->accl_out[1]*0.8+srcRawData->accl_low[1]*0.8/65536.0;
	dstData->accl[2] = srcRawData->accl_out[2]*0.8+srcRawData->accl_low[2]*0.8/65536.0;

	dstData->magn[0] = srcRawData->magn_out[0]*0.1;
	dstData->magn[1] = srcRawData->magn_out[1]*0.1;
	dstData->magn[2] = srcRawData->magn_out[2]*0.1;

	// baro
	dstData->baro = srcRawData->baro_out*40.0+srcRawData->baro_low*20.0/65536.0;
	// temp
	dstData->temp = srcRawData->temp*0.00565+25;
//	printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",dstData->gyro[0],dstData->gyro[1],dstData->gyro[2],dstData->accl[0],dstData->accl[1],dstData->accl[2],dstData->magn[0],dstData->magn[1],dstData->magn[2],dstData->baro,dstData->temp);
}

// end of this file
