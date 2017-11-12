#include "main.h"
#include "System_Init.h"
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)
extern u8 USART_Tx[BUF_SIZE];
extern u8 USART_Tx2[BUF_SIZE];
extern u16 ADCValue;
void Delay(int i)
{
    while(i--)
    {
        
    }
}
void Sys_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
                           RCC_AHB1Periph_GPIOG , ENABLE);

	/*
		1，NOTE:PA2-->USART2_TX; PA3-->USART2_RX   更改为
		   NOTE:PA9-->USART1_TX; PA10-->USART1_RX
		
		2，NOTE:PB10-->USART3_TX; PB11-->USART3_RX	 更改为
		   NOTE:PD5-->USART2_TX; PD6-->USART2_RX
	*/

    // Configure PA ************************************************//
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC:PA0/PA4/PA5
	//5V的欠压检测引脚元由原来的PA0更改为PF3,即由ADC中的通道0改为通道9 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN ;      //配置为模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);     //引脚复用器映射   PA6-->SPI1-->MISO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);     //引脚复用器映射   PA7-->SPI1-->MOSI
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG_FS);

		//不能同时将PA2/PA3， PD5/PD6复用为USART2， 否则USART2的接受中断无法进入中断 
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  
		//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 


    //PA2-->USART2_TX; PA3-->USART2_RX 更改为  PA9-->USART1_TX; PA10-->USART1_RX
    //USART1主要是用来和串口屏进行通讯
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 

	
    // Configure PB ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//| GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);     //引脚复用器映射   PB3-->SPI1-->SCK
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);     //引脚复用器映射   PB6-->I2C1_SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);     //引脚复用器映射   PB7-->I2C1_SDA
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);   
    // Configure PC ************************************************//  
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SDIO);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SDIO);  

	
    // Configure PD ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_SDIO);

		//PB10-->USART3_TX; PB11-->USART3_RX更改为PD5-->USART2_TX; PD6-->USART2_RX
		//USART2主要是用来和PC进行通讯
		//不能同时将PA2/PA3， PD5/PD6复用为USART2， 如果同时复用，则会造成USART2的接受中断无法进入
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  
		//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);    
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2); 
 
    // Configure PE ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    // Configure PF ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

	//5V的欠压检测引脚元由原来的PA0更改为PF3,即由ADC中的通道0改为通道9 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN ;      //配置为模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOF, &GPIO_InitStructure); 

	
    // Configure PG ************************************************//  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStructure); 
}


void Sys_TIMx_Init(void)
{
   /* TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Prescaler = (120000000/TIM_CLK_FREQ)-1;//对时钟4分频	
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;//BOOT_FREQ - 1;//
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure);
    TIM_ARRPreloadConfig(TIM8, ENABLE);//计数器预装载使能
    
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low;//
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//OC = 1 WHEN MOE = 0//TIM_OCIdleState_Reset;//
    TIM_OCInitStructure.TIM_Pulse = 20;//占空比
    
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//比较值预装载使能
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//比较值预装载使能
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);//比较值预装载使能
    
    TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Disable);
    TIM_CCxCmd(TIM8, TIM_Channel_1, TIM_CCx_Disable);
    TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Disable);
    
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
    
    DMA_InitTypeDef DMA_InitStructure;
    
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(TIM8->ARR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)addr;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);
    
    TIM_DMACmd(TIM8, TIM_DMA_CC1, ENABLE);//UPDATE 作为触	源
    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);	
    */
}
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructure;

	// TIM3是挂载在APB1之下，所以我们通过APB1总线下的使能函数来使能TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    /* Time base configuration */
	//TIM_TimeBaseStructInit():Fills each TIM_TimeBaseInitStruct member with its default value.
	//在使用这个外设置前将TIM3设置位默认值，这样做是为了实现程序的健壮性
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	//50KHz/1000 = 50Hz
    TIM_TimeBaseStructure.TIM_Period = 1000-1;//100hz  //受串口传输速度影响波特率在115200时最高发送频率是50Hz每秒，故采样频率最高50Hz；
    TIM_TimeBaseStructure.TIM_Prescaler = 600 -1;//APB1即PLCK1的时钟频率为30MHz，30MHz/600 = 50KHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //对APB1总线时钟PLCK1不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数，即从0开始到计数值
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// TIM_ARRPreloadConfig():Enables or disables TIMx peripheral Preload register on ARR
    TIM_ARRPreloadConfig(TIM3, ENABLE);//计数器预装载使能

	//Clears the TIMx's interrupt pending bits.
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	//Enables or disables the specified TIM interrupts.
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
}
void Sys_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  

//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
//    NVIC_Init(&NVIC_InitStructure); 
    
    /* Enable the USARTx Interrupt */
	
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}
//******************************************USART*****************************//

void USART1_Init(void)       //USART1主要是和触摸屏进行交互通信
{
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);      //DMA时钟使能
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
	//开启串口的DMA发送功能
    USART_DMACmd(USART1, USART_DMAReq_Tx , ENABLE);     //USART_DMAReq_Tx:USART DMA transmit request
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);      //USART1使能的是接收和发送中断
        
    DMA_StructInit(&DMA_InitStructure);   //Fills each DMA_InitStruct member with its default value
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART1->DR));  //外设基地址，用来设置DMA传输的外设基地址
    //此处我们要进行串口的DMA传输，故外设基地址为串口接收发送的数据存储器USART2->DR的地址，表示为&(USART2->DR)
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx2;  //内存基地址。也就是我们存放DMA数据的内存地址        
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向，此处我们设置位从内存读取到数据然后发送到串口，故外设自然就是目的地了
    DMA_InitStructure.DMA_BufferSize = 15;  //设置一次传输数据量的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 设置传输数据的时候外设地址是不变还是递增。此处我们一直往固定外设地址&(USART2->DR)发送数据，故地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //传输数据时候内存地址是否递增。我们是将内存中连续存储单元的数据发送到串口，故我们的内存地址是需要递增的
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设的数据长度是字节传输(8bits),半字传输(16bits)还是字传输(32bits)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //设置内存的数据长度，我们此处肯定和外设的数据长度一样，串口是8位字节传输，故值设置为DMA_PeripheralDataSize_Byte
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   //DMA模式是否循环采集，我们设置此参数采集完成之后不循环，故设置为DMA_Mode_Normal
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // 设置DMA通道的优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  //是否开启FIFO模式，此处我们选择不开启
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;  //FIFO阈值设置
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   //配置存储器突发传输配置
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); // 初始化DMA Stream
    
    
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);   //使能DMA2_Stream7 interrupts.
    DMA_Cmd(DMA2_Stream7, ENABLE);    //使能DMA1_Stream6，启动传输 
 //   DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
    USART_Cmd(USART1, ENABLE);   
}

void USART2_Init(void)   //USART2主要是和PC进行交互通信
{
		DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);         //USART2使能的是接收中断
        
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART2->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);

    //DMA_IT_TC:  Transfer complete interrupt mask
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);   //使能DMA1 Stream3 interrupts.
    DMA_Cmd(DMA1_Stream6, ENABLE);
        
    USART_Cmd(USART2, ENABLE);


	
//    DMA_InitTypeDef DMA_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);      //DMA时钟使能
//    
//    USART_InitStructure.USART_BaudRate = 115200;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(USART2, &USART_InitStructure);
//	//开启串口的DMA发送功能
//    USART_DMACmd(USART2, USART_DMAReq_Tx , ENABLE);     //USART_DMAReq_Tx:USART DMA transmit request
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);      //USART2使能的是接收和发送中断
//        
//    DMA_StructInit(&DMA_InitStructure);   //Fills each DMA_InitStruct member with its default value
//    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   //通道选择
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART2->DR));  //外设基地址，用来设置DMA传输的外设基地址
//    //此处我们要进行串口的DMA传输，故外设基地址为串口接收发送的数据存储器USART2->DR的地址，表示为&(USART2->DR)
//    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx2;  //内存基地址。也就是我们存放DMA数据的内存地址        
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; //数据传输方向，此处我们设置位从内存读取到数据然后发送到串口，故外设自然就是目的地了
//    DMA_InitStructure.DMA_BufferSize = 15;  //设置一次传输数据量的大小
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 设置传输数据的时候外设地址是不变还是递增。此处我们一直往固定外设地址&(USART2->DR)发送数据，故地址不递增
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //传输数据时候内存地址是否递增。我们是将内存中连续存储单元的数据发送到串口，故我们的内存地址是需要递增的
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设的数据长度是字节传输(8bits),半字传输(16bits)还是字传输(32bits)
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //设置内存的数据长度，我们此处肯定和外设的数据长度一样，串口是8位字节传输，故值设置为DMA_PeripheralDataSize_Byte
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   //DMA模式是否循环采集，我们设置此参数采集完成之后不循环，故设置为DMA_Mode_Normal
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  // 设置DMA通道的优先级
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  //是否开启FIFO模式，此处我们选择不开启
//    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;  //FIFO阈值设置
//    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   //配置存储器突发传输配置
//    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //
//    DMA_Init(DMA1_Stream6, &DMA_InitStructure); // 初始化DMA Stream
//    
//    
// //   DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
//    DMA_Cmd(DMA1_Stream6, ENABLE);    //使能DMA1_Stream6，启动传输 
// //   DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
//    USART_Cmd(USART2, ENABLE);
  
}


void USART1_Tx(u16 Data_count)
{    
    DMA_Cmd(DMA2_Stream7, DISABLE);       //禁能DMA2_Stream7传输功能

	//前面已经禁能DMA功能，故后面为了确认确实是禁能了，才能进行下一步的动作，保证程序的健壮性
    if (DMA_GetCmdStatus(DMA2_Stream7) == DISABLE)   //Returns the status of EN bit for the specified DMAy Streamx.
    {
		//DMA_GetFlagStatus()该函数的功能是:查询DMA传输通道的状态
        if (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == SET)  //DMA_FLAG_TCIFx:  Streamx transfer complete flag
        {
            DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
        }
		//DMA_SetCurrDataCounter该函数功能:Enables or disables the specified DMAy Streamx.
		//DMA_SetCurrDataCounter(重新设定每次DMA传输的数据)
		//设置对应的DMA数据流传输的数据量大小
        DMA_SetCurrDataCounter(DMA2_Stream7, Data_count);     //Writes the number of data units to be transferred on the DMAy Streamx.
        DMA_Cmd(DMA2_Stream7, ENABLE);   //使能DMA1_Stream6传输功能
    } 
}

void USART2_Tx(u16 Data_count)
{
    if (DMA_GetCmdStatus(DMA1_Stream6) == DISABLE)
    {
        if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) == SET)
        {
            DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
        }
        DMA_SetCurrDataCounter(DMA1_Stream6, Data_count);
        DMA_Cmd(DMA1_Stream6, ENABLE);
    } 
}

void USART3_Init(void)   //USART3主要是和PC进行交互通信
{
    DMA_InitTypeDef DMA_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_DMACmd(USART3, USART_DMAReq_Tx , ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //USART3使能的是接收中断
        
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)USART_Tx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold	= DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);

    //DMA_IT_TC:  Transfer complete interrupt mask
    DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);   //使能DMA1 Stream3 interrupts.
    DMA_Cmd(DMA1_Stream3, ENABLE);
        
    USART_Cmd(USART3, ENABLE);
  
}

//void USART3_Tx(u16 Data_count)
//{
//    if (DMA_GetCmdStatus(DMA1_Stream3) == DISABLE)
//    {
//        if (DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3) == SET)
//        {
//            DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
//        }
//        DMA_SetCurrDataCounter(DMA1_Stream3, Data_count);
//        DMA_Cmd(DMA1_Stream3, ENABLE);
//    } 
//}

void Sys_SPI1_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	/* 使能gpio时钟和SPI2 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//SPI_SSOutputCmd(SPI2,ENABLE); 
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线输入输出全双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置为SPI的主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; //SPI数据大小：发送16位帧数据结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   //设备空闲状态时同步时钟SCK的状态，High表示高电平，Low表示低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  //时钟相位，1表示在同步时钟SCK的奇数沿边采样，2表示偶数沿边采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;     //NSS由软件控件片选
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//时钟的预分频值,15Mb
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //MSB高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;     //CRC较验和的多项式
	SPI_Init(SPI1, &SPI_InitStructure);   //初始化SPI2的配置项
	SPI_Cmd(SPI1, ENABLE);      //使能SPI2
}
void Sys_ADC_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;


  //PA3和PF3不同步
//  DMA_InitTypeDef       DMA_InitStructure;

  //PF3-->ADC3-->DMA2_Stream0     PA3-->ADC2-->DMA2_Stream3
  /* Enable peripheral clocks *************************************************/
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //开启DMA2的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 | RCC_APB2Periph_ADC2, ENABLE); //开启ADC2和ADC3的时钟

//  /* DMA2_Stream3 channel0 configuration **************************************/
//  DMA_DeInit(DMA2_Stream0);
//  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCValue;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//  DMA_InitStructure.DMA_BufferSize = 1;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
//  /* DMA2_Stream0 enable */
//  DMA_Cmd(DMA2_Stream0, ENABLE);

  //通用控制参数设置，针对于三个ADC进行通用设置
  //ADC_CommonInitTypeDef通用初始化结构体。ADC_CommonInitTypeDef结构体内容决定三个ADC共用的工作环境，比如模式选择、ADC时钟等等。
  //转换时间 = 采样时间 + 固定周期的时间
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式，即一个ADC进行AD转换，转换速率慢
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; //预分频,ADC时钟是有PCLK2分频而来，分频系数决定ADC时钟频率，可选的分频系数为2、4、6和8。ADC最大时钟配置为36MHz
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //DMA禁止
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两个采样阶段的延迟周期数
  ADC_CommonInit(&ADC_CommonInitStructure);


//  //单独针对于ADC1进行配置
//  /* ADC1 Init ****************************************************************/
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //设置ADC转换分辨率
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //我们在此处使用单词转换，不打开扫描模式
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//连续转换，此处应该用DISABLE
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //我们使用软件触发,所以禁能外部通道的触发
//  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //因为我们选择的是软件触发，所以此处我们不用进行配置
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //设置数据对齐方式
//  //假如设置ADC精度为12位，如果设置数据为左对齐，那AD转换完成数据存放在ADC_DR寄存器的[4:15]位内；如果为右对齐，则存放在ADC_DR寄存器的[0:11]位内。
//  
//  ADC_InitStructure.ADC_NbrOfConversion = 1; //用来设置规则序列的长度，这里我们是单次转换，所以我们的值设置为1 即可
//  ADC_Init(ADC1, &ADC_InitStructure);
//
//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC1, ENABLE);


  //单独针对于ADC2进行配置
  /* ADC2 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //设置ADC转换分辨率
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //我们在此处使用单次转换，不打开扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//连续转换，此处应该用DISABLE
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //我们使用软件触发,所以禁能外部通道的触发
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //因为我们选择的是软件触发，所以此处我们不用进行配置
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //设置数据对齐方式
  //假如设置ADC精度为12位，如果设置数据为左对齐，那AD转换完成数据存放在ADC_DR寄存器的[4:15]位内；如果为右对齐，则存放在ADC_DR寄存器的[0:11]位内。 
  ADC_InitStructure.ADC_NbrOfConversion = 1; //用来设置规则序列的长度，这里我们是单次转换，所以我们的值设置为1 即可
  ADC_Init(ADC2, &ADC_InitStructure);

//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC2, ENABLE);



  //单独针对于ADC3进行配置
  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //设置ADC转换分辨率
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; //我们在此处使用单词转换，不打开扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;// ENABLE;//连续转换，此处应该用DISABLE
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //我们使用软件触发,所以禁能外部通道的触发
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1; //因为我们选择的是软件触发，所以此处我们不用进行配置
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //设置数据对齐方式
  //假如设置ADC精度为12位，如果设置数据为左对齐，那AD转换完成数据存放在ADC_DR寄存器的[4:15]位内；如果为右对齐，则存放在ADC_DR寄存器的[0:11]位内。
  
  ADC_InitStructure.ADC_NbrOfConversion = 1; //用来设置规则序列的长度，这里我们是单次转换，所以我们的值设置为1 即可
  ADC_Init(ADC3, &ADC_InitStructure);

//  /* Enable ADC1 DMA */
//  ADC_DMACmd(ADC3, ENABLE);
  
  /* ADC1 regular channel18 (VBAT) configuration ******************************/
  //用来绑定ADC通道转换顺序和时间
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_480Cycles);
  
//  /* Enable DMA request after last transfer (Single-ADC mode) */
//  ADC_DMARequestAfterLastTransferCmd(ADC1 | ADC2 | ADC3, ENABLE);

  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADC2, ENABLE);  //开启AD转换器
  ADC_Cmd(ADC3, ENABLE);  //开启AD转换器

}



void Sys_DAC_Init(void)
{

    DAC_InitTypeDef DAC_InitStructure;
    /* DAC channel2 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software; 
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_2, ENABLE); 
    
    DAC_Init(DAC_Channel_1, &DAC_InitStructure); 
    DAC_Cmd(DAC_Channel_1, ENABLE); 
}


void SET_DAC_VALUE(float DAC1_VALUE,float DAC2_VALUE)
{
    u16 val1,val2;
    val1 = (u16)((DAC1_VALUE * 4096)/ 3); 
    val2 = (u16)((DAC2_VALUE * 4096)/ 3); 
    
    DAC_SetChannel1Data(DAC_Align_12b_R, val1);    
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);	
    
    DAC_SetChannel2Data(DAC_Align_12b_R, val2); 
    DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);	  
}


void* INT_To_ASCII(u16 data,char* lptr)
{
     u16 Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u16 Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
  
     Int_Data4 = data/10000;
     Res_Data4 = data%10000;
     
     Int_Data3 = Res_Data4/1000;
     Res_Data3 = Res_Data4%1000;
     
     Int_Data2 = Res_Data3/100;
     Res_Data2 = Res_Data3%100;
     
     Int_Data1 = Res_Data2/10;
     Res_Data1 = Res_Data2%10;
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data4+48;
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
     else if(Int_Data3>0)
     {
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
     {
        *lptr++ = Int_Data2+48;
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)
     {
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
        *lptr++ = Res_Data1+48;  
     }
     else
     {
        *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
void* TwoPoint_To_ASCII(float data,char* lptr)
{
     u16 Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u16 Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
     u16 Temp ;
	 
     Temp = (u16)(data*100);    //保留两位有效数字，220.5689*100 = 22056
     Int_Data4 = Temp/10000;    //Int_Data4 = 2
     Res_Data4 = Temp%10000;    //Res_Data4 = 2056
     
     Int_Data3 = Res_Data4/1000;   //Int_Data3 = 2
     Res_Data3 = Res_Data4%1000;   //Res_Data3 = 056
     
     Int_Data2 = Res_Data3/100;    //Int_Data2 = 0
     Res_Data2 = Res_Data3%100;    //Res_Data2 = 56
     
     Int_Data1 = Res_Data2/10;     //Int_Data1 = 5
     Res_Data1 = Res_Data2%10;     //Res_Data1 = 6
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data4+48;    //lptr[0] = '2';  arr[0] = '2';
        *lptr++ = Int_Data3+48;    //lptr[1] = '2';  arr[1] = '2';
        *lptr++ = Int_Data2+48;    //lptr[2] = '0';  arr[2] = '0';
        *lptr++ = '.';             //lptr[3] = '.';  arr[3] = '.';
        *lptr++ = Int_Data1+48;    //lptr[4] = '5';  arr[4] = '5';
        *lptr++ = Res_Data1+48;    //lptr[5] = '6';  arr[5] = '6';
     }
     else if(Int_Data3>0)
     {
         *lptr++ = '0';
        *lptr++ = Int_Data3+48;
        *lptr++ = Int_Data2+48;
         *lptr++ = '.';
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
        *lptr++ = Int_Data2+48;
         *lptr++ = '.';
        *lptr++ = Int_Data1+48;
        *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)    
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = Res_Data1+48;  
     }
     else
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
        *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
void* TreePoint_To_ASCII(float data,char* lptr)
{
     u32 Int_Data5,Int_Data4,Int_Data3,Int_Data2,Int_Data1;
     u32 Res_Data5,Res_Data4,Res_Data3,Res_Data2,Res_Data1;  
     u32 Temp ;
	 
     Temp = (u32)(data*1000);       //保留三位有效数字;  199.78654*1000 = 199786 
     
     Int_Data5 = Temp/100000;       //Int_Data5 = 1
     Res_Data5 = Temp%100000;       //Int_Data5 = 99786 
     
     Int_Data4 = Res_Data5/10000;   //Int_Data4 = 9 
     Res_Data4 = Res_Data5%10000;   //Res_Data4 = 9786 
     
     Int_Data3 = Res_Data4/1000;    //Int_Data3 = 9 
     Res_Data3 = Res_Data4%1000;    //Res_Data3 = 786 
     
     Int_Data2 = Res_Data3/100;     //Int_Data2 = 7
     Res_Data2 = Res_Data3%100;     //Res_Data2 = 86 
     
     Int_Data1 = Res_Data2/10;      //Int_Data1 = 8
     Res_Data1 = Res_Data2%10;      //Res_Data1 = 6 
     
     if(Int_Data4>0)
     {
        *lptr++ = Int_Data5+48;     //lptr[0] = '1'
        *lptr++ = Int_Data4+48;     //lptr[1] = '9'
        *lptr++ = Int_Data3+48;     //lptr[2] = '9'
        *lptr++ = '.';              //lptr[3] = '.'
        *lptr++ = Int_Data2+48;     //lptr[4] = '7'
        *lptr++ = Int_Data1+48;     //lptr[5] = '8'
        *lptr++ = Res_Data1+48;     //lptr[6] = '6'
     }
     else if(Int_Data3>0)
     {
         *lptr++ = '0';
         *lptr++ = Int_Data4+48;
         *lptr++ = Int_Data3+48;
         *lptr++ = '.';
         *lptr++ = Int_Data2+48;
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }
       else if(Int_Data2>0)
       {
           *lptr++ = '0';
           *lptr++ = '0';
           *lptr++ = Int_Data3+48;
           *lptr++ = '.';
           *lptr++ = Int_Data2+48;
           *lptr++ = Int_Data1+48;
           *lptr++ = Res_Data1+48;
     }
       else if(Int_Data1>0)    
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = Int_Data2+48;
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }   
     else if(Res_Data1>0)
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = Int_Data1+48;
         *lptr++ = Res_Data1+48;
     }
     else
     {
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '.';
         *lptr++ = '0';
         *lptr++ = '0';
         *lptr++ = '0';
     }
     *lptr =  '\0';
     return lptr;
}
