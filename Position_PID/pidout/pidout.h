#ifndef _pidout_
#define _pidout_

#include "stm32f10x_conf.h"

#define pwmout_1    GPIO_SetBits(GPIOB, GPIO_Pin_8)    //停止加热
#define pwmout_0    GPIO_ResetBits(GPIOB, GPIO_Pin_8)  //加热
void PIDOUT_init(void);//pid输出脚初始化
void PID_out(void); //输出PID运算结果到负载

#endif

