#include"gameover.h"
#include"datatype.h"
#include"snake.h"
#include"welcome.h"
#include<stdio.h>
#include<windows.h>


extern snake_s *head;
extern snake_para_s snake_para;

/*
 * 判断是否咬到自己
 */
int biteself()
{
	snake_s *self;   /* 定义self为蛇身上的一个节点 */
	self = head->next;   /* self作为除蛇头外的第一个节点，即蛇身 */
	while (self)
	{
		if ((self->x == head->x) && (self->y == head->y))
		{
			return 1;   /* 返回1，表示蛇头咬到自己 */
		}
		self = self->next; /* 遍历蛇身的每一个节点 */
	}
	return 0;
}


/*
 * 判断是否撞到墙
 */
void cantcrosswall()
{
	if ((head->x == 0) || (head->x == 60) || (head->y == 0) || (head->y == 30))
	{
		//gameoverstatus = 1;/*表示游戏结束时的第一种情况*/
		snake_para.gameoverstatus = 1;
		GameOver();
	}
}

/*
 * 游戏结束时的界面
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
				printf("∽");
			}
			else if((j == 5) ||(j == 24))
			{
				printf("∽");
			}
		}
	}
}


/*
 * 游戏结束时的提醒信息
 */
static void scoreremind()
{
	gotoxy(30,10);
	printf("您的得分是：%d",snake_para.score);
	if (snake_para.score > snake_para.heighscores)
	{
		heigscore_save();
		gotoxy(30,14);
		printf("恭喜您打破纪录，超出最高纪录%d分",(snake_para.score - snake_para.heighscores));
	}
	else
	{
		gotoxy(30,14);
		printf("继续加油哦！");
	}
}


/*
 * 游戏结束时的选项
 */
static void choose()
{
	int n = 0;
	gotoxy(30,25);
	color(11);
	printf("1---重新开始        2---回到主界面");
	gotoxy(30,27);
	printf("请选择：");
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
 * 游戏结束
 */
void GameOver()
{
	switch (snake_para.gameoverstatus)
	{
		case 1:        /* 撞墙结束 */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("撞到墙壁，GAMEOVER!");
			scoreremind();
			choose();
			break;
		case 2:		   /* 撞到自己结束 */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("咬到自己，GAMEOVER!");
			scoreremind();
			choose();
			break;
		case 3:		   /* ESC键结束 */
			OverInterface();
			gotoxy(40,8);
			color(10);
			printf("您已结束游戏！");
			scoreremind();
			choose();
			break;
	default:
		break;
	}
}