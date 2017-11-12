#include "max6675.h"
#include "pid.h"

float temper;
extern u16 Kms10;


#define cs_1   GPIO_SetBits(GPIOC, GPIO_Pin_11)
#define cs_0   GPIO_ResetBits(GPIOC, GPIO_Pin_11)

#define sck_1  GPIO_SetBits(GPIOC, GPIO_Pin_12)
#define sck_0   GPIO_ResetBits(GPIOC, GPIO_Pin_12)

#define so GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) 

u16 read_max6675()  //????
{
  u16 d,i; 
  //so_1;
  cs_0;
  delay_us(2);
  for(i=0;i<16;i++)
  {
    sck_1;
    delay_us(2);
    sck_0;
    delay_us(2);
    d<<=1;
    if(so)
     d++;
  }
  cs_1;
  return d;
}

void read_temper()//每隔10ms*20 = 200ms进行一次温度采集，作为PV
{
  u16 d;
	if(Kms10<20)  return ;
	
  d=read_max6675();
  pid.curr=((d>>4)&0x0fff)*0.25;//将对应的值转化为温度值，并将Kms10给全局变量清零
  Kms10=0;	
}


void Max6675_Init()
{GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //开A口时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);  //开D口时钟
	
	 
   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


