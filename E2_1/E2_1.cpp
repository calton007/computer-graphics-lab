///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E2_1 画直线  鼠标点击画
//最后修改:2015-09-29
#include <graphics.h>      
#include <conio.h>
#include <stdio.h>
void drawline(int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true)
	{
		putpixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}
int main()
{
	// 初始化图形窗口
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLUE);
	MOUSEMSG m;		// 定义鼠标消息
	int x0, y0, x1, y1, flag = 1;
	while(is_graph_open())
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		if (m.uMsg == WM_CLOSE)
			break;
		//记录起点坐标
		if (flag==1 && m.uMsg == WM_LBUTTONDOWN)
		{
			x0 = m.x;
			y0 = m.y;
			flag = 2; 
			rectangle(x0 - 3, y0 - 3, x0 + 3, y0 + 3);
		}
		//记录终点坐标
		else if (flag == 2 && m.uMsg == WM_LBUTTONDOWN)
		{
			x1 = m.x;
			y1 = m.y;
			flag = 0;
			rectangle(x1 - 3, y1 - 3, x1 + 3, y1 + 3);			
		}
		if (flag == 0)
		{
			//交换起点和终点
			if (x0 > x1)
			{

				int t;
				t = x0;
				x0 = x1;
				x1 = t;
				t = y0;
				y0 = y1;
				y1 = t;
			}
			drawline(x0, y0, x1, y1, RED);
			flag = 1;
		}
	}
	

	// 关闭图形窗口
	closegraph();	
	return 0;
}