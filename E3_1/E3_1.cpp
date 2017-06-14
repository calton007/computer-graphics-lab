///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E3_1 线段裁剪
//先绘制一个20边的图形（蓝色），用鼠标点击两点确定裁剪的矩形框，矩形框内的线段平移并显示为绿色
//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-10-20
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
struct Window{
	float xl;
	float xr;
	float yt;
	float yb;
}win;
typedef struct Point{
	float x;
	float y;
	void set(float a, float b){ x = a; y = b; }
}Point;

int encode(Point p, int *code)
{
	int c = 0;
	if (p.x < win.xl)
		c |= LEFT;
	else if (p.x > win.xr)
		c |= RIGHT;
	if (p.y > win.yt)
		c |= TOP;
	else if (p.y < win.yb)
		c |= BOTTOM;
	*code = c;
	return 0;
}
int C_S_LineClip(Point p1, Point p2)
{
	int code1, code2, code;
	Point p;
	encode(p1, &code1);
	encode(p2, &code2);
	while (code1 != 0 || code2 != 0)
	{
		if ((code1 & code2) != 0)
			return 0;
		code = code1;
		if (code1 == 0)
			code = code2;
		if ((LEFT & code) != 0)
		{
			p.set(win.xl, p1.y + (p2.y - p1.y)*(win.xl - p1.x) / (p2.x - p1.x));			
		}
		else if ((RIGHT & code) != 0)
		{
			p.set(win.xr, p1.y + (p2.y - p1.y)*(win.xr - p1.x) / (p2.x - p1.x));
		}
		else if ((BOTTOM & code) != 0)
		{
			p.set(p1.x + (p2.x - p1.x)*(win.yb - p1.y) / (p2.y - p1.y), win.yb);			
		}
		else if ((TOP & code) != 0)
		{
			p.set(p1.x + (p2.x - p1.x)*(win.yt - p1.y) / (p2.y - p1.y), win.yt);
		}
		if (code == code1)
		{
			p1 = p;
			encode(p1, &code1);
		}
		else
		{
			p2 = p;
			encode(p2, &code2);
		}
	}
	setlinecolor(GREEN);	
	line(p1.x+310, p1.y, p2.x+310, p2.y);//平移并画线
	return 0;
}

int main()
{
	MOUSEMSG m1,m2;
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();	
	setlinecolor(BLUE);
	const int n = 20;
	int i, j;
	Point p[n];
	float t = 3.14159 * 2 / n;
	for (i = 0; i<n; i++)
	{
		p[i].set(150*cos(i*t) + 170,150*sin(i*t) + 170);
	}
	for (i = 0; i <= n - 2; i++) //画砖石图形
		for (j = i + 1; j <= n - 1; j++)
			line(p[i].x, p[i].y, p[j].x, p[j].y); 
	//获得矩形窗口两对角顶点坐标
	while (1)
	{
		m1 = GetMouseMsg();
		if (m1.uMsg == WM_LBUTTONDOWN)
			break;
	}
	while (1)
	{
		m2 = GetMouseMsg();
		if (m2.uMsg == WM_LBUTTONDOWN)
			break;
	}
	if (m1.x <= m2.x)
	{
		win.xl = m1.x;
		win.xr = m2.x;
	}
	else
	{
		win.xl = m2.x;
		win.xr = m1.x;
	}
	if (m1.y <= m2.y)
	{
		win.yt = m2.y;
		win.yb = m1.y;
	}
	else
	{
		win.yt = m1.y;
		win.yb = m2.y;
	}	
	rectangle(win.xl, win.yt, win.xr, win.yb);
	//删去矩形框内的线
	for (i = 0; i <= n - 2; i++)
		for (j = i + 1; j <= n - 1; j++)
			C_S_LineClip(p[i], p[j]);
	_getch();
	closegraph();
	return 0;
}