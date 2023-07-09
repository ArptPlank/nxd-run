#include<iostream>
#include<easyx.h>
#include <conio.h>
#include<thread>
#include <future>
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //用来检测按键的点击事件
#include <windows.h>     
#include<mmsystem.h>//这是包含多媒体设备接口头文件
#pragma comment(lib,"winmm.lib")//加载静态库
#pragma comment( lib, "MSIMG32.LIB")
using namespace std;
int abs(int n) {
	if (n > 0) {
		return n;
	}
	else {
		return -n;
	}
}

void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //新版png
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}

class intout {
public:
	intout();
	int i;
	int distance = 0;
	void putint();
};

intout::intout(){
	settextcolor(GREEN);
	settextstyle(40, 0, "黑体");
	setbkmode(TRANSPARENT);
	i = 0;
}

void intout::putint() {
	char num[20];
	sprintf_s(num, "%d", i);
	settextcolor(GREEN);
	settextstyle(40, 0, "黑体");
	outtextxy(0, 0, "score:");
	outtextxy(120, 0, num);
	char num2[20];
	sprintf_s(num2, "%d", distance / 50);
	settextcolor(GREEN);
	settextstyle(40, 0, "黑体");
	outtextxy(200, 0, "distance:");
	outtextxy(375, 0, num2);
}

class chara {
public:
	int num = 0;
	int state;
	int x = 80;
	int y;
	int speed = 0;
	int speedtime = 0;
	void jump();
	void faster();
	void put(int w);
	int v;
	int life = 80;
	int lifewhite = 320;
	int attack;
	int atsize;
	int atx;
	int aty;
	int mp = 80;
	//int jpnum = 0;
	int nojpnum = 0;
	int lq = 0;
	void operatlife(int m ,int &state);
	IMAGE nxd[32];
	IMAGE nxdjump;
	chara();
};
chara::chara() {
	y = 240;
	v = 0;
	state = 1;
	loadimage(&nxd[0], "w_00.png");
	loadimage(&nxd[1], "w_01.png");
	loadimage(&nxd[2], "w_02.png");
	loadimage(&nxd[3], "w_03.png");
	loadimage(&nxd[4], "w_04.png");
	loadimage(&nxd[5], "w_05.png");
	loadimage(&nxd[6], "w_06.png");
	loadimage(&nxd[7], "w_07.png");
	loadimage(&nxd[8], "w_08.png");
	loadimage(&nxd[9], "w_09.png");
	loadimage(&nxd[10], "w_10.png");
	loadimage(&nxd[11], "w_11.png");
	loadimage(&nxd[12], "w_12.png");
	loadimage(&nxd[13], "w_13.png");
	loadimage(&nxd[14], "w_14.png");
	loadimage(&nxd[15], "w_15.png");
	loadimage(&nxd[16], "w_16.png");
	loadimage(&nxd[17], "w_17.png");
	loadimage(&nxd[18], "w_18.png");
	loadimage(&nxd[19], "w_19.png");
	loadimage(&nxd[20], "w_20.png");
	loadimage(&nxd[21], "w_21.png");
	loadimage(&nxd[22], "w_22.png");
	loadimage(&nxd[23], "w_23.png");
	loadimage(&nxd[24], "w_24.png");
	loadimage(&nxd[25], "w_25.png");
	loadimage(&nxd[26], "w_26.png");
	loadimage(&nxd[27], "w_27.png");
	loadimage(&nxd[28], "w_28.png");
	loadimage(&nxd[29], "w_29.png");
	loadimage(&nxd[30], "w_30.png");
	loadimage(&nxd[31], "w_31.png");
	loadimage(&nxdjump, "fly.png");
}

void chara::operatlife(int m,int &state) {
	life += m;
	attack = (m > 0 ? 0 : -m);
	atsize = -(m * 8);
	atx = rand() % 480 + 480;
	aty = rand() % 200 + 40;
	if (life <= 0) {
		state = 1;
	}
	//cout << atsize << endl;
}

void chara::jump() {
	if (y > 0) {
		v += 18;
		state = 2;
	}
}
void chara::faster() {
	if (mp > 0) {
		speed = 10;
		speedtime += 2;
		mp--;
		lq = 0;
		nojpnum = 0;
	}
}
void chara::put(int w) {
	if (state == 1) {
		transparentimage3(NULL, x, y, &nxd[num]);//透明png
		//putimage(x, y, &nxd[num]);
		if (num >= 31) {
			num = -1;
		}
		num += (1 + speed/10);
		if (num >= 32) {
			num = num % 32;
		}
	}
	else if (state == 2) {
		transparentimage3(NULL, x, y, &nxdjump);//透明png
		//putimage(x, y, &nxd[num]);
	}
	if (y != 240 || v != 0) {
		y = 240 - v;
		v -= 6;
		//cout << y << endl;
	}
	if (y == 240) {
		if (v != 0) {
			state = 1;
			v = 0;
		}
	}
	if (speedtime == 0) {
		speed = 0;
	}
	if (speedtime > 0) {
		speedtime--;
	}
	if (life != lifewhite) {
		if (life > lifewhite/4) {
			lifewhite = life * 4;
		}
		else {
			lifewhite -= 4;
		}
	}
	if (w == 0) {
		setlinecolor(GREEN);
		setlinestyle(0, 5);
		roundrect(520, 20, 840, 50, 25, 25);
		setlinestyle(0, 0);
		setfillcolor(RGB(250, 220, 220));
		setlinecolor(RGB(250, 220, 220));
		roundrect(520, 20, 520 + lifewhite, 50, 25, 25);
		setfillcolor(RED);
		setlinecolor(GREEN);
		fillroundrect(520, 20, 520 + life * 4, 50, 25, 25);
		settextstyle(30, 0, "黑体");
		outtextxy(530,18 , "HP");
		//cout << v << endl;
		//cout << speed << endl;
	}
	if (w == 0) {
		setlinecolor(GREEN);
		setlinestyle(0, 5);
		roundrect(520, 60, 840, 90, 25, 25);
		setlinestyle(0, 0);
		setfillcolor(BLUE);
		setlinecolor(BLUE);
		fillroundrect(520, 60, 520 + mp * 4, 90, 25, 25);
		settextstyle(30, 0, "黑体");
		outtextxy(530, 58, "MP");
		if (nojpnum > 50) {
			lq = 1;
			nojpnum = 0;
		}
		if (mp < 80 && lq == 1) {
			mp++;
		}
		if (mp == 80) {
			lq = 0;
		}
		//cout << v << endl;
		//cout << speed << endl;
	}
	if (w == 0 && attack > 0) {
		char num[20];
		sprintf_s(num, "%d", attack);
		//outtextxy(0, 0, "score:");
		settextcolor(RGB(255, 255 - atsize * 2, 255 - (255 / attack) * (atsize / 8)));
		settextstyle(atsize, 0, "华文行楷");
		/*int atx = rand() % 480 + 480;
		int aty = rand() % 200 + 40;*/
		outtextxy(atx, aty, num);
		outtextxy(atx - atsize / 2, aty, "-");
		atsize -= 2;
		if (atsize <= 0) {
			attack = 0;
			atsize = 0;
		}
	}
	//cout << v << endl;
	//cout << speed << endl;
}

class apple {
public:
	apple();
	void put();
	void refresh();
	IMAGE a;
	IMAGE glod;
	int x;
	int y;
	int state;
	int addlife = 15;
};

//图片大小36*36
apple::apple() {
	loadimage(&a, "apple.png");
	loadimage(&glod, "goldapple.png");
}

void apple::refresh() {
	y = rand() % 310;
	x = 960;
	if (rand() % 10 <= 1) {
		state = 1;
	}
	else {
		state = 0;
	}
}

void apple::put() {
	//transparentimage3(NULL, x, y, &a);//透明png
	////putimage(x, y, &a);
	if (state == 0) {
		transparentimage3(NULL, x, y, &a);//透明png
		//putimage(x, y, &a);
	}
	else {
		transparentimage3(NULL, x, y, &glod);//透明png
		//putimage(x, y, &a);
	}
}

class barrier {
public:
	int x;
	int y;
	bool exit;
	IMAGE yj;
	barrier();
	void put();
	void refresh();
};

barrier::barrier() {
	loadimage(&yj, "yj.png");
	exit = false;
	y = 200;
}

void barrier::put() {
	transparentimage3(NULL, x, y, &yj);
}

void barrier::refresh() {
	if (rand() % 10 < 3) {
		exit = true;
	}
	else {
		exit = false;
	}
	x = 960;
	y = 200;
}

class FloatingSpirit {
public:
	int x;
	int y;
	IMAGE spi;
	int agress = -rand() % 5 - 10;
	void put();
	void move();
	void refresh();
	int exit = 1;
	int state = 1;
	FloatingSpirit();
};

FloatingSpirit::FloatingSpirit() {
	loadimage(&spi, "飘浮灵.png");
}

void FloatingSpirit::put() {
	if (exit == 1) {
		transparentimage3(NULL, x, y, &spi);//透明png
	}
}

void FloatingSpirit::move() {
	if (y >= 270) {
		state = 0;
	}
	else if (y <= 0) {
		state = 1;
	}
	if (state == 0) {
		y -= 3;
	}
	else if (state == 1) {
		y += 3;
	}
}

void FloatingSpirit::refresh() {
	if (rand() % 10 < 4) {
		exit = 1;
	}
	else {
		exit = 0;
	}
	agress = -rand() % 5 - 10;
	state = rand() % 2;
	x = 960;
}

class slim {
public:
	int x;
	int y = 300;
	IMAGE sli;
	int agress = -rand() % 10 - 10;
	int exit;
	slim();
	void put();
	void refresh();
};

slim::slim() {
	loadimage(&sli, "shilaimu.png");
	exit = 0;
}

void slim::refresh() {
	x = 960;
	agress = -rand() % 10 - 10;
	if (rand() % 10 < 5) {
		exit = 1;
	}
	else {
		exit = 0;
	}
	
}

void slim::put() {
	transparentimage3(NULL, x, y, &sli);
}

//碰撞检测
class crash {
public:
	crash();
	bool iscrash(chara c, apple a);//apple  chara;
	bool iscrash(chara c, barrier yj);//chara  barrier;
	bool iscrash(chara c, slim s);
	bool iscrash(chara c, FloatingSpirit f);
};

crash::crash() {

}

bool crash::iscrash(chara c, apple a) {
	if (c.state == 1) {
		if (abs(c.x-a.x) < 95) {
			if (pow(c.y - a.y + 37, 2) + pow(75, 2) <= 7200) {
				return true;
			}
			return false;
		}
		return false;
	}
	else if (c.state == 2) {
		if (abs(c.x - a.x) < 110) {
			if (pow(c.y - a.y + 40, 2) + pow(100, 2) <= 12000) {
				return true;
			}
			return false;
		}
		return false;
	}
}

struct myline {
	int x;
	int g;
};

bool crash::iscrash(chara cha, barrier bar) {
	myline L[4];
	L[0].x = cha.x;
	L[0].g = 1;
	L[1].x = cha.x + 75;
	L[1].g = 1;
	L[2].x = bar.x;
	L[2].g = 2;
	L[3].x = bar.x + 80;
	L[3].g = 2;
	if (cha.y <= bar.y - 110) {
		return false;
	}
	else {
		myline temp;
		for (int i = 0; i < 4; i++)//注意数组下标从s[0]开始
		{
			for (int j = 0; j < 3 - i; j++)
			{
				if (L[j].x > L[j + 1].x)
				{
					temp = L[j];
					L[j] = L[j + 1];
					L[j + 1] = temp;
				}
			}
		}
		if (L[0].g == 1 && L[1].g == 1) {
			return false;
		}
		else if (L[0].g == 2 && L[1].g == 2) {
			return false;
		}
		return true;
	}
}

bool crash::iscrash(chara cha, slim s) {
	myline L[4];
	L[0].x = cha.x;
	L[0].g = 1;
	L[1].x = cha.x + 75;
	L[1].g = 1;
	L[2].x = s.x;
	L[2].g = 2;
	L[3].x = s.x + 57;
	L[3].g = 2;
	if (cha.y <= s.y - 110) {
		return false;
	}
	else {
		myline temp;
		for (int i = 0; i < 4; i++)//注意数组下标从s[0]开始
		{
			for (int j = 0; j < 3 - i; j++)
			{
				if (L[j].x > L[j + 1].x)
				{
					temp = L[j];
					L[j] = L[j + 1];
					L[j + 1] = temp;
				}
			}
		}
		if (L[0].g == 1 && L[1].g == 1) {
			return false;
		}
		else if (L[0].g == 2 && L[1].g == 2) {
			return false;
		}
		return true;
	}
}

bool crash::iscrash(chara c, FloatingSpirit f) {
	myline L[4];
	L[0].x = c.x;
	L[0].g = 1;
	L[1].x = c.x + 75;
	L[1].g = 1;
	L[2].x = f.x;
	L[2].g = 2;
	L[3].x = f.x + 126;
	L[3].g = 2;
	if (c.y <= f.y - 110) {
		return false;
	}
	else if (c.y >= f.y + 111) {
		return false;
	}
	else {
		myline temp;
		for (int i = 0; i < 4; i++)//注意数组下标从s[0]开始
		{
			for (int j = 0; j < 3 - i; j++)
			{
				if (L[j].x > L[j + 1].x)
				{
					temp = L[j];
					L[j] = L[j + 1];
					L[j + 1] = temp;
				}
			}
		}
		if (L[0].g == 1 && L[1].g == 1) {
			return false;
		}
		else if (L[0].g == 2 && L[1].g == 2) {
			return false;
		}
		return true;
	}
}

class ground {
public:
	ground();
	void putground();
	IMAGE ground1;
	int x = 0;
	int y = 0;
};

ground::ground() {
	loadimage(&ground1, "ground2.png");
	/*loadimage(&ground2, "ground_white.jpg");
	loadimage(&ground3, "ground_black.jpg");*/
}

void ground::putground() {
	/*putimage(x, y, &ground2, SRCAND);
	putimage(x, y, &ground3, SRCPAINT);*/
	transparentimage3(NULL, x, y, &ground1);//透明png
}
class bk {
public:
	bk();
	ground gif[8];
	apple app[6];
	barrier bar[3];
	slim slis[3];
	FloatingSpirit spi[3];
	void bkphoto();
	void Background_animation(int speed);
	IMAGE back;
};

bk::bk(){
	initgraph(960, 480);
	int num = 0;
	for (int i = 0; i < 8; i++) {
		gif[i].y = 350;
		gif[i].x = num + 116;
		num += 116;
	}
	int f = 400;
	for (int i = 0; i < 3; i++) {
		if (rand() % 10 < 3) {
			bar[i].exit = true;
			bar[i].x = f;
			bar[i].y = 200;
		}
		else {
			bar[i].exit = false;
			bar[i].x = f;
			bar[i].y = 200;
		}
		if (i == 0) {
			bar[i].exit = false;
		}
		f += 200;
	}
	for (int i = 0; i < 6; i++) {
		app[i].x = 240 * i;
		int q = 0;
		for (int j = 0; j < 3; j++) {
			if (bar[j].exit == true) {
				if (app[i].x >= bar[j].x - 50 && app[i].x <= bar[j].x + 50) {
					app[i].y = rand() % 110;
					q = 1;
					if (rand() % 10 <= 1) {
						app[i].state = 1;
					}
					else {
						app[i].state = 0;
					}
				}
			}
		}
		if (q == 1) {
			continue;
		}
		app[i].y = rand() % 310;
	}
	for (int i = 0; i < 3; i++) {
		if (rand() % 10 < 5) {
			slis[i].exit = 1;
		}
		slis[i].x = 300 + 200 * i;
	}
	for (int i = 0; i < 3; i++) {
		if (rand() % 10 < 4) {
			spi[i].exit = 1;
			spi[i].y = rand() % 350;
			if(rand()%2 == 0){
				spi[i].y *= 1;
			}
			else {
				spi[i].y *= -1;
			}
		}
		spi[i].x = 350 + 200 * i;
	}
}

void bk::bkphoto() {
	loadimage(&back, "background.png", 960, 480, true);
	//putimage(0, 0, &back);
}
void bk::Background_animation(int speed) {
	putimage(0, 0, &back);
	for (int i = 0; i < 8; i++) {
		gif[i].putground();
		gif[i].x -= 10 + speed;
		if (gif[i].x < -116) {
			gif[i].x = 844;
		}
		//cout << "我执行了" << endl;
	}
	//cout << gif[0].y;
}

class music {
public:
	void playbgm();
	void playsjmusic();
	void playdefeatmusic();
};

void music::playbgm() {
	mciSendString("open 背景音乐.mp3", 0, 0, 0);
	mciSendString("setaudio 背景音乐.mp3 volume to 100", 0, 0, 0);
	mciSendString("play 背景音乐.mp3 repeat", 0, 0, 0);//播放音乐
}

void music::playdefeatmusic() {
	switch (rand() % 3) {
	case 0:
		mciSendString("setaudio 失败.mp3 volume to 90", 0, 0, 0);
		mciSendString("close 失败.mp3", 0, 0, 0);
		mciSendString("open 失败.mp3", 0, 0, 0);
		mciSendString("play 失败.mp3", 0, 0, 0);//播放音乐
		break;
	case 1:
		mciSendString("setaudio 失败2.mp3 volume to 90", 0, 0, 0);
		mciSendString("close 失败2.mp3", 0, 0, 0);
		mciSendString("open 失败2.mp3", 0, 0, 0);
		mciSendString("play 失败2.mp3", 0, 0, 0);//播放音乐
		break;
	case 2:
		mciSendString("setaudio 失败3.mp3 volume to 90", 0, 0, 0);
		mciSendString("close 失败3.mp3", 0, 0, 0);
		mciSendString("open 失败3.mp3", 0, 0, 0);
		mciSendString("play 失败3.mp3", 0, 0, 0);//播放音乐
		break;
	}
}

void music::playsjmusic() {
	switch (rand() % 2) {
	case 0:
		mciSendString("setaudio 受击.mp3 volume to 90", 0, 0, 0);
		mciSendString("close 受击.mp3", 0, 0, 0);
		mciSendString("open 受击.mp3", 0, 0, 0);
		mciSendString("play 受击.mp3", 0, 0, 0);//播放音乐
		break;
	case 1:
		mciSendString("setaudio 受击2.mp3 volume to 90", 0, 0, 0);
		mciSendString("close 受击2.mp3", 0, 0, 0);
		mciSendString("open 受击2.mp3", 0, 0, 0);
		mciSendString("play 受击2.mp3", 0, 0, 0);//播放音乐
	}
}
struct clean {
	int* state;
	bk* b;
	intout* io;
	chara* cha;
	//FloatingSpirit* s;
};


void click(clean *cl) {
	int* state = cl->state;
	bk* b = cl->b;
	intout* io = cl->io;
	chara* cha = cl->cha;
	ExMessage msg;
	int u = 1;
	msg = getmessage();//获取鼠标消息
	if (msg.message == WM_LBUTTONDOWN) {
		if (msg.x >= 350 && msg.x <= 550 && msg.y >= 300 && msg.y <= 350) {
			*state = 0;
			int f = 500;
			(*cha).life = 80;
			for (int i = 0; i < 3; i++) {
				if (rand() % 10 < 5) {
					(*b).bar[i].exit = true;
					(*b).bar[i].x = f;
					(*b).bar[i].y = 200;
				}
				else {
					(*b).bar[i].exit = false;
					(*b).bar[i].x = f;
					(*b).bar[i].y = 160;
				}
				f += 200;
			}
			(*b).bar[1].exit = 0;
			for (int i = 0; i < 6; i++) {
				(*b).app[i].x = 240 * i;
				int q = 0;
				for (int j = 0; j < 3; j++) {
					if ((*b).bar[j].exit == true) {
						if ((*b).app[i].x >= (*b).bar[j].x - 50 && (*b).app[i].x <= (*b).bar[j].x + 50) {
							(*b).app[i].y = rand() % 110;
							q = 1;
						}
					}
				}
				if (q == 1) {
					continue;
				}
				(*b).app[i].y = rand() % 310;
			}
			for (int i = 0; i < 3; i++) {
				if (rand() % 10 < 4) {
					(*b).spi[i].exit = 1;
					(*b).spi[i].y = rand() % 350;
					if (rand() % 2 == 0) {
						(*b).spi[i].y *= 1;
					}
					else {
						(*b).spi[i].y *= -1;
					}
				}
				(*b).spi[i].x = 350 + 200 * i;
			}
			(*io).distance = 0;
			(*cha).mp = 80;
			(*cha).lq = 0;
			(*cha).nojpnum = 0;
			settextcolor(GREEN);
			settextstyle(40, 0, "黑体");
			setbkmode(TRANSPARENT);
			(*io).i = 0;
		}
	}
	return;
}
struct _cl {
	int* state;
};
void _click(clean* _cl) {
	int* state = _cl->state;
	ExMessage msg;
	int u = 1;
	msg = getmessage();//获取鼠标消息
	if (msg.message == WM_LBUTTONDOWN) {
		if (msg.x >= 350 && msg.x <= 550 && msg.y >= 300 && msg.y <= 350) {
			*state = 0;
		}
	}
	return;
}

bool landequ() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
	if (KEY_DOWN(VK_RBUTTON)) {//鼠标左键按下
		return true;
	}
}

bool landequ2() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hStdin, mode);
	if (KEY_DOWN(VK_LBUTTON)) {//鼠标左键按下
		return true;
	}
}

int main() {
	bk b;
	chara nxd;
	crash boom;
	intout io;
	music mis;
	//int distance = 0;
	b.bkphoto();
	srand(time(0));
	int state = 2;
	ExMessage msg;//定义一个消息变量
	//bool mouse = false;
	//MSG msg;
	int w = 0;
	//int d = 0;
	clean cl;
	cl.b = &b;
	cl.io = &io;
	cl.state = &state;
	cl.cha = &nxd;
	mis.playbgm();
	while (true) {
		BeginBatchDraw();
		io.distance += (10 + nxd.speed);
		if (state == 0) {
			b.Background_animation(nxd.speed);
			nxd.put(0);
			if (GetAsyncKeyState(VK_SPACE) || landequ2()) {
				nxd.jump();
				//cout << nxd.v << endl;
			}
			else {
				nxd.nojpnum++;
			}
			//cout << nxd.nojpnum << endl;
			if (GetAsyncKeyState(VK_SHIFT) || landequ()) {
				nxd.faster();
				//cout << nxd.v << endl;
			}
			for (int i = 0; i < 6; i++) {
				if (boom.iscrash(nxd, b.app[i])) {
					if (b.app[i].state == 1) {
						nxd.life += b.app[i].addlife;
					}
					if (nxd.life > 80) {
						io.i += (nxd.life - 80) / 5;
						nxd.life = 80;
					}
					b.app[i].refresh();
					io.i++;
				}
				b.app[i].put();
				b.app[i].x -= (10 + nxd.speed);
				if (b.app[i].x <= -36) {
					b.app[i].refresh();
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.bar[i].exit) {
					b.bar[i].put();
					b.bar[i].x -= (10 + nxd.speed);
				}
				else {
					b.bar[i].x -= (10 + nxd.speed);
				}
				if (b.bar[i].x <= -50) {
					b.bar[i].refresh();
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.bar[i].exit == 1) {
					if (boom.iscrash(nxd, b.bar[i])) {
						mis.playdefeatmusic();
						state = 1;
					}
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.slis[i].exit == 1) {
					if (boom.iscrash(nxd, b.slis[i])) {
						nxd.operatlife(b.slis[i].agress, state);
						mis.playsjmusic();
						b.slis[i].exit = 0;
					}
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.slis[i].exit) {
					b.slis[i].put();
					b.slis[i].x -= (10 + nxd.speed);
				}
				else {
					b.slis[i].x -= (10 + nxd.speed);
				}
				if (b.slis[i].x <= -80) {
					b.slis[i].refresh();
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.spi[i].exit == 1) {
					if (boom.iscrash(nxd, b.spi[i])) {
						nxd.operatlife(b.spi[i].agress, state);
						mis.playsjmusic();
						b.spi[i].exit = 0;
					}
				}
			}
			for (int i = 0; i < 3; i++) {
				if (b.spi[i].exit) {
					b.spi[i].move();
					b.spi[i].put();
					b.spi[i].x -= (10 + nxd.speed);
				}
				else {
					b.spi[i].move();
					b.spi[i].x -= (10 + nxd.speed);
				}
				if (b.spi[i].x <= -157) {
					b.spi[i].refresh();
				}
			}
			/*for (int i = 0; i < 3; i++) {
				cout << b.spi[i].x << ",";
			}
			cout << endl;*/
			//system("pause");
			io.putint();
			//w = 0;
		}
		else if (state == 1) {
			b.Background_animation(0);
			nxd.put(1);
			settextcolor(GREEN);
			settextstyle(140, 0, "黑体");
			setbkmode(TRANSPARENT);
			outtextxy(160, 0, "Game Over");
			char num[20];
			settextstyle(100, 0, "黑体");
			sprintf_s(num, "%d", io.i);
			outtextxy(230, 120, "score:");
			outtextxy(530, 120, num);
			setfillcolor(YELLOW);//设置填充颜色的；
			setlinecolor(GREEN);//设置线条颜色
			setlinestyle(200);
			roundrect(350, 300, 550, 350, 25, 25);
			fillroundrect(350, 300, 550, 350, 25, 25);
			settextstyle(50, 0, "黑体");
			outtextxy(350, 300, "重新开始");
			//peekmessage(&m, NULL, 0, 0, PM_REMOVE);//获取鼠标消息
			//std::future<ExMessage> msg = std::async(std::launch::async, click);
			/*thread ta(click, &state,&b,&io);
			ta.join();
			std::cout << state;*/
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)click, (LPVOID)&cl, NULL, NULL);
		}
		else if (state == 2) {
			b.Background_animation(0);
			nxd.put(1);
			settextcolor(GREEN);
			settextstyle(100, 0, "黑体");
			setbkmode(TRANSPARENT);
			outtextxy(220, 100, "纳西达跑酷");
			setfillcolor(YELLOW);//设置填充颜色的；
			setlinecolor(GREEN);//设置线条颜色
			setlinestyle(200);
			roundrect(350, 300, 550, 350, 25, 25);
			fillroundrect(350, 300, 550, 350, 25, 25);
			settextstyle(50, 0, "黑体");
			outtextxy(350, 300, "开始游戏");
			//peekmessage(&m, NULL, 0, 0, PM_REMOVE);//获取鼠标消息
			//std::future<ExMessage> msg = std::async(std::launch::async, click);
			/*thread ta(click, &state,&b,&io);
			ta.join();
			std::cout << state;*/
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_click, (LPVOID)&cl, NULL, NULL);
		}
		FlushBatchDraw();
		cleardevice();
		Sleep(40);
	}
	std::system("pause");
	return 0;
}