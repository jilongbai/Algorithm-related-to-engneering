
/******************************************************************************************************************************************************************
                                          
										      ///////////////////////算数平均滤波///////////////////////////
														 
1.方法：由于随机干扰(噪声型干扰)随着数据样本的增加其统计平均值趋于零，故对被检测物理量进行连续多次采样，然后求其算术平均值作为有效采样值，这样就可以达到抑制随机干扰的效果。连续采样次数越多，抑制随机干扰的效果越好。这种滤波算法就是算数平均滤波算法。
	NOTE1：虽然算数平均滤波对随机干扰的抑制效果比较好，但却不能消除毛刺型突发干扰，只要有
一个采样数据样本受到毛刺性突发干扰，算术平均值将明显偏离真实值.
		
2.例程：
		(1)调用函数：
		(2)变量说明：
		(3)常量说明：
		(4)入口：
		(5)出口：
*******************************************************************************************************************************************************************/

typedef unsigned char INT8U;
typedef unsigned int INT16U;

#define N 10
INT16U Samp[60];

void TaskSamp(void * pdata)
{
	INT8U i, n;
	INT16U R;
	
	for (n=0; n<60; n++)
	{
		R = 0;                        //初始化数据累加器
		for (i=0; i<N; i++)           //每个有效样本需要连续进行N次采样
		{
			R += SampAdc();           //进行一次采样，累加采样结果
			OSTimeDly(2);
		}
			
		Samp[n] = R/N;                //取算术平均值作为有效值，保存到有效数据组中
		OSTimeDly(3000 - 2*N);	      //再延时，使得主采样周期为1min
	}
		
		OSTaskCreate(TaskSend, (void *)0, &TaskSendStk[TaskStk-1], 6);  //创建发送任务
		
		OSTaskDel(OS_PRIO_SELF);             //采样任务删除自己
}

/********************************************************************************************************************************************************************
                                          
										      ///////////////////////去极值平均滤波///////////////////////////
														 
1.方法：去极值滤波算法其实是一个组合滤波算法，算术平均不能滤除毛刺性突发干扰，只是通过平均操作将其影响削弱，因毛刺性突发干扰使采样值远离真实值，针对毛刺型突发干扰这个特点，可以比较容易地将其剔除，不参加平均值计算，从而使平均滤波输出的值更接近真实值。
		算法原理如下：
		连续采样N次，将其累加求和，同时找出其中的最大值和最小值，再从累加和中减去最大值和最小值，将剩余的N-2个采样值求平均，即得有效采样值。
		
2.例程：
		(1)调用函数：
		(2)变量说明：
		(3)常量说明：
		(4)入口：
		(5)出口：
********************************************************************************************************************************************************************/
typedef unsigned char INT8U;
typedef unsigned int INT16U;

#define N 10
INT16U Samp[60];

void TaskSamp(void * pdata)
{
	INT8U i, n;
	INT16U R, S, max, min;
	
	for (n=0; n<60; n++)
	{
		R = max = min = SampAdc();   //必须用采样值初始化数据累加器、最大采样值和最小采样值
		                             //同时更新新一轮的数据累加器
		OSTimeDly(2);
		
		for (i=1; i<N; i++)          //NOTE:此处我们的i=1;作为我们的起始值，而不是0，因为我们在前面已经采样过一次了，所以我们后边只要采样N-1次就可以了
		{
			S = SampAdc();
			R += S;
			if (S > max)             //获得新的最大值
			{
				max = S;
			}
			if (S < min)             //获得新的最小值
			{
				min = S;
			}
			OSTimeDly(2);
		}
		
		R -= max;	                 //剔除最大值
		R -= min;                    //剔除最小值
		Samp[n] = R/(N-2);           //取算术平均值作为有效值，保存到有效数据组中
		OSTimeDly(3000 - 2*N);	     //再延时，使得主采样周期为1min
	}
		
		OSTaskCreate(TaskSend, (void *)0, &TaskSendStk[TaskStk-1], 6);   //创建发送任务
		 
		OSTaskDel(OS_PRIO_SELF);                                         //采样任务删除自己
}



