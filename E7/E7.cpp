///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E7 B样条曲线
//鼠标左键点击选择点，鼠标右键点击绘图，鼠标左键双击清空
//最后修改:2015-11-21
#include <graphics.h>
#include <iostream>
#include <vector>
struct Point
{
	float x;
	float y;
};
struct PointGroup
{
	std::vector<Point> p;
};
PointGroup pg;
void B_spline()
{
	float a0, a1, a2, a3, b0, b1, b2, b3;
	int k, x, y;
	if (pg.p.size() < 4)
		return;
	float i, t, dt, n = static_cast<float>(pg.p.size());
	setlinecolor(BLACK);
	dt = 1 / n;
	for (k = 0; k < static_cast<int>(pg.p.size()); k++)
	{
		if (k == 0)
			moveto(pg.p[k].x, pg.p[k].y);
		lineto(pg.p[k].x, pg.p[k].y);
	}
	setlinecolor(BLUE);
	for (k = 0; k < static_cast<int>(pg.p.size()) - 3; k++)
	{
		/*
		-1	 3	-3	 1
		 3	-6	 3	 0
		-3	 0	 3	 0
		 1	 4	 1	 0		
		*/
		a3 = (-1 * pg.p[k].x + 3 * pg.p[k + 1].x - 3 * pg.p[k + 2].x + 1 * pg.p[k + 3].x) / 6;
		a2 = ( 3 * pg.p[k].x - 6 * pg.p[k + 1].x + 3 * pg.p[k + 2].x + 0 * pg.p[k + 3].x) / 6;
		a1 = (-3 * pg.p[k].x - 0 * pg.p[k + 1].x + 3 * pg.p[k + 2].x + 0 * pg.p[k + 3].x) / 6;
		a0 = ( 1 * pg.p[k].x + 4 * pg.p[k + 1].x + 1 * pg.p[k + 2].x + 0 * pg.p[k + 3].x) / 6;
		
		b3 = (-1 * pg.p[k].y + 3 * pg.p[k + 1].y - 3 * pg.p[k + 2].y + 1 * pg.p[k + 3].y) / 6;
		b2 = ( 3 * pg.p[k].y - 6 * pg.p[k + 1].y + 3 * pg.p[k + 2].y + 0 * pg.p[k + 3].y) / 6;
		b1 = (-3 * pg.p[k].y - 0 * pg.p[k + 1].y + 3 * pg.p[k + 2].y + 0 * pg.p[k + 3].y) / 6;
		b0 = ( 1 * pg.p[k].y + 4 * pg.p[k + 1].y + 1 * pg.p[k + 2].y + 0 * pg.p[k + 3].y) / 6;
		
		
		for (i = 0; i < n; i += 0.1)
		{
			t = i * dt;
			x = a0 + a1 * t + a2 * t * t + a3 * t * t * t;
			y = b0 + b1 * t + b2 * t * t + b3 * t * t * t;
			if (i == 0)
				moveto(x, y);
			lineto(x, y);
		}
	}
}
int main()
{
	MOUSEMSG m;		// 定义鼠标消息
	initgraph(640, 480);
	setbkcolor(WHITE);
	setfillcolor(BLACK);
	cleardevice();	
	while (is_graph_open())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_CLOSE)
			break;
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			pg.p.push_back(Point{ static_cast<float>(m.x), static_cast<float>(m.y) });
			solidcircle(m.x, m.y, 2);
		}
		if (m.uMsg == WM_RBUTTONDOWN)
			B_spline();
		if (m.uMsg == WM_LBUTTONDBLCLK)
		{
			pg.p.clear();
			cleardevice();
		}
	}
	closegraph();
	return 0;
}