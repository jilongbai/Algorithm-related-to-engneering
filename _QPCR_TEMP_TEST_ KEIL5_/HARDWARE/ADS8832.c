#include "ADS8832.h"
#include "System_Init.h"
#include "stm32f2xx_spi.h"
#include "main.h"
extern  u16 ADC_Value[20];
extern  u8 Mid_Filt_Win;
void Qpcr_Delay(unsigned int n)
{
   unsigned int i;
   unsigned int j;
   for( i = n; i > 0; i --)
   {
      j=1;
      for(;j>0;j--)
      {
      }
   }
}
/************************************************************************/
//   函数功能:准备ADS8832的SPI设置
//   输入参数:无
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    8b，
/************************************************************************/
void ADS8832_PrepareSpi(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	//APB2总线时钟(PCLK2 = SYSCLK/2) = 60MHz
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //使能SPI1的时钟

    SPI_Cmd(SPI1, DISABLE);
    /*spi CONFIGURATION*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线输入输出全双工模式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //将SPI1设置为主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //设置SPI数据帧格式：有8位和16位，我们此处选择16位的数据帧格式
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   //设置时钟的极性，我们设置串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  //设置时钟相位，也就是选择在串行同步时钟的第几个跳变沿(上升沿或者下降沿)数据被采样
    //可以为第一个或者第二个边沿采集，这里我们选择第一个跳变沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //设置NSS信号是由硬件(NSS管脚)还是软件控制。此处我们选择由软件控件片选
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//SPI时钟的预分频值,从2分频到256分频，此处我们的传输速度=60MHz/SPI_BaudRatePrescaler_8 = 7.5MHz;;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //此处我们设置数据的传输顺序，是MSB位在前还是LSB位在前，此处我们选择MSB高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;     //CRC较验和的多项式
    SPI_Init(SPI1, &SPI_InitStructure);   //初始化SPI1的配置项
    SPI_Cmd(SPI1, ENABLE);      //使能SPI1
    
}
/************************************************************************/
//   函数功能:初始化ADC
//   输入参数:无
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    GPIOG14->EOC
/************************************************************************/
void ADS8832_Init(void)
{
    u16 cfr1, cfr2;
    cfr1 = 0X07FD;//手动控制 0X07FD = 0b 0000 0111 1111 1101 
    //bit[11] = 0; 手动选择通道        bit[10] = 1; Conversion clock (CCLK) = internal OSC
	//bit[9] = 1;  手动触发转换        bit[8] = 1;  该位主要针对于自动触发模式下的采样率的设置

	//bit[7] = 1; EOC/INT active low   bit[6] = 1;  EOC/INT该引脚作为EOC功能
	//bit[5] = 1;                      bit[4] = 1;

	//bit[3] = 0;                      bit[2] = 1; 
	//bit[1] = 0;                      bit[0] = 1; 


	//#define ADC0_CS_H()       GPIO_SetBits(GPIOC, GPIO_Pin_4)
	//将PC4置1，即ADC0未选中
    ADC0_CS_H();
	
	//#define ADC1_CS_H()       GPIO_SetBits(GPIOB, GPIO_Pin_1)
    //将PB1置1，即ADC1未选中
    ADC1_CS_H();
	
	//#define ADC2_CS_H()       GPIO_SetBits(GPIOG, GPIO_Pin_1)
    ADC2_CS_H();

	//#define ADC0_CONV_H()     GPIO_SetBits(GPIOC, GPIO_Pin_5)
	//将PC5置1，即禁止ADC0转换
    ADC0_CONV_H();

	//#define ADC1_CONV_H()     GPIO_SetBits(GPIOB, GPIO_Pin_2)
	//将PB2置1，即禁止ADC1转换
    ADC1_CONV_H();
    ADC2_CONV_H();

	
    //****************************************************
	//使能ADC0
	
    //#define ADC_0    0
    ADS8832_WriteCfr(cfr1,ADC_0);     //在函数内部完成转换
    Qpcr_Delay(10);
    while(1)
    {
        cfr2  = ADS8832_ReadCfr(ADC_0);   //读取配置数据
        Qpcr_Delay(10);
        if(cfr1==cfr2)                    //如果配置成功则推出该循环，如果配置未成功，则重新配置
            break;
        ADS8832_WriteCfr(cfr1,ADC_0);
        Qpcr_Delay(10);
    }

	
    //***************************************************
	//使能ADC1
	
    ADS8832_WriteCfr(cfr1,ADC_1);
    Qpcr_Delay(10);
    cfr2 = 0;
    while(1)
    {
        cfr2  = ADS8832_ReadCfr(ADC_1);
        Qpcr_Delay(10);
        if(cfr1==cfr2)
            break;
        ADS8832_WriteCfr(cfr1,ADC_1);
        Qpcr_Delay(10);
    }

	
    //***************************************************
//    ADS8832_WriteCfr(cfr1,ADC_2);
//    Qpcr_Delay(10);
//    while(1)
//    {
//        cfr2  = ADS8832_ReadCfr(ADC_2);
//        Qpcr_Delay(10);
//        if(cfr1==cfr2)
//            break;
//        ADS8832_WriteCfr(cfr1,ADC_2);
//        Qpcr_Delay(10);
//    }
}


/************************************************************************/
//   函数功能:ADS8832开始转换
//   输入参数:无
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    Convst下降沿开始转换
/************************************************************************/
void ADS8832_Convst(u8 ADC_x)
{
    if(ADC_x ==0)
    {
        ADC0_CONV_L();
        Qpcr_Delay(15);	
        ADC0_CONV_H();
    }
    else if(ADC_x ==1)
    {
        ADC1_CONV_L();
        Qpcr_Delay(15);	
        ADC1_CONV_H();
    }
    else
    {
        ADC2_CONV_L();
        Qpcr_Delay(15);	
        ADC2_CONV_H();
    }
}

/************************************************************************/
//   函数功能:读取ADS8832配置数据
//   输入参数:无
//   输出参数:u16, ADS8832配置数据
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
u16 ADS8832_ReadCfr(u8 ADC_x)
{
	//不管是写入配置还是读取配置，首先第一步肯定是需要将对应的ADC芯片选中，低电平有效
    switch(ADC_x)
    {
        case 0: ADC0_CS_L();
        break;
        case 1: ADC1_CS_L();
        break;
        case 2: ADC2_CS_L();
        break;
        default:
        break;
    }
    Qpcr_Delay(10);

	//SPI_I2S_GetFlagStatus()Checks whether the specified SPIx/I2Sx flag is set or not.
	//SPI_I2S_FLAG_RXNE: Receive buffer not empty flag.
    if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET )
    {
        SPI_I2S_ReceiveData(SPI1);   //SPI_I2S_ReceiveData():Returns the most recent received data by the SPIx/I2Sx peripheral.
    }
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
    SPI_I2S_SendData(SPI1, ADS8332_READ_CFR); 
    Qpcr_Delay(2);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET );	
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
    Qpcr_Delay(2);
    switch(ADC_x)
    {
        case 0: ADC0_CS_H();
        break;
        case 1: ADC1_CS_H();
        break;
        case 2: ADC2_CS_H();
        break;
        default:
        break;
    } 
    SPI_I2S_ReceiveData(SPI1);
    return (SPI_I2S_ReceiveData(SPI1)&0xfff);
}

/************************************************************************/
//   函数功能:写入ADS8832配置数据
//   输入参数:u16, ADS8832配置数据
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
void ADS8832_WriteCfr(u16 CfrData,u8 ADC_x)
{
    switch(ADC_x)
    {
        case 0: ADC0_CS_L();
        break;
        case 1: ADC1_CS_L();
        break;
        case 2: ADC2_CS_L();
        break;
        default:
        break;
    }
    Qpcr_Delay(10);

	
	//**************************************************************

	//下面几条语句主要是:首先一直等到SPI的buffer为空为止；然后发送数据；等到数据发送完成，buffer为空，则将片选信号拉高，禁能ADC
	//SPI_I2S_GetFlagStatus()Checks whether the specified SPIx/I2Sx flag is set or not.
	//SPI_I2S_FLAG_TXE: Transmit buffer empty flag.
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (ADS8332_WRITE_CFR | (0x0fff&CfrData)));
    Qpcr_Delay(2);
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);	
    Qpcr_Delay(2);
	//**************************************************************


	//将片选信号拉高，禁能ADC
    switch(ADC_x)
    {
        case 0: ADC0_CS_H();
        break;
        case 1: ADC1_CS_H();
        break;
        case 2: ADC2_CS_H();
        break;
        default:
        break;
    }
}

/************************************************************************/
//   函数功能:设置ADS8832采样通道
//   输入参数:u8, 通道值 0-7
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
void ADS8832_SetChannel(u8 ChannelNum)
{
    
    u16 ChnSelCmd;
    if(ChannelNum<8)
    {
        ADC0_CS_L();
    }
    else if(ChannelNum<16)
    {
        ADC1_CS_L();
        ChannelNum -= 8;
    }
    else
    {
        ADC2_CS_L();
        ChannelNum -= 16;
    }
    ChnSelCmd = (ChannelNum<<12);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, ChnSelCmd);
    Qpcr_Delay(100);
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
    if(ChannelNum<8)
    {
        ADC0_CS_H();
    }
    else if(ChannelNum<16)
    {
        ADC1_CS_H();
    }
    else
    {
        ADC2_CS_H();
    }
}

/************************************************************************/
//   函数功能:连续读取ADS8832的16个通道的采样结果
//   输入参数:u8, 通道值 0-15
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
void ADS8832_16Channel_Sample(void)
{
    u16 ChnSelCmd = 0;
    u8  i = 0; 
   
    for(i = 0; i < 8; ++i)
    {         
       //******************发送切换通道命令同时接收上次转换数据**********
        ADC0_CS_H();    
        ADC0_CS_L(); 
        ChnSelCmd = (i<<12);	//每次i++实现通道切换
        SPI_I2S_SendData(SPI1, ChnSelCmd); 
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
        ADC0_CS_H();
        SPI_I2S_ReceiveData(SPI1);
        //******************开始转换***********************
        Qpcr_Delay(3000); //采样保持时间 时间太低会导致采样电容充不满
        ADS8832_Convst(ADC_0); 
        
        //*****************************************
        ADC0_CS_L();      
        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
        ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);
        ADC0_CS_H();
		//每次采集完成后进行软件滤波
        ADC_Value[i] = Mid_Data_Filter(ADC_Value[i] , Mid_Filt_Win , i);//Mid_Data_Num 为奇数
        //Qpcr_Delay(10);
    }
   for(i = 0; i < 8; ++i)
    {         
       //******************发送切换通道命令同时接收上次转换数据**********
        ADC1_CS_H();    
        ADC1_CS_L(); 
        ChnSelCmd = (i<<12);	//(8<<12);// 
        SPI_I2S_SendData(SPI1, ChnSelCmd); 
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
        ADC1_CS_H();
        SPI_I2S_ReceiveData(SPI1);
        //******************开始转换***********************
        Qpcr_Delay(3000); //采样保持时间 时间太低会导致采样电容充不满
        ADS8832_Convst(ADC_1); 
        //*****************************************
        ADC1_CS_L();      
        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
        ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);
        ADC1_CS_H();
        ADC_Value[i+8] = Mid_Data_Filter(ADC_Value[i+8] , Mid_Filt_Win , i+8);//Mid_Data_Num 为奇数
      //  Qpcr_Delay(10);
    }
}

void ADS8832_16Channel_10TimesSample(void)
{
//    u16 ChnSelCmd = 0;
//    u8 j, i = 0;
//    u8 k = 0;
//    u16 Max_Data = 0;
//    u16 Min_Data = 65535;
//    u32 Sum_Data = 0;
//    u16 Temp[15] = 0;
//    for(i = 0; i < 8; ++i)
//    {         
//       //******************发送切换通道命令同时接收上次转换数据**********
//        ADC0_CS_H();    
//        ADC0_CS_L(); 
//        ChnSelCmd = (i<<12);	//0;// 
//        SPI_I2S_SendData(SPI1, ChnSelCmd); 
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET); 
//        Sum_Data = 0;
//        Max_Data = 0;
//        Min_Data = 65535;
//        ADC0_CS_H();
//        Qpcr_Delay(150); //采样保持时间 时间太低会导致采样电容充不满
//       // for(j = 0; j < 10; ++j)
//        {
//
//          SPI_I2S_ReceiveData(SPI1);
//          //******************开始转换***********************
//          ADS8832_Convst(ADC_0); 
//          //*****************************************
//          ADC0_CS_L();      
//          SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//          while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//          ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);//Temp[j] = SPI_I2S_ReceiveData(SPI1);//ADC_Value[i] = SPI_I2S_ReceiveData(SPI1);
////          Sum_Data += Temp[j];
////          if(Max_Data<Temp[j])
////          {
////            Max_Data = Temp[j];
////          }
////          if(Min_Data>Temp[j])
////          {
////            Min_Data = Temp[j];
////          }
//          ADC0_CS_H();
//         // Qpcr_Delay(10);
//        }
////        Sum_Data -= Max_Data;
////        Sum_Data -= Min_Data;
////        ADC_Value[i] = Sum_Data>>3;
//    }
//   for(i = 0; i < 8; ++i)
//    {         
//       //******************发送切换通道命令同时接收上次转换数据**********
//        ADC1_CS_H();    
//        ADC1_CS_L(); 
//        ChnSelCmd = (i<<12);	//(8<<12);// 
//        SPI_I2S_SendData(SPI1, ChnSelCmd); 
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);  
//        Sum_Data = 0;
//        Max_Data = 0;
//        Min_Data = 65535;
//        ADC1_CS_H();
//        Qpcr_Delay(150); //采样保持时间 时间太低会导致采样电容充不满
//    //     for(j = 0; j < 10; ++j)
//         {
//
//           SPI_I2S_ReceiveData(SPI1);
//           //******************开始转换***********************
//           ADS8832_Convst(ADC_1); 
//           //*****************************************
//           ADC1_CS_L();      
//           SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//           while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//          ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);// Temp[j] = SPI_I2S_ReceiveData(SPI1);//  ADC_Value[i+8] = SPI_I2S_ReceiveData(SPI1);
////           Sum_Data += Temp[j];
////           if(Max_Data<Temp[j])
////           {
////             Max_Data = Temp[j];
////           }
////           if(Min_Data>Temp[j])
////           {
////             Min_Data = Temp[j];
////           }
//           ADC1_CS_H();
//         //  Qpcr_Delay(10);
//         }
////         Sum_Data -= Max_Data;
////        Sum_Data -= Min_Data;
////        ADC_Value[i + 8] = Sum_Data>>3;
//    }
}
/************************************************************************/
//   函数功能:复位ADS8832
//   输入参数:无
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    Reset低电平复位ADC
/************************************************************************/
//void ADS8832_Reset(void)
//{
//}

/************************************************************************/
//   函数功能:读取ADS8832指定通道的采样结果
//   输入参数:u8, 通道值 0-7
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
//u16 ADS8832_TakeSample(void)
//{
//	u16 res=0;
//
//        Qpcr_Delay(2);
//        ADS8832_Convst(); 
//        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)==Bit_RESET);		// wait for EOC high
//       ADC1_CS_L();
//       if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET )
//        {
//            SPI_I2S_ReceiveData(SPI1);
//        }
//        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);    
//        SPI_I2S_SendData(SPI1, ADS8332_READ_DATA);
//        Qpcr_Delay(2);
//        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//        while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
//        res = SPI_I2S_ReceiveData(SPI1);
//	ADC1_CS_H();
//	return  res;
//}

/************************************************************************/
//   函数功能:配置ADS8832连续采样
//   输入参数:无
//   输出参数:无
//	 全局变量:无 
//   作者:    yufangmin
//   修改日期:2015/12/01
//   备注:    
/************************************************************************/
//void ADS8832_Cont_Sample(void)
//{
//	ADS8832_WriteCfr(ADS8332_CH_AUTO|ADS8332_INT_CLK|ADS8332_SMPL_250K|ADS8332_EOC_LOW|ADS8332_P10_EOC|ADS8332_P10_OUT|ADS8332_AUTONAP_DIS|ADS8332_NAP_DIS|ADS8332_PD_DIS|ADS8332_NOSW_RST);//|ADS8332_TAG_OUT
//}
