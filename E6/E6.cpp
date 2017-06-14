///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E6 立方体的正轴测消隐
//运行后，显示立方体的正轴测，按下任意键后，开始旋转
//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-11-19

#include <graphics.h>      
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;
const int MATRIX_SIZE = 4;
class Matrix{
public:
	float unit[MATRIX_SIZE][MATRIX_SIZE];
	Matrix(int i)//生成对角矩阵，或者零矩阵
	{
		if (i == 1)
		{
			for (int i = 0; i < MATRIX_SIZE; i++)
				for (int j = 0; j < MATRIX_SIZE; j++)
				{
				if (i == j)
					unit[i][j] = 1;
				else
					unit[i][j] = 0;
				}
		}
		else
		{
			for (int i = 0; i < MATRIX_SIZE; i++)
				for (int j = 0; j < MATRIX_SIZE; j++)
				{
				unit[i][j] = 0;
				}
		}
	}
	void reset()//重置为对角矩阵
	{
		for (int i = 0; i < MATRIX_SIZE; i++)
			for (int j = 0; j < MATRIX_SIZE; j++)
			{
			if (i == j)
				unit[i][j] = 1;
			else
				unit[i][j] = 0;
			}
	}
	Matrix operator *(Matrix a)//运算符重载
	{
		int i, j, k;
		Matrix t(0);
		for (i = 0; i < MATRIX_SIZE; i++)
		{
			for (j = 0; j < MATRIX_SIZE; j++)
			{
				for (k = 0; k < MATRIX_SIZE; k++)
				{
					t.unit[i][j] += (unit[i][k] * a.unit[k][j]);
				}

			}
		}
		return t;
	}
};
class Point{
public:
	float p[4];
	Point(){}
	Point(float a, float b, float c, float d)
	{
		p[0] = a;
		p[1] = b;
		p[2] = c;
		p[3] = d;
	}
	Point operator *(const Matrix &m)
	{
		int i, j;
		Point t(0, 0, 0, 0);
		for (i = 0; i < MATRIX_SIZE; i++)
		{
			for (j = 0; j < MATRIX_SIZE; j++)
			{
				t.p[i] += (p[j] * m.unit[j][i]);
			}
		}
		if (t.p[3] != 1) //齐次化
		{
			t.p[0] /= t.p[3];
			t.p[1] /= t.p[3];
			t.p[2] /= t.p[3];
		}
		return t;
	}
};
class Plane{
public:
	int begin;	//起始位置
	int end;	//终止位置
	bool visible;	//是否可见
	Plane(int b, int e, bool v)
	{
		begin = b;  
		end = e;	
		visible = v;	
	}
};
//绘制图案

//顶点表
Point point[8] = { Point(0, 0, 0, 1), Point(10, 0, 0, 1), Point(0, 10, 0, 1), Point(10, 10, 0, 1), Point(0, 0, 10, 1), Point(10, 0, 10, 1), Point(0, 10, 10, 1), Point(10, 10, 10, 1) };
//面表
Plane plane[6] = { Plane(0, 4, true), Plane(5, 9, true), Plane(10, 14, true), Plane(15, 19, true), Plane(20, 24, true), Plane(25, 29, true) };
//环表
int edge[30] = { 1, 3, 4, 2, 1, 5, 6, 8, 7, 5, 1, 2, 6, 5, 1, 3, 7, 8, 4, 3, 1, 5, 7, 3, 1, 2, 4, 8, 6, 2 };

void draw(const Point point[], const int edge[])
{
	int i, j;
	for (i = 0; i < 6; i++)
	{
		if (plane[i].visible == true)
		{
			for (j = 0; j < 4; j++)
			{
				line(point[edge[plane[i].begin + j] - 1].p[0], point[edge[plane[i].begin + j] - 1].p[2], point[edge[plane[i].begin + j + 1] - 1].p[0], point[edge[plane[i].begin + j + 1] - 1].p[2]);
			}
		}
	}
}

void judge(Plane plane[], const Point point[])
{
	int x1, z1, x2, z2;
	float e;
	for (int i = 0; i < 6; i++)
	{
		//计算平面上的两个向量
		//plane[].begin：平面的起点在环表的位置，
		//edge[]：取出点的坐标的位置
		//点从1开始编号，所以取得时候要减去1
		//point[].p[0]为点的x坐标，point[].p[2]为点的z坐标
		x1 = point[edge[plane[i].begin + 1] - 1].p[0] - point[edge[plane[i].begin] - 1].p[0];
		z1 = point[edge[plane[i].begin + 1] - 1].p[2] - point[edge[plane[i].begin] - 1].p[2];
		x2 = point[edge[plane[i].end - 1] - 1].p[0] - point[edge[plane[i].end] - 1].p[0];
		z2 = point[edge[plane[i].end - 1] - 1].p[2] - point[edge[plane[i].end] - 1].p[2];
		//求法向量与y轴夹角
		e = (x1*z2) - (x2*z1);
		if (e >= 0)
			plane[i].visible = true;
		else
			plane[i].visible = false;
	}
}
void projection()
{
	Point temp[8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = point[i];
	}
	Matrix m(1), t(1);
	//绕z轴旋转
	m.unit[0][0] = 0.707;
	m.unit[0][1] = 0.707;
	m.unit[1][0] = -0.707;
	m.unit[1][1] = 0.707;
	//绕x轴旋转
	t.unit[1][1] = 0.816;
	t.unit[1][2] = -0.577;
	t.unit[2][1] = 0.577;
	t.unit[2][2] = 0.816;
	m = m*t;
	t.reset();

	//投影到XOZ平面
	t.unit[1][1] = 0;
	m = m*t;
	t.reset();

	//放大
	t.unit[0][0] = 10;
	t.unit[1][1] = 10;
	t.unit[2][2] = 10;
	m = m*t;
	t.reset();

	//坐标变换
	for (int i = 0; i < 8; i++)
		temp[i] = temp[i] * m;
	
	//判断面是否可见
	judge(plane, temp);
	draw(temp, edge);
}

void rotate()
{
	Point temp[8], temp2[8];
	for (int i = 0; i < 8; i++)
	{
		temp[i] = point[i];
	}
	for (int i = 0; i < 8; i++)
	{
		temp2[i] = point[i];
	}
	Matrix m(1), t(1);

	//绕z轴旋转
	m.unit[0][0] = 0.707;
	m.unit[0][1] = 0.707;
	m.unit[1][0] = -0.707;
	m.unit[1][1] = 0.707;

	//绕x轴旋转
	t.unit[1][1] = 0.816;
	t.unit[1][2] = -0.577;
	t.unit[2][1] = 0.577;
	t.unit[2][2] = 0.816;
	m = m*t;
	t.reset();

	//投影到XOZ平面
	t.unit[1][1] = 0;
	m = m*t;
	t.reset();

	//放大
	t.unit[0][0] = 10;
	t.unit[1][1] = 10;
	t.unit[2][2] = 10;
	m = m*t;
	t.reset();


	Matrix r(1);
	r.unit[0][1] = cos(0.03);
	r.unit[0][1] = sin(0.03);
	r.unit[1][0] = -sin(0.03);
	r.unit[1][1] = cos(0.03);
	//m为正轴测投影变换矩阵，r为旋转矩阵

	while (1)
	{
		cleardevice();
		for (int i = 0; i < 8; i++)
			temp[i] = temp[i] * r;	//原始图形旋转
		for (int i = 0; i < 8; i++)
			temp2[i] = temp[i] * m; //作正轴测投影
		//判断可见性
		judge(plane, temp2);
		draw(temp2, edge);
		Sleep(50);
	}
}

int main()
{
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	setorigin(320, 240);//设置原点坐标为屏幕中心
	projection();//正轴测投影

	_getch();

	rotate();//正轴测投影旋转

	_getch();
	closegraph();
	system("pause");
	return 0;
}