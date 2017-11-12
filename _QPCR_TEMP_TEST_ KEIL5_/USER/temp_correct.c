
#include "main.h"
#include "temp_correct.h"

/*
《聚合酶链反应分析仪校准规范》对PCR仪的温度控制程序设定要求如下:
1 30°C 1min --> 2 95°C 3min --> 3 30°C 2min --> 4 90°C 3min -->
5 50°C 3min --> 6 70°C 3min -->7 60°C 3min --> 8 30°C 1min
*/

//该表格做查表用
const float Temp_Comp[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};
//该表格记录校准设备在校准点的数据
const float Temp_Correct[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};
//该表格记录工装在个校准点的数据
const float Temp_Gongzhc[20] = {105.00, 95.00, 90.00, 70.00, 60.00, 50.00, 30.00};


/*
函数功能：实现对整个系统的温度校准
输入参数：float temprature,输入需要校准的温度值
输出参数：校准后的温度值
NOTE:此处采用点斜式计算y = k*x + b；线性方程
*/
float Temp_Correction(float temprature)
{
	u16 X = 0;
	u16 i = 0;
	float T = 0;
	float k = 0;
	
	//如果是在《聚合酶链反应分析仪校准规范》规定的温度之外，则不进行校准，原来是多少就是多少
	if ((temprature < 30.00) || (temprature > 105.00))
	{
		return temprature;
	}

	//在《聚合酶链反应分析仪校准规范》规定的温度之外，则进行分段校准
	if(temprature >= Temp_Comp[3])
	{
		/*
		举例1：temprature = 93；则 93 > 70; i = 1; 93>90; i =2; 93<95; 最终i = 2； X = 3-2 = 1;
		举例2：temprature = 98；则 98 > 70; i = 1; 98>90; i =2; 98<95; i=3； 95<105; X = 3-3 = 0
		*/
		while(temprature >= Temp_Comp[3-i])
		{
			i++;
		} 
		X = 3 - i ;  
		//求斜率
		k = (Temp_Correct[X+1] - Temp_Correct[X]) / (Temp_Gongzhc[X+1] - Temp_Gongzhc[X]);
		//利用点斜式实现线性插值
		T = Temp_Correct[X] + k*(temprature - Temp_Gongzhc[X]);  
	}
	else
	{
		/*
		举例1：temprature = 55；则 55 < 70; i = 1; 55<60; i =2; 55>50; 最终i = 2； X = 3+2 = 5;
		举例2：temprature = 36；则36 < 70; i = 1; 36<60; i =2; 36<50; i=3； 36>30;  最终i =3; X = 3+3 = 6
		*/
		while(temprature < Temp_Comp[3+i])
		{
			i++;
		}
		X = 3 + i; 
		k = (Temp_Correct[X] - Temp_Correct[X-1]) / (Temp_Gongzhc[X] - Temp_Gongzhc[X-1]);
		T = Temp_Correct[X-1] + k*(temprature - Temp_Gongzhc[X-1]); 
	}	
	return T;	
}

/*
NOTE:上面的函数采用点斜式实现分段线性插值
	(1)temprature >= Temp_Comp[3]
	(x0, y0)-->( Temp_Gongzhc[X], Temp_Correct[X])
	(x1, y1)-->(Temp_Gongzhc[X+1], Temp_Correct[X+1])
	k = (y1-y0)/(x1-x0) = (Temp_Correct[X+1] - Temp_Correct[X]) / (Temp_Gongzhc[X+1] - Temp_Gongzhc[X]);
	y = y0 + k(x -x0)   =  Temp_Correct[X] + k*(temprature - Temp_Gongzhc[X]);               
	==> y = y0 + (y1-y0)*(x-x0)/(x1-x0);
	
	(2)temprature < Temp_Comp[3]
	(x0, y0)-->( Temp_Gongzhc[X-1], Temp_Correct[X-1])
	(x1, y1)-->(Temp_Gongzhc[X], Temp_Correct[X])
	k = (y1-y0)/(x1-x0) = (Temp_Correct[X] - Temp_Correct[X-1]) / (Temp_Gongzhc[X] - Temp_Gongzhc[X-1]);
	y = y0 + k(x -x0)   =  Temp_Correct[X-1] + k*(temprature - Temp_Gongzhc[X-1]);               
	==> y = y0 + (y1-y0)*(x-x0)/(x1-x0);	
*/


