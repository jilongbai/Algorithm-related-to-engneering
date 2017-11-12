#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"

typedef struct
{
 float Sv;//用户设定值
 float Pv;
 
 float Kp;
 float T;  //PID计算周期--采样周期
 float Ti;
 float Td; 
	
	
	
 float Ek;  //本次偏差
 float Ek_1;//上次偏差
 float SEk; //历史偏差之和
	
		float Iout;
	float Pout;
	float Dout;
	
 float OUT0;

 float OUT;
	
	
 u16 C10ms;
	
 u16 pwmcycle;//pwm周期
	
}PID;

extern PID pid; //存放PID算法所需要的数据
void PID_Calc(void); //pid计算

#endif
