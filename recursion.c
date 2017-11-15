#include <stdio.h>

//递归的详细过程分析

void recursion(int n);
/**************************************************************************************
执行结果：
Before recursion, n = 3.
Before recursion, n = 2.
Before recursion, n = 1.
End recursion, n = 1.
After recursion, n = 1.
After recursion, n = 2.
After recursion, n = 3.
递归的过程其实就是一层又一层的保护现场，如果递归深度特别大，那么将会占用大量的栈空间，
最终有可能栈空间会被撑爆，整个程序此时就会死掉，多以递归必须要具有收敛性，即递归的终止
条件，也就是说递归的终止条件，在最终的时候必须得到满足，然后才能结束递归。

每次递归时候，必然会占用栈空间，所以在栈空间在撑爆之前，必须到达终止条件，然后才能结束
递归。

总结：
递归函数的原则：收敛性，栈溢出
****************************************************************************************/

int main(void)
{
	recursion(3);
	
	return 0;
}

void recursion(int n)
{
	printf("Before recursion, n = %d.\n", n);
	if (n > 1)
	{
		recursion(n-1);  //如果此处将函数改为recursion(n);那么将会进入死循环，直至最终将栈空间撑爆
	}
	else
	{
		printf("End recursion, n = %d.\n", n);
	}
	printf("After recursion, n = %d.\n", n);
}
/*********************************************************************************************
执行结果分析：
       (1)n=3; Before recursion, n = 3.
       (2)n>1. n-1 = 2; Before recursion, n = 2.
	   (3)n>2. n-1 = 1; Before recursion, n = 1.
	   (4)n=1. 目前刚刚进入在recursion(1);此函数中，由于不满足n>1的条件，故会直接执行后
	      面的的两条语句，此时会End recursion, n = 1.After recursion, n = 1.
	   (5)此时，recursion(1);该函数执行完成，而此时相当于是recursion(2);该函数的if(2>1)
		  内部语句执行完成，else的语句不满足，所以跳过执行，故此时会执行After recursion, n = 2.
	   (6)当上述动作完成之后，相当于是recursion(3);此函数的if(3>1)该语句被执行完成，而
	      else的条件不成立，所以不执行End recursion...,而是执行After recursion, n = 3.这条语句。
**********************************************************************************************/

/*********************************************************************************************
如果上述递归函数修改如下，将会迅速撑爆栈空间，进而栈溢出，发生段错误：

void recursion(int n)
{
	int a[100];   //局部变量，为了快速撑爆栈，所以定义了大数组，
	//printf("Before recursion, n = %d.\n", n);
	if (n > 1)
	{
		recursion(n);  //此处将函数改为recursion(n);那么将会进入死循环，直至最终将栈空间撑爆
	}
	else
	{
		printf("End recursion, n = %d.\n", n);
	}
	printf("After recursion, n = %d.\n", n);
}

*********************************************************************************************/



