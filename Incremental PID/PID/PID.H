#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"
/*
δOUT= OUTK- OUTk-1 =kp(EK-EK-1)+((Kp*T)/Ti)Ek+(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2))；
Kp:算法增益调节
Ek:本次的偏差
Ek-1:上次的偏差
Ek-2:上上次的偏差
Ti:积分时间
TD:微分时间常数
Ek = Sv-Xk;其中: Sv为设定值，Xk为当前采样值，即最近一次采样值
T:采样周期，也叫控制周期，每个T时间段进行一次PID计算
*/


typedef struct
{
    float curr;  //当前温度
	float set;   //设定温度
	float En;      //本次偏差值
	float En_1;    //上一次偏差值
	float En_2;    //上上一次偏差值
	
	float Kp;   //比例系数
	float Tsam; //采样周期---控制周期，每隔Tsam控制器输出一次PID运算结果
	float Ti;//积分时间常数
	float Td;//微分时间常数
	
	float Dout;//增量PID计算本次应该输出的增量值--本次计算的结果
	
	u16 calc_cycle;
	short currpwm;  //当前的pwm宽度
	u16 pwm_cycle;//pwm的周期
}PID;


extern u8 STATUS;
extern PID pid;
void PIDParament_Init(void);  //
void pid_calc(void);  //pid??

#endif

