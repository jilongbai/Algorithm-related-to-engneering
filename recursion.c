#include <stdio.h>

//递归的详细过程分析

void recursion(int n);
/****************************************
执行结果：
Before recursion, n = 3.
Before recursion, n = 2.
Before recursion, n = 1.
End recursion, n = 1.
After recursion, n = 1.
After recursion, n = 2.
After recursion, n = 3.
******************************************/

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
		recursion(n-1);
	}
	else
	{
		printf("End recursion, n = %d.\n", n);
	}
	printf("After recursion, n = %d.\n", n);
}
/*
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
*/



