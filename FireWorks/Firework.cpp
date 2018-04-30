#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )

using namespace std;

#define NUM 13
#define W	GetSystemMetrics(SM_CXSCREEN)
#define H   GetSystemMetrics(SM_CYSCREEN)


struct FIRE
{
	int r;
	int max_r;
	int x, y;
	int cen_x, cen_y;
	int width, height;
	int xy[240][240];

	bool show;
	bool draw;
	DWORD t1, t2, dt;
}Fire[NUM];


struct JET
{
	int x, y;
	int hx, hy;
	int height;
	bool shoot;
	int xy[2][20][50];
	DWORD t1, t2, dt;
	IMAGE img[2];
	byte n : 1;
}Jet[NUM];

void Init(int);
void Load();
void Shoot();
void Chose(DWORD&);
void Style(DWORD&);
void Show(DWORD*);
int style_flag();


void main()
{
	initgraph(W, H);
	srand(time(0));

	HWND hwnd = GetHWnd();
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, W, H, SWP_SHOWWINDOW);
	setbkcolor(RGB(0, 0, 0));
	cleardevice();

	DWORD t1 = timeGetTime();
	DWORD st1 = timeGetTime();
	DWORD* pMem = GetImageBuffer();
	DWORD init_time = timeGetTime();

	for (int i = 0; i < NUM; i++)
	{
		Init(i);
	}
	Load();
	BeginBatchDraw();

	while (!_kbhit())
	{
		Sleep(10);
		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % W;
				int py1 = rand() % H;

				if (py1 < H - 1)
					pMem[py1 * W + px1] = pMem[py1 * W + px1 + 1] = BLACK;
			}
		}
		Chose(t1);
		Shoot();
		Show(pMem);
		if (timeGetTime() - init_time > 20000)
		{
			Style(st1);
			init_time = timeGetTime();
		}
		FlushBatchDraw();
	}
}


void Init(int i)
{
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
	int x[13] = { 120, 120, 110, 117, 110, 93,  102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85,  118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	Fire[i].x = 0;
	Fire[i].y = 0;
	Fire[i].width = 240;
	Fire[i].height = 240;
	Fire[i].max_r = r[i];
	Fire[i].cen_x = x[i];
	Fire[i].cen_y = y[i];
	Fire[i].show = false;
	Fire[i].dt = 5;
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;

	Jet[i].x = -240;
	Jet[i].y = -240;
	Jet[i].hx = -240;
	Jet[i].hy = -240;
	Jet[i].height = 0;
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;
	Jet[i].n = 0;
	Jet[i].shoot = false;
}

void Load()
{
	IMAGE fm, gm;
	loadimage(&fm, "./fire/flower.jpg", 3120, 240);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		SetWorkingImage(&gm);

		for (int a = 0; a < 240; a++)
			for (int b = 0; b < 240; b++)
				Fire[i].xy[a][b] = getpixel(a, b);
	}

	IMAGE sm;
	loadimage(&sm, "./fire/shoot.jpg", 200, 50);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&sm);
		int n = rand() % 5;
		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);
	}
	SetWorkingImage();
}


void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();
	if (t2 - t1 > 100)
	{
		int n = rand() % 20;
		if (n < 13 && Jet[n].shoot == false && Fire[n].show == false)
		{
			Jet[n].x = rand() % W;
			Jet[n].y = rand() % 100 + 650;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 390 + 10;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n], SRCINVERT);

			char c1[50], c2[30], c3[30];
			sprintf_s(c1, "open ./fire/shoot.mp3 alias s%d", n);
			sprintf_s(c2, "play s%d", n);
			sprintf_s(c3, "close n%d", n);
			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		t1 = t2;
	}
}


void Shoot()
{
	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			if (Jet[i].y <= Jet[i].hy)
			{
				char c1[50], c2[30], c3[30];
				sprintf_s(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf_s(c2, "play n%d", i);
				sprintf_s(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);
				Fire[i].x = Jet[i].hx + 10;
				Fire[i].y = Jet[i].hy;
				Fire[i].show = true;
				Jet[i].shoot = false;
			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}

void Style(DWORD& st1)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 > 1)
	{
		int x[14] = { 40, 55, 72, 80, 75, 60, 40, 20, 5, 0, 8, 25, 40 };
		int y[14] = { 20, 3, 10, 25, 40, 60, 80, 60, 40, 25, 10, 3, 20 };
		for (int i = 0; i < NUM; i++)
		{
			cleardevice();
			Jet[i].x = (x[i] - 40) * (H - 200) / 80 * 1.1 + W / 2;
			Jet[i].y = y[i] * (H - 200) / 80 + H;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * (H - 200) / 80 + 80;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;

			char c1[50], c2[30], c3[30];
			sprintf_s(c1, "open ./fire/shoot.mp3 alias s%d", i);
			sprintf_s(c2, "play s%d", i);
			sprintf_s(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		st1 = st2;
	}
}


void Show(DWORD* pMem)
{
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++)
	{
		Fire[i].t2 = timeGetTime();

		if (Fire[i].t2 - Fire[i].t1 > Fire[i].dt && Fire[i].show == true)
		{
			if (Fire[i].r < Fire[i].max_r)
			{
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r / 10];
				Fire[i].draw = true;
			}

			if (Fire[i].r >= Fire[i].max_r - 1)
			{
				Fire[i].draw = false;
				Init(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}

		if (Fire[i].draw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height)
				{
					int b = Fire[i].xy[x1][y1] & 0xff;
					int g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (Fire[i].xy[x1][y1] >> 16);

					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					if (r > 0x30 && g > 0x30 && b > 0x30 && xx > 0 && xx < W && yy > 0 && yy < H)
						pMem[yy * W + xx] = BGR(Fire[i].xy[x1][y1]);
				}
			}
			Fire[i].draw = false;
		}
	}
}
