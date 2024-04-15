#include <stdio.h>
#include<conio.h>
#include <string>
//ͼ�ο�
#include<graphics.h>
#include<time.h>

enum My
{
	WIDTH = 480, //������Ⱥ͸߶�
	HEIGHT = 700,
	BULLET_NUM = 50,//����ӵ�����
	ENEMY_NUM = 10,
	BIG,
	SMALL
};
struct Plance//�ɻ��ṹ
{
	int x;
	int y;
	bool live;//�Ƿ���
	int width;
	int height;
	int hp;
	int type;//�л����� big��small

}player, bull[BULLET_NUM], enemy[ENEMY_NUM];

auto kill_count = 0;

//��ͼƬ���ؽ����� int a=10;
IMAGE bk;
//�������ͼƬ role player mover
IMAGE img_role[2];
//�����ӵ�
IMAGE img_bull[2];
//����л�ͼƬ
IMAGE img_enemy[2][2];
void loadImg()
{
	//����ͼƬ 480 * 700
	loadimage(&bk, "./images/background.png");


	//�������ͼƬ
	loadimage(&img_role[0], "./images/me1.png");
	loadimage(&img_role[1], "./images/me2.png");
	//�����ӵ�ͼƬ
	loadimage(&img_bull[0], "./images/bullet1.png");
	loadimage(&img_bull[1], "./images/bullet2.png");
	//���صл���ͼƬ
	loadimage(&img_enemy[0][0], "./images/enemy_1.png");
	loadimage(&img_enemy[0][1], "./images/enemy_2.png");
	loadimage(&img_enemy[1][0], "./images/planeNormal_1.png");
	loadimage(&img_enemy[1][1], "./images/planeNormal_2.png");
}

bool PointInRect(int x, int y, RECT& r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}

// һ����ʼ����
void Welcome()
{
	LPCTSTR title = _T("�ɻ���ս");
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");

	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("����"));
	settextcolor(BLACK);
	outtextxy(480 / 2 - textwidth(title) / 2, 700 / 5, title);

	settextstyle(40, 0, _T("����"));
	tplayr.left = 480 / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = 700 / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = 480 / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = 700 / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top, tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (PointInRect(mess.x, mess.y, tplayr))
			{
				return;
			}
			else if (PointInRect(mess.x, mess.y, texitr))
			{
				exit(0);
			}
		}
	}
}
void enemyHp(int i)
{
	int flag = rand() % 10;
	if (flag >= 0 && flag <= 2)//0~9
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 69;
		enemy[i].height = 99;
	}
	else
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
}
void gameInit()
{
	loadImg();
	player.x = WIDTH / 2;
	player.y = HEIGHT / 2;
	player.live = true;
	//��ʼ���ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
		enemyHp(i);
	}
}
//��Ϸ�Ļ��ƺ���
void gameDraw()
{
	//�ѱ���ͼƬ����������
	putimage(0, 0, &bk);

	putimage(player.x, player.y, &img_role[0], NOTSRCERASE);
	putimage(player.x, player.y, &img_role[1], NOTSRCERASE);
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)
		{
			putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);
		}
	}
	//��ͼ�л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == BIG)
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], NOTSRCERASE);
			}
		}
	}

	std::string kill_count_ws = std::to_string(kill_count);
	const char* kill_count_ws_data = kill_count_ws.data();
	outtextxy(0, 20, kill_count_ws_data);

	
}
void createBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bull[i].live)
		{

			bull[i].x = player.x + 51;
			bull[i].y = player.y;
			bull[i].live = true;
			break;
		}
	}
}
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bull[i].live)
		{
			bull[i].y -= speed;
			if (bull[i].y < 0)
			{
				bull[i].live = false;
			}
		}
	}
}
bool Timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}

//��ɫ�ƶ���������
void playerMove(int speed)
{
	ExMessage mess;
	if (peekmessage(&mess, EM_MOUSE))
	{
		player.x = mess.x - 51;
		player.y = mess.y - 63;

	}
	if (Timer(200, 1))
	{
		//����һ���ӵ�
		createBullet();
	}
}
//�л����ƶ�
void enemyMove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].live = false;
			}
		}
	}
}
//�����л�
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH - 40);
			enemy[i].y = 0;
			enemyHp(i);
			printf("pos��%d,%d��%d %d\n", enemy[i].x, enemy[i].y, enemy[i].live, enemy[i].hp);
			break;
		}
	}
}


void playPlance()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
			continue;
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bull[k].live)
				continue;
			if (bull[k].x > enemy[i].x
				&& bull[k].x < enemy[i].x + enemy[i].width
				&& bull[k].y > enemy[i].y
				&& bull[k].y < enemy[i].y + enemy[i].height)
			{
				bull[k].live = false;
				enemy[i].hp--;
			}
		}
		if (enemy[i].hp <= 0)
		{
			enemy[i].live = false;
			kill_count++;
		}
	}
}
void showenemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		printf("pos��%d,%d��%d %d\n", enemy[i].x, enemy[i].y, enemy[i].live, enemy[i].hp);
	}
}
int main()
{
	//����һ������
	initgraph(WIDTH, HEIGHT, SHOWCONSOLE);
	gameInit();
	//˫�����ͼ
	BeginBatchDraw();
	Welcome();
	while (1)
	{
		
		gameDraw();
		FlushBatchDraw();
		playerMove(1);
		BulletMove(1);

		if (Timer(500, 0))
		{
			createEnemy();
		}
		if (Timer(10, 2))
		{
			enemyMove(1);
		}


		playPlance();
		//printf("%d\n,i");
	}
	EndBatchDraw();

	return 0;
}