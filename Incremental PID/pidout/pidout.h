#ifndef _pidout_
#define _pidout_
#include"stm32f10x_conf.h"

#define pwmout_1    GPIO_SetBits(GPIOB, GPIO_Pin_8)    //停止加热
#define pwmout_0    GPIO_ResetBits(GPIOB, GPIO_Pin_8)  //加热
extern u16 sec;
void PWM_init(void);
void pwm_out(void);
#endif
