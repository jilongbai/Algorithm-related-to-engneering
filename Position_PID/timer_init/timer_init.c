
 #include "time_init.h"

#include"stm32f10x_conf.h"

  
void Timer4_init()	//T4 10ms时钟
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 10000-1; //计数个数     //100us*10=1000us=10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//分频值   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//分割时钟			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
 TIM_DeInit(TIM4);
TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM4, ENABLE); 	 //使能定时器2

 /*以下定时器4中断初始化*/
TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //向上计数溢出产生中断


}
  
void Timer3_init()	//T3 1ms时钟
{	 //72000000/7200=100us
//
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 1000-1; //计数个数     //10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//分频值   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//分割时钟			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
 TIM_DeInit(TIM3);
TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM3, ENABLE); 	 //使能定时器2

 /*以下定时器4中断初始化*/
TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //向上计数溢出产生中断


}

