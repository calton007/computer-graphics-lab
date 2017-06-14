/*
#include <graphics.h>      
#include <conio.h>
#include <math.h>
#include <stdio.h>
int main()
{
	int *x, *y, i, j, n, r = 150, x0 = 200, y0 = 200;
	float t;
	printf("请输入多边形边数:");
	scanf_s("%d", &n);	//n为多边形的边数
	x = new int[n];
	y = new int[n];
	initgraph(640, 480);
	circle(200, 200, 150);
	t = 3.14159 * 2 / n;
	for (i = 0; i<n; i++)
	{
		x[i] = r*cos(i*t) + x0;
		y[i] = r*sin(i*t) + y0;
	}
	for (i = 0; i <= n - 2; i++)
		for (j = i + 1; j <= n - 1; j++)
			line(x[i], y[i], x[j], y[j]);
	_getch();
	closegraph();
	delete[] x;
	delete[] y;
	return 0;
}
*/
