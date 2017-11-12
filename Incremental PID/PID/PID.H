#ifndef _pid_
#define _pid_
#include "stm32f10x_conf.h"
/*
��OUT= OUTK- OUTk-1 =kp(EK-EK-1)+((Kp*T)/Ti)Ek+(((Kp*TD)/T)*(Ek-2Ek-1+Ek-2))��
Kp:�㷨�������
Ek:���ε�ƫ��
Ek-1:�ϴε�ƫ��
Ek-2:���ϴε�ƫ��
Ti:����ʱ��
TD:΢��ʱ�䳣��
Ek = Sv-Xk;����: SvΪ�趨ֵ��XkΪ��ǰ����ֵ�������һ�β���ֵ
T:�������ڣ�Ҳ�п������ڣ�ÿ��Tʱ��ν���һ��PID����
*/


typedef struct
{
    float curr;  //��ǰ�¶�
	float set;   //�趨�¶�
	float En;      //����ƫ��ֵ
	float En_1;    //��һ��ƫ��ֵ
	float En_2;    //����һ��ƫ��ֵ
	
	float Kp;   //����ϵ��
	float Tsam; //��������---�������ڣ�ÿ��Tsam���������һ��PID������
	float Ti;//����ʱ�䳣��
	float Td;//΢��ʱ�䳣��
	
	float Dout;//����PID���㱾��Ӧ�����������ֵ--���μ���Ľ��
	
	u16 calc_cycle;
	short currpwm;  //��ǰ��pwm����
	u16 pwm_cycle;//pwm������
}PID;


extern u8 STATUS;
extern PID pid;
void PIDParament_Init(void);  //
void pid_calc(void);  //pid??

#endif
