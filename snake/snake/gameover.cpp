#include"gameover.h"
#include"datatype.h"
#include"snake.h"
#include"welcome.h"
#include<stdio.h>
#include<windows.h>


extern snake_s *head;
extern snake_para_s snake_para;

/*
 * �ж��Ƿ�ҧ���Լ�
 */
int biteself()
{
	snake_s *self;   /* ����selfΪ�����ϵ�һ���ڵ� */
	self = head->next;   /* self��Ϊ����ͷ��ĵ�һ���ڵ㣬������ */
	while (self)
	{
		if ((self->x == head->x) && (self->y == head->y))
		{
			return 1;   /* ����1����ʾ��ͷҧ���Լ� */
		}
		self = self->next; /* ���������ÿһ���ڵ� */
	}
	return 0;
}


/*
 * �ж��Ƿ�ײ��ǽ
 */
void cantcrosswall()
{
	if ((head->x == 0) || (head->x == 60) || (head->y == 0) || (head->y == 30))
	{
		//gameoverstatus = 1;/*��ʾ��Ϸ����ʱ�ĵ�һ�����*/
		snake_para.gameoverstatus = 1;
		GameOver();
	}
}

/*
 * ��Ϸ����ʱ�Ľ���
 */
static void OverInterface()
{
	system("cls");
	color(7);
	for (int i = 20; i < 80; i += 2)
	{
		for (int j = 5; j < 25; j++)
		{
			gotoxy(i,j);
			if ((i == 20) || (i == 78))
			{
				printf("��");
			}
			else if((j == 5) ||(j == 24))
			{
				printf("��");
			}
		}
	}
}


/*
 * ��Ϸ����ʱ��������Ϣ
 */
static void scoreremind()
{
	gotoxy(30,10);
	printf("���ĵ÷��ǣ�%d",snake_para.score);
	if (snake_para.score > snake_para.heighscores)
	{
		heigscore_save();
		gotoxy(30,14);
		printf("��ϲ�����Ƽ�¼��������߼�¼%d��",(snake_para.score - snake_para.heighscores));
	}
	else
	{
		gotoxy(30,14);
		printf("��������Ŷ��");
	}
}


/*
 * ��Ϸ����ʱ��ѡ��
 */
static void choose()
{
	int n = 0;
	gotoxy(30,25);
	color(11);
	printf("1---���¿�ʼ        2---�ص�������");
	gotoxy(30,27);
	printf("��ѡ��");
	scanf("%d",&n);
	switch (n)
	{
		case 1:
			system("cls");
			creatgamemap();
			initsnake();
			creatfood();
			keyboardcontrol();
			break;
		case 2:
			system("cls");
			PrintSnake();
			WelcomeToGame();
			break;
	}
}

/*
 * ��Ϸ����
 */
void GameOver()
{
	switch (snake_para.gameoverstatus)
	{
		case 1:        /* ײǽ���� */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("ײ��ǽ�ڣ�GAMEOVER!");
			scoreremind();
			choose();
			break;
		case 2:		   /* ײ���Լ����� */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("ҧ���Լ���GAMEOVER!");
			scoreremind();
			choose();
			break;
		case 3:		   /* ESC������ */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("���ѽ�����Ϸ��");
			scoreremind();
			choose();
			break;
	default:
		break;
	}
}