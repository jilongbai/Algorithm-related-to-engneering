
/**********************************************************************************************************
1.方法：限幅滤波算法。又称为嵌位滤波法或者程序判断滤波法。这种滤波法的思路是：
		先根据经验判断，确定两次采样允许的最大偏差值(设为A).每次检测到新采样值时进行判断：
		(1)if 本次采样值与上次滤波结果之差≤A，则本次采样值有效，令本次采样结果 = 新采样值；
		(2)if 本次采样值与上次滤波结果之差>A，则本次采样值无效，令本次滤波结果 = 上次滤波结果；
		(3)优点：能有效克服因偶然因素引起的脉冲干扰
		(4)缺点：无法抑制那种周期性的干扰，且平滑度差：有点像PID算法中单独只有P作用时的效果
2.例程：如下函数unsigned char AmplitudeLimterFilter()
		(1)调用函数
		   GetAD();该函数用来取得当前采样值
		(2)变量说明：
		   Value:最近一次有效采样的值，该变量为全局变量
		   NewValue:当前采样的值
		   ReturnValue:返回值
		(3)常量说明：
		   A:两次采样的最大误差值，该值需要使用者根据实际情况设置
		(4)入口：
		(5)出口：
************************************************************************************************************/
#define Alias 10
unsigned char Value;

unsigned char AmplitudeLimterFilter()
{
	unsigned char NewValue;
	unsigned char ReturnValue;
	
	NewValue = GetAD();
	
	//判断当前采样值与上一次有效采样值之差是否超出我们已经确定的两次采样允许的最大偏差值
	if (((NewValue - Value) > A) || ((Value - NewValue) > A))
	{
		ReturnValue = Value;
	}	
	else
	{
		ReturnValue = NewValue;
	}
	
	return ReturnValue;
}


/*******************************************************
1.方法：连续采样N次(N取奇次)，把N次采样值按大小排列去中间值为有效值。这种滤波法的思路是：
		(1)不论当前采样值因为某正原因导致其偏差过大或者过小，(假设我们是按照从小到大排列)，那么排列之后
		(2)偏差过大者被排在了前边，偏差过小的被排到了后边，这样中间的就比较可信
		(3)优点：能有效克服因偶然因素引起的波动干扰；对温度，液位等变化缓慢的被测参数有良好的滤波效果
		(4)缺点：对流量、速度等快速变化的参数不宜
2.例程：如下函数unsigned char MiddleValueFilter()
		(1)调用函数
		   GetAD();该函数用来取得当前采样值
		   Delay();基本延时函数
		(2)变量说明：
		   ArrDataBuffer[N]:用来存放一次性采集的N组数据
		   Temp:完成冒泡法使用的临时寄存器
		   i,j,k:循环使用的参数值
		(3)常量说明：
		   N:数组长度
		(4)入口：
		(5)出口：
********************************************************/

#define N 9

unsigned char MiddleValueFilter()
{
	unsigned char i, j, k;
	unsigned Temp;
	unsigned char ArrDataBuffer[N];
	
	for (i=0; i<N; i++)
	{
		ArrDataBuffer[i] = GetAD();
		Delay();
	}
	
	for (j=0; j<N-1; j++)
	{
		for (k=0; k<N-j-1; k++)
		{
			if (ArrDataBuffer[k] > ArrDataBuffer[k+1])
			{
				Temp = ArrDataBuffer[k];
				ArrDataBuffer[k] = ArrDataBuffer[k+1];
				ArrDataBuffer[k+1] = Temp;
			}	
		}
	}
	
	return ArrDataBuffer[(N-1)/2];
}

/*************************************************************






*************************************************************/

#define N 12

unsigned char ArithmeticalAverageValueFilter()
{
	unsigned char i;
	unsigned int sum = 0;
	unsigned char value;
	
	for (i=0; i<N; i++)
	{
		sum += GetAD();
		Delay();
	}	
	
	value = sum/N;
	
	return value;
	
}


/*********************************************************






********************************************************/
#define N 12
unsigned char Data[];

unsigned char GlideAverageValueFilter()
{
	unsigned char i;
	unsigned char Value;
	unsigned sum = 0;
	
	Data[N] = GetAD();
	
	for (i=0; i<N; i++)
	{
		Data[i] = Data[i+1];
		sum += Data[i];
	}
	
	Value = sum/N;
	
	return Value;	
}


/*******************************************





********************************************/

#define  N  12
unsigned char MiddleAverageValueFilter()
{
	unsigned char i, j, k, l;
	unsigned char Temp;
	unsigned int sum = 0;
	unsigned char Value;
	unsigned char ArrDataBuffer[N];
	
	for (i=0; i<N; i++)
	{
		ArrDataBuffer[i] = GetAD();
		Delay();
	}
	
	for (j=0; j<N-1; j++)
	{
		for (k=0; k<N-j-1; k++)
		{
			if (ArrDataBuffer[k] > ArrDataBuffer[k+1])
			{
				Temp = ArrDataBuffer[k];
				ArrDataBuffer[k] = ArrDataBuffer[k+1];
				ArrDataBuffer[k+1] = Temp;
			}	
		}
	}
	
	for (l=1; l<N-1; l++)
	{
		sum += ArrDataBuffer[l];	
	}
	
	Value = sum/(N-2);
	
	return Value;
}


/*****************************************************




*****************************************************/
unsigned char filter1(unsigned char NEW_DATA, unsigned char QUEUE[], unsigned char n)
{
	unsigned char i;
	unsigned char max, min;
	unsigned int sum;
	unsigned char Value;
	QUEUE[0] = NEW_DATA;
	max = QUEUE[0];
	min = QUEUE[0];
	sum = QUEUE[0];
	
	for (i=n-1; i!=0; i--)
	{
		if (QUEUE[i] > max)
		{
			max = QUEUE[i];
		}	
		else if (QUEUE[i] < min)
		{
			min = QUEUE[0];
		}
		sum += QUEUE[i];
		QUEUE[i] = QUEUE[i-1];
	}
	
	i = n-2;
	
	sum = sum -max -min + i/2;
	
	Value = sum/i;  //(i/2)/i= 0.5,故在sum中加入i/2是为了方便四舍五入
	
	return Value;
}


/*****************************************************



*****************************************************/

#define A 10
#define N 12
unsigned char Data[];

unsigned char LimitRangeAverageValueFilter()
{
	unsigned char i;
	unsigned int sum;
	unsigned char Value;
	
	Data[N] = GetAD();
	if (((Data[N]-Data[N-1]) > A) || ((Data[N-1] - Data[N]) > A))
	{
		Data[N] = Data[N-1];
	}
	else 
	{
		Data[N] = Data[N];
	}
	
	for (i=0; i<N; i++)
	{
		Data[i] = Data[i+1];
		sum += Data[i];
	}
	
	Value = sum/N;
	
	return Value;
}

/*******************************************



*******************************************/
#define a 128
unsigned char Value;

unsigned char OneFactorialFilter()
{
	unsigned char NewValue;
	unsigned char ReturnValue;
	
	NewValue = GetAD();
	ReturnValue = (255-a)*NewValue + a*Value;    
	//加权系数其实是:(255-a)/255 = (255-128)/255 = 0.5, a/255 = 128/255 = 0.5
	ReturnValue /= 255;
	
	return ReturnValue;	
}

/****************************************************




****************************************************/

#define N 10
#define CoeSum 55

const Coefficient[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
unsigned char Data[];

unsigned char AAGAFilter()   //AAGA:AddAuthorityGlideAverageValueFilter
{
	unsigned char i;
	unsigned int sum = 0;
	Data[N] = GetAD();
	
	for (i=0; i<N; i++)
	{
		Data[i] = Data[i+1];
		sum += Data[i]*Coefficient[i];
	}
	
	Value = sum/CoeSum;
	
	return Value;	
}

/*****************************************************


******************************************************/

#define N 20
unsigned char Count;
unsigned char Value;

unsigned char AvoidDitheringFilter()
{
	unsigned char NewValue;
	NewValue = GetAD();
	
	if (NewValue == Value)
	{
		Count = 0;
	}
	else
	{
		Count++;
		
		if (Count > N)
		{
			Count = 0;
			Value = NewValue;
		}	
	}
	
	return Value;
}
















