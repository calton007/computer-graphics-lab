///////////////////////////////////////////////////////////////////////////////////////
//程序名称：E1_3 绘制三角形并填充
//编译环境：Microsoft Visual Studio Ultimate 2013 EasyX_2014冬至版
//作者：李观波<1206020120>
//最后修改:2015-09-22
#include <graphics.h>      
#include <conio.h>
int main()
{
	POINT pts1[] = { 100, 0, 128, 0, 48, 80, 150, 80, 170, 100, 0, 100, 100, 0 };
	POINT pts2[] = { 0, 100, 20, 120, 220, 120, 128, 28, 113, 43, 170, 100, 0, 100 };
	POINT pts3[] = { 76, 80, 128, 28, 220, 120, 228, 100, 128, 0, 48, 80, 76, 80 };
	initgraph(640, 480);
	setbkcolor(WHITE);		//设置背景色为白色
	cleardevice();			//背景色刷新屏幕
	setorigin(100, 100);	//设置原点坐标
	setaspectratio(2, 2);	//设置缩放因子
	setlinecolor(RED);		//边框颜色设置成红色
	setfillcolor(RED);		//填充颜色设置成红色	
	fillpolygon(pts1, 7);
	setfillcolor(YELLOW);	//填充颜色设置成黄色
	fillpolygon(pts2, 7);
	setfillcolor(BLUE);		//填充颜色设置成蓝色
	fillpolygon(pts3, 7);
	_getch();
	closegraph();
	return 0;
}