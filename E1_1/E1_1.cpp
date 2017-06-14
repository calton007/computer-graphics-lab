///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E1_1 一笔画
//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-09-22
#include <graphics.h>   
#include <conio.h>
int main()
{
	int pointAx = 160, pointAy = 240, pointBx = 480, pointBy = 240, x;	//A,B为两个定点
	initgraph(640, 480);
	setbkcolor(BLUE);
	cleardevice();
	moveto(pointAx, pointAy);
	for (x = 0; x <= 320; x += 20)
	{
		lineto(pointAx + x, pointAy - 120);
		lineto(pointBx, pointBy);
		lineto(pointBx - x, pointBy + 120);
		lineto(pointAx, pointAy);
		//line(pointAx, pointAy, pointAx + x, pointAy - 120);
		//line(pointAx + x, pointAy - 120, pointBx, pointBy);
		//line(pointBx, pointBy, pointBx - x, pointBy + 120);
		//line(pointBx - x, pointBy + 120, pointAx, pointAy);
	}
	_getch();
	closegraph();
	return 0;
}