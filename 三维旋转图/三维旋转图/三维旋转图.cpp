// �������ƣ���ά��ת��
// ���뻷����Visual C++ 6.0 ~ 2022��EasyX_20210730
// ���뷢����2010-9-14
// �����£�2021-8-20
//

#include <graphics.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define MAXPOINT	5000
#define	PI			3.1415926536


// ������ά��
struct POINT3D
{
	double x;
	double y;
	double z;
};

POINT3D p3d[MAXPOINT];		// ���е���ά��
double viewZ = 3;			// �ӵ� z ������


// ��ʼ����ά��
void InitPoint()
{
	// �����������
	srand((unsigned)time(NULL));

	// ����������������㣨�������������������Բ������Ĺ�ϵ��
	double rxy, a;
	for (int i = 0; i < MAXPOINT; i++)
	{
		p3d[i].z = 2.0 * rand() / RAND_MAX - 1;	// ����� z ����
		rxy = sqrt(1 - p3d[i].z * p3d[i].z);	// ������άʸ���� xoy ƽ���ͶӰ����
		a = 2 * PI * rand() / RAND_MAX;			// ��������Ƕ�
		p3d[i].x = cos(a) * rxy;
		p3d[i].y = sin(a) * rxy;
	}
}


// ʹ��ά�㰴 x ����תָ���Ƕ�
void RotateX(POINT3D& p, double angle)
{
	double y = p.y;
	p.y = p.y * cos(angle) + p.z * sin(-angle);
	p.z = y * sin(angle) + p.z * cos(angle);
}


// ʹ��ά�㰴 y ����תָ���Ƕ�
void RotateY(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.z * sin(-angle);
	p.z = x * sin(angle) + p.z * cos(angle);
}


// ʹ��ά�㰴 z ����תָ���Ƕ�
void RotateZ(POINT3D& p, double angle)
{
	double x = p.x;
	p.x = p.x * cos(angle) + p.y * sin(-angle);
	p.y = x * sin(angle) + p.y * cos(angle);
}


// ����ά��ͶӰ����ά��Ļ�ϣ�����͸�ӣ�
POINT Projection(POINT3D p)
{
	POINT p2d;
	p2d.x = (int)(p.x * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 320;
	p2d.y = (int)(p.y * (viewZ / (viewZ - p.z)) * 200 + 0.5) + 240;
	return p2d;
}


// ��ȷ��ʱ����(���Ծ�ȷ�� 1ms������ ��1ms)
// by yangw80<yw80@qq.com>, 2011-5-4
void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// ��̬��������¼��һ�� tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// ���� tick

	if (clock() > oldclock)					// ����Ѿ���ʱ��������ʱ
		oldclock = clock();
	else
		while (clock() < oldclock)			// ��ʱ
			Sleep(1);						// �ͷ� CPU ����Ȩ������ CPU ռ����
	//			Sleep(0);						// ���߾��ȡ����� CPU ռ����
}


// ʵ��ֱ�Ӳ�����ʾ���������豸����
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

	// ����
	void putpixel(int x, int y, COLORREF c)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
			m_pbuffer[y * m_width + x] = BGR(c);
	}

	// ������ʾ
	void flushdevice()
	{
		FlushBatchDraw();
	}
};


// ������
int main()
{
	Device d(640, 480);
	InitPoint();

	int c;
	POINT p2d;
	while (!_kbhit())
	{
		cleardevice();			// �����Ļ

		for (int i = 0; i < MAXPOINT; i++)
		{
			// ʹ�õ�Χ����������������ת�˶�
			RotateX(p3d[i], PI / 800);
			RotateY(p3d[i], PI / 600);
			RotateZ(p3d[i], PI / 400);

			// ���ݵ����ȣ�������Ӧ�Ҷȵ���ɫ
			c = (int)(p3d[i].z * 100) + 155;

			// ͶӰ�õ㵽��Ļ��
			p2d = Projection(p3d[i]);

			// ����
			d.putpixel(p2d.x, p2d.y, RGB(c, c, c));
		}

		d.flushdevice();
		HpSleep(10);			// ��ʱ 10 ����
	}

	return 0;
}