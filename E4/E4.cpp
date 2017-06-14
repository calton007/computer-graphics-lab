///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E4 二维图形变换
//北极星图案变化顺序：1、放大 2、缩小 3、顺时针旋转 4、逆时针旋转 5、右下平移 6、三种对称 7、左上平移 8、四种错切
//p数组为北极星顶点坐标，考虑到北极星由多个三角形组成，因此一个三角形的三组坐标可生成一个3*3的矩阵，共16组三角形，
//所以一共有16个矩阵。

//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-10-27
#include <graphics.h>      
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;


class Matrix{
public:
	float unit[3][3];
	Matrix(int p[6])//生成三角形的3*3矩阵
	{
		int t = 0, i, j;
		for (i = 0; i<3; i++)
		{
			for (j = 0; j<2; j++)
			{
				unit[i][j] = p[t++];
			}
			unit[i][j] = 1;
		}
	};
	Matrix(int i)//生成对角矩阵，或者零矩阵
	{
		if (i == 1)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
				if (i == j)
					unit[i][j] = 1;
				else
					unit[i][j] = 0;
				}
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					unit[i][j] = 0;
				}
		}
	}
	void reset()//重置为对角矩阵
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
			if (i == j)
				unit[i][j] = 1;
			else
				unit[i][j] = 0;
			}
	}
	void set(float a, float b, int type)//type决定变换类型，根据类型设置矩阵
	{
		switch (type)
		{
			//平移
			case 0:
			{
				unit[2][0] = a;
				unit[2][1] = b;
				break;
			}
				//旋转
			case 1:
			{
				unit[0][0] = cos(a / 3.14);
				unit[0][1] = sin(a / 3.14);
				unit[1][0] = -sin(a / 3.14);
				unit[1][1] = cos(a / 3.14);
				break;
			}
				//比例
			case 2:
			{
				unit[0][0] = a;
				unit[1][1] = b;
				break;
			}
				//错切
			case 3:
			{
				unit[1][0] = a;
				unit[0][1] = b;
				break;
			}
				//对称
			case 4:
			{
				unit[0][0] = a;
				unit[1][1] = b;
			}
		}
	}
	Matrix operator *(Matrix a)//运算符重载
	{
		int i, j, k;
		Matrix t(0);
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{
					t.unit[i][j] += (unit[i][k] * a.unit[k][j]);
				}
				
			}
		}
		return t;
	}
};
Matrix* m;//三角形矩阵数组（初始化在main函数中调用）

//北极星顶点坐标
int p[16][6] = { { 0, 0, 50, 10, 100, 0 }, { 0, 0, 80, 60, 50, 10 }, { 0, 0, 25, 50, 80, 60 }, { 0, 0, 0, 100, 25, 50 },
{ 0, 0, -0, 100, -25, 50 }, { 0, 0, -25, 50, -80, 60 }, { 0, 0, -80, 60, -50, 10 }, { 0, 0, -50, 10, -100, 0 },
{ 0, 0, -50, -10, -100, 0 }, { 0, 0, -80, -60, -50, -10 }, { 0, 0, -25, -50, -80, -60 }, { 0, 0, 0, -100, -25, -50 },
{ 0, 0, 0, -100, 25, -50 }, { 0, 0, 25, -50, 80, -60 }, { 0, 0, 80, -60, 50, -10 }, { 0, 0, 50, -10, 100, 0 } };

void initMatrix()//三角形矩阵数组初始化函数
{
	int i;
	m = (Matrix *)malloc(sizeof(Matrix) * 16);
	for (i = 0; i<16; i++)	//构造三角形顶点矩阵
		m[i] = Matrix(p[i]);
}

//根据矩阵r进行变换
void transform(int c,int time,Matrix r)
{
	int i;
	for (int count = 1; count <= c; count++)//变换
	{
		for (i = 0; i < 16; i++)
		{
			m[i] = m[i] * r;	//矩阵变换
			//坐标变换
			p[i][0] = m[i].unit[0][0];
			p[i][1] = m[i].unit[0][1];
			p[i][2] = m[i].unit[1][0];
			p[i][3] = m[i].unit[1][1];
			p[i][4] = m[i].unit[2][0];
			p[i][5] = m[i].unit[2][1];
		}
		cleardevice();//清除屏幕
		for (i = 0; i<16; i += 2)	//画出北极星的图案
		{
			fillpolygon((POINT*)p[i], 3);
			polygon((POINT*)p[i + 1], 3);
		}
		Sleep(time); //延时
	}
}
int main()
{
	
	int i;
	initMatrix();//三角形矩阵数组初始化
	initgraph(640, 480);
	setorigin(320, 240);//设置原点坐标为屏幕中心
	setbkcolor(0xFC5454);//背景设置为蓝色
	cleardevice();
	setlinecolor(0x0000A8);//线条设置为红色
	setfillcolor(0x0000A8);//填充设置为红色

	for (i = 0; i<16; i += 2)	//画出北极星的图案
	{
		fillpolygon((POINT*)p[i], 3);
		polygon((POINT*)p[i + 1], 3);
	}
	Matrix r(1);
	//比例变换
	r.set(1.01, 1.01, 2);
	transform(50, 50, r);//放大
	r.reset();
	Sleep(500);
	r.set(0.99, 0.99, 2);//缩小
	transform(50, 50, r);
	r.reset();
	Sleep(500);

	//旋转
	r.set(1, 0, 1);
	transform(25, 100, r);//顺时针
	r.reset();
	Sleep(500);
	r.set(-1, 0, 1);//逆时针
	transform(25, 100, r);
	r.reset();
	Sleep(500);

	//平移
	r.set(1.5, 1, 0);//右下
	transform(100,25, r);
	r.reset();	
	Sleep(500);

	//对称
	r.set(-1, 1, 4);//关于y轴对称
	transform(2, 1000, r);
	r.reset();
	Sleep(500);
	r.set(1, -1, 4);//关于x轴对称
	transform(2, 1000, r);
	r.reset();
	Sleep(500);
	r.set(-1, -1, 4);//关于原点对称
	transform(2, 1000, r);
	r.reset();
	Sleep(500);

	//平移
	r.set(-1.5, -1, 0);//左上
	transform(100, 25, r);
	r.reset();
	Sleep(500);

	//错切
	r.set(0.05, 0, 3);
	transform(20,50, r);
	r.reset();
	Sleep(500);
	r.set(-0.05, 0, 3);
	transform(20, 50, r);
	r.reset();
	Sleep(500);
	r.set(0, 0.05, 3);
	transform(20, 50, r);
	r.reset();
	Sleep(500);
	r.set(0, -0.05, 3);
	transform(20, 50, r);

	_getch();
	closegraph();
	free(m);
	return 0;
}