#include "pid.h"

PID pid; //存放PID算法所需要的数据


void PID_Calc()  //pid计算
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
 if(pid.C10ms<(pid.T))  //计算周期未到
 {
    return ;
 }

 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
 pid.Pout=pid.Kp*pid.Ek;      //比例输出
 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
}


void PID_Calc_overshoot()  //pid计算-过冲处理�
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
  float SvSave;
 if(pid.C10ms<(pid.T))  //计算周期未到，pid.C10ms是每1ms自加一，所以可以用来和PID的计算时间来对比
 {
    return ;
 }

//
//猜测:该程序不完整，理论上应该没进行一次PID计算，则pid.times++,而且在温度<65℃的时候，pid.times恒等于0，即此时不进行过冲处理
//在定时器3的中断循环中对pid.times进行++操作，每1s中pid.times自加一，对过冲的处理主要是通过时间来控制
//而，当温度升高到65度以后，则开始过冲处理
//如何进行过冲处理呢?就是设置在<120℃一下分阶段进行PID处理，
//0-60s处理65-72℃
//60-100s处理72-96度的PID过程
//100-200s处理96-102度的PID过程
//200-300s处理102-108度的PID过程
//300-400s处理108-114度的PID过程
//即每6℃的温度梯度，需要100s的时间来进行处理，进而防止过冲


//判断Sv
  SvSave=pid.Sv;     //Sv = 120
 
 if(pid.Pv<(SvSave*0.55))       //温度达到计数开始 66度
 {
    pid.times=0;
 }
 
  if(pid.times<=60)//3               
 {
	//120*0.6 = 72
	pid.Sv=SvSave*0.6;
 }
 
 if((pid.times>60)&&(pid.times<=100))//3分钟 96度  100s
 {
  pid.Sv=SvSave*0.8;
 }
 
 if((pid.times>100)&&(pid.times<=200))//2分钟 102度 100s
 {
  pid.Sv=SvSave*0.85;
 }
 
 if((pid.times>200)&&(pid.times<=300))//2分钟 108度 100s
 {
  pid.Sv=SvSave*0.9;
 }
 
  if((pid.times>300)&&(pid.times<=400))//2分钟 114度 100s
 {
  pid.Sv=SvSave*0.95;
 }
 
 if(pid.times>400)
 {
  pid.Sv=SvSave;
 }
//////////////////////////////////
/*
                         n
OUT=(Kp*Ek) + (Kp*(T/Ti)∑Ek) + (Kp*(TD/T)(EK-Ek-1)) +OUT0           
                        k=0
Kp:算法增益调节
Ek:当前本次偏差
T: 采样周期，即控制周期，每隔T时间段进行一次PID计算
Ti:积分时间
TD:微分时间常数
Ek_1:上次偏差值
OUT0:是一个常数，目的是保证OUT输出不为0，如果OUT=0，则PID失控

令:
Ki= Kp*(T/Ti);
KD=(Kp*(TD/T)
则:
 
                   n
OUT=(Kp*Ek) + (Ki∑Ek) + (KD(EK-Ek-1)) +OUT0           
                   k=0

*/

	//计算上述公式的第一项:(Kp*Ek)
 pid.Ek=pid.Sv-pid.Pv;   //得到当前的偏差值
 pid.Pout=pid.Kp*pid.Ek;      //比例输出
    //计算上述公式的第二项: 
 pid.SEk+=pid.Ek;        //历史偏差总和
 
 DelEk=pid.Ek-pid.Ek_1;  //最近两次偏差之差
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //积分输出

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //微分输出
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //本次的计算结果
 pid.Ek_1=pid.Ek;  //更新偏差
 
 pid.C10ms=0;
 pid.Sv=SvSave;
}












