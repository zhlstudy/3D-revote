// 程序名称：三维旋转球
// 编译环境：Visual C++ 6.0 ~ 2022，EasyX_20210730
// 代码发布：2010-9-14
// 最后更新：2021-8-20
//

#include <graphics.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define MAXPOINT	5000
#define	PI			3.1415926536


// 定义三维点
struct POINT3D
{
	double x;
	double y;
	double z;
};

POINT3D p3d[MAXPOINT];		// 所有的三维点
double viewZ = 3;			// 视点 z 轴坐标


// 初始化三维点
void InitPoint()
{
	// 产生随机种子
	srand((unsigned)time(NULL));

	// 产生球体表面的随机点（根据球体面积与其外切圆柱面积的关系）
	double rxy, a;
	for (int i = 0; i < MAXPOINT; i++)
	{
		p3d[i].z = 2.0 * rand() / RAND_MAX - 1;	// 求随机 z 坐标
		rxy = sqrt(1 - p3d[i].z * p3d[i].z);	// 计算三维矢量在 xoy 平面的投影长度
		a = 2 * PI * rand() / RAND_MAX;			// 产生随机角度
		p3d[i].x = cos(a) * rxy;
		p3d[i].y = sin(a) * rxy;
	}
}


// 使三维点按 x 轴旋转指定角度
void RotateX(POINT3D& p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}


// 使三维点按 y 轴旋转指定角度
void RotateY(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}


// 使三维点按 z 轴旋转指定角度
void RotateZ(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}


// 将三维点投影到二维屏幕上（单点透视）
POINT Projection(POINT3D p)
{
	POINT p2d;
	p2d.x = (int)(p.x * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 320;
	p2d.y = (int)(p.y * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 240;
	return p2d;
}


// 精确延时函数(可以精确到 1ms，精度 ±1ms)
// by yangw80<yw80@qq.com>, 2011-5-4
void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
	//			Sleep(0);						// 更高精度、更高 CPU 占用率
}


// 实现直接操作显示缓冲区的设备对象
class Device
{
private:
	DWORD* m_pbuffer;
	int m_width;
	int m_height;

public:
	Device(int w, int h) : m_width(w), m_height(h)
	{
		initgraph(w, h);
		BeginBatchDraw();
		m_pbuffer = GetImageBuffer();
	}

	~Device()
	{
		EndBatchDraw();
		closegraph();
	}

	// 画点
	void putpixel(int x, int y, COLORREF c)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
			m_pbuffer[y * m_width + x] = BGR(c);
	}

	// 更新显示
	void flushdevice()
	{
		FlushBatchDraw();
	}
};


// 主函数
int main()
{
	Device d(640, 480);
	InitPoint();

	int c;
	POINT p2d;
	while (!_kbhit())
	{
		cleardevice();			// 清除屏幕

		for (int i = 0; i < MAXPOINT; i++)
		{
			// 使该点围绕三个坐标轴做旋转运动
			RotateX(p3d[i], PI / 800);
			RotateY(p3d[i], PI / 600);
			RotateZ(p3d[i], PI / 400);

			// 根据点的深度，产生相应灰度的颜色
			c = (int)(p3d[i].z * 100) + 155;

			// 投影该点到屏幕上
			p2d = Projection(p3d[i]);

			// 画点
			d.putpixel(p2d.x, p2d.y, RGB(c, c, c));
		}

		d.flushdevice();
		HpSleep(10);			// 延时 10 毫秒
	}

	return 0;
}