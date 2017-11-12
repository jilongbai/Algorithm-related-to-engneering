#include "pidout.h"
#include "pid.h"
void PIDOUT_init()//pid输出脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开B口时钟  
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void PID_out()  //输出PID运算结果到负载---每1ms被调用1次
{
   static u16 pw;
	 pw++;
	 if(pw>=pid.pwmcycle)  //
	 {
	   pw=0;
	 }
	  //0  ~  pid.pwmcycle-1
	 
	 if(pw<pid.OUT)
	 {
	   pwmout_0;//加热
	 }
	 else
	 {
	   pwmout_1;//停止加热
	 }
	 
}
