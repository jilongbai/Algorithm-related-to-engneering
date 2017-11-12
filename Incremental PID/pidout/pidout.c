#include "pidout.h"
#include "pid.h"

extern u8 rtc[3];
void PWM_init()
{ 
 GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开A口时钟
	
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
}

void pwm_out()
{
	static u16 ms;
	ms++;
	if(ms>=pid.pwm_cycle )
	{
	 ms=0;
	}
	
  if(ms>=pid.currpwm)
	{
	  pwmout_1 ;   //停止
	}
	else
	{
	  pwmout_0;   //加热
	}
}

