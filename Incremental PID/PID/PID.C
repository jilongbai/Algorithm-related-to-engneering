#include "pid.h"

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

PID pid;
extern u16 pidcalcms;
void PIDParament_Init()  
{
  	pid.set =100;         //初始设定温度为100℃
	pid.currpwm=0;        //初始PWM宽度为0
	pid.pwm_cycle=100;    //PWM的周期为100
	pid.calc_cycle=100;  
	
	pid.Td=2000;
	pid.Ti=4000;
	pid.Kp=5;
	pid.Tsam=500;
}

void pid_calc()  //500ms进行一次PID计算，200ms重新采样一次，当前值
{
  float dk1;float dk2;
  float t1,t2,t3;
  
	//pidcalcms为全局变量，每隔1ms自加一
	if(pidcalcms<pid.Tsam) return ;  //加到Tsam = 500 ms时，PID的计算周期时间到，开始PID处理，否则不处理，推出该函数
	pid.En=pid.set-pid.curr;         //本次误差 比例P
	//执行完上条指令后En,En_1,En_2的值的变化
	//        第一次: En = 5; En_2 = 0; En_1 = 0;
	//        第二次: En = 2; En_2 = 0; En_1 = 5; //也就说每次，En都是最值，而En_2是上一次的En_1,即上上一次的偏差，En_1是上一次的En
	//        第三次: En = 3; En_2 = 5; En_1 = 2; //如此就能保证每次都是本次偏差，上一次偏差，上上一次偏差
	//		  第四次: En = 11; En_2 = 2; En_1 = 3;
	//        第五次: En = -2; En_2 = 3; En_1 = 11;
	dk1=pid.En-pid.En_1;             //本次偏差与上次偏差之差，微分D
	dk2=pid.En-2*pid.En_1+pid.En_2;  //dk2 = (pid.En_2 - pid.En_1) - (pid.En_1 - pid.En)

/*δOUT= OUTK- OUTk-1 =kp(EK-EK-1)+((Kp*T)/Ti)Ek+(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2))；*/	
    //一，首先计算上式中的kp(EK-EK-1)该项
    t1=pid.Kp*dk1; 
	//二，然后计算上式中的((Kp*T)/Ti)Ek这一项(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2)) = ((Kp*TD)/T)*dk2;
	t2=(pid.Kp*pid.Tsam)/pid.Ti;
	t2=t2*pid.En;
	//三，最后计算上式中的最后一项，
	t3=(pid.Kp*pid.Td)/pid.Tsam;
	t3=t3*dk2;
	
  	pid.Dout=t1+t2+t3;  //本次应该输出的增量
  	
	pid.currpwm+=pid.Dout;  //针对上一次计算出本次PID计算以后PID值的需要的变化量相比上一次

	//如果占空比>=100%的情况，都按照100%的情况计算
	if(pid.currpwm>pid.pwm_cycle)
	{
	  pid.currpwm=pid.pwm_cycle;
	}
	//如果出线占空比<0%的情况，都按照0%的情况计算
	if(pid.currpwm<0)
	{
	 pid.currpwm=0;
	}
	//更新偏差值，因为每次变化的都是pid.En,

	pid.En_2=pid.En_1;
	pid.En_1=pid.En;
	//举例:在指行上面一条语句之后的情况
	//        第一次: En = 5; En_2 = 0; En_1 = 5;
	//        第二次: En = 2; En_2 = 5; En_1 = 2;
	//        第三次: En = 3; En_2 = 2; En_1 = 3;
	//		  第四次: En = 11; En_2 = 3; En_1 = 11;
	//        第五次: En = -2; En_2 = 11; En_1 = -2;

	//将pidcalcms赋值为0，开始下一次PID计算
    pidcalcms=0;
}
