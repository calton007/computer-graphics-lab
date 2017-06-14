///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E1_2 奇数n阶完全图一笔画
//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-09-22
//参考文献：李明勇,李忠.奇顶点完全图中欧拉回路的设计与实现.宜宾学院学报,2003年6期:98-103
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#define R 200
#define PI 3.1415926
int gcd(int a, int b)
{
	int r;
	r = a%b;
	while (r)
	{
		a = b;
		b = r;
		r = a%b;
	}
	return b;
}
int main()
{
	int d, flag, i, j, k, p, *x, *y,n;
	float t;
	printf("请输入多边形边数:");
	scanf_s("%d", &n);	//n为多边形的边数
	x = new int[n];
	y = new int[n];
	t = 2 * PI / n;
	for (i = 0; i < n; i++)		//生成顶点坐标
	{
		x[i] = R * (1.1 + cos(i * t));
		y[i] = R * (1.1 + sin(i * t));
	}
	initgraph(640, 480);	
	moveto(x[0], y[0]);
	flag = 1;
	p = 0;
	for (i = 1; i <= (n - 1) / 2; i++)
	{
		if (gcd(i, n) == 1)		//判断i和n是否互质
		{
			if (flag == 1)
				for (j = 1; j <= n; j++)
				{
					p = (p + i) % n;
					lineto(x[p], y[p]);
				}
					
			else
				while (p)
				{
					p = (p + i) % n;
					lineto(x[p], y[p]);
				}
		}
		else
		{
			if (flag == 1)
			{
				d = gcd(n, i);
				for (j = 1; j <= d; j++)
				{
					for (k = 1; k <= n / d; k++)
					{
						p = (p + i) % n;
						lineto(x[p], y[p]);
					}
						p = (p + i + 1) % n;
						lineto(x[p], y[p]);
				}
				while (p)
				{
					p = (p + i + 1) % n;
					lineto(x[p], y[p]);
				}
				flag = 0;
			}
			else
			{
				d = gcd(n, i);
				for (j = 1; j < d; j++)
				{
					p = (p + i + 1) % n;
					lineto(x[p], y[p]);
					for (k = 1; k <= n / d; k++)
					{
						p = (p + i) % n;
						lineto(x[p], y[p]);
					}
				}
				while (p)
				{
					p = (p + i + 1) % n;
					lineto(x[p], y[p]);
				}				
			}
		}
	}
	_getch();
	closegraph();	
	delete[] x;
	delete[] y;
	return 0;
}


