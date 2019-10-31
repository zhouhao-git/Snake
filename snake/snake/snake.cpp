#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<conio.h>	
#include<stdlib.h>
#include"datatype.h"    
#include"welcome.h"
#include"gameover.h"
#include"snake.h"


snake_para_s snake_para;
snake_s *head, *food;
snake_s *q; /* 遍历蛇时用到的指针 */


void SnakeParaInit()
{
	snake_para.add_score = 10;
	snake_para.gameoverstatus = 0;
	snake_para.heighscores = 0;	
	snake_para.sleeptime = 200;
	snake_para.run_status = DOWN;
	snake_para.score = 0;
}

/*
 * 设置控制台窗体大小
 */
void windowsize(int w,int h)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = {w, h};
	SetConsoleScreenBufferSize(hOut,size);
	SMALL_RECT rc = {1, 1, w, h};
	SetConsoleWindowInfo(hOut,true,&rc);
	return;
}

int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);  /*更改文字颜色*/
	return 0;
}

/*
 * 设置光标的位置
 */
void gotoxy(int x, int y)
{
	COORD cursor;
	cursor.X = x;
	cursor.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cursor); /*定位光标位置*/
}


/*
 * 创建游戏地图
 */
void creatgamemap()
{
	for (int i = 0; i < 61; i+=2)  /*打印上下边框*/
	{
		gotoxy(i, 0);
		color(19);
		printf("□");
		gotoxy(i,30);
		printf("□");
	}
	for (int j = 0; j < 31; j++)  /*打印左右边框*/
	{
		gotoxy(0,j);
		color(18);
		printf("□");
		gotoxy(60,j);
		printf("□");
	}

	for (int i = 2; i < 60; i+=2)      /*绘制游戏中间区*/
	{
		for (int j = 1; j < 30; j++)
		{
			gotoxy(i,j);
			color(3);
			printf("■\n\n");
		}
	}

	for (int i = 0; i < 31; i++)/*绘制分割线*/
	{
		gotoxy(62,i);
		color(6);
		printf("┆");
	}
}

/*
 * 绘制游戏右侧得分和提示界面
 */
void scoreandtips()
{
	heigscore_read();

	for (int i = 65; i < 100; i+=2)
	{
		gotoxy(i,10);
		color(1);
		printf("═");
		gotoxy(i,30);
		printf("═");
	}
	for (int j = 11; j < 30; j++)
	{
		gotoxy(64,j);
		color(1);
		printf("║");
		gotoxy(100,j);
		printf("║");
	}

	gotoxy(75,2);
	color(11);
	printf("☆最高分记录：%d",snake_para.heighscores);
	gotoxy(80,6);
	color(14);
	printf("得分：%d\n",snake_para.score);

	color(13);
	gotoxy(80,13);
	printf("小提示！");
	gotoxy(66,17);
	printf("每个食物得分：10分");
	gotoxy(66,19);
	printf("用↑↓← →改变蛇的移动方向");
	gotoxy(66,21);
	printf("不能撞墙，不能咬到自己");
	gotoxy(66,23);
	printf("F1加速   F2减速");
	gotoxy(66,25);
	printf("Space：暂停游戏");
	gotoxy(66,27);
	printf("Esc：退出游戏");
}




/*
 * 绘制蛇身
 */
void initsnake()
{
	snake_s *tail;
	tail = (snake_s*)malloc(sizeof(snake_s));  //蛇尾
	if (!tail)
	{
		MessageBoxA(NULL,"蛇尾内存分配失败","警告",MB_OK);
		return;
	}
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (int i = 1; i <= 4; i++)
	{
		head = (snake_s*)malloc(sizeof(snake_s));  //舌头
		head->next = tail;
		head->x = 24 + i * 2;
		head->y = 5;
		tail = head;
	}

	while (tail)
	{
		//if (snake_flag == 1)
		//{
		//	gotoxy(tail->x,tail->y);
		//	color(14);
		//	printf("∝");
		//}
		//else
		{
			gotoxy(tail->x,tail->y);
			color(14);
			printf("☆");
		}
		tail = tail->next;
	}
}


/* 
 * 绘制随机事物(1、出现在网格中间，不能出现在网格线上 2、食物不能与蛇身重合)
 */
void creatfood()
{
	snake_s *food_info;

	srand((unsigned)time(NULL)); 
	food_info = (snake_s*)malloc(sizeof(snake_s));
	if (!food_info)
	{
		MessageBoxA(NULL,"食物内存分配失败","警告",MB_OK);
		return;
	}
	while ((food_info->x % 2) != 0)      /*保证食物落在网格中间而不是边线上,方格中间的坐标值为奇数*/
	{
		food_info->x = rand() % 57 + 2;   
	}	
	food_info->y = rand() % 29 + 1;      
	q = head;
	while (q->next != NULL)      /*每产生一个食物就与蛇身进行比较*/
	{
		if ((q->x == food_info->x) && (q->y == food_info->y)) /* 判断食物是否与蛇身重合 */
		{
			free(food_info);           
			creatfood();
		}
		q = q->next;
	}
	gotoxy(food_info->x,food_info->y);
	food = food_info;
	color(12);
	printf("●");
}


/**************游戏逻辑部分设计*******************/
/*游戏结束判断：
 * 1、判断是否咬到自己 2、撞墙  */


/* 
 * 加速，迟到食物自动提速或F1加速
 */
void speedup()
{
	if (snake_para.sleeptime > 50)
	{
		//sleeptime -= 10;
		//add_score +=2;    /* 每吃一个食物，加分值增2*/
		snake_para.sleeptime -= 10;
	}
}

/*
 * 减速，F2减速
 */
void speeddown()
{
	if(snake_para.sleeptime < 350)
	{
		snake_para.sleeptime += 30;
		//add_score -= 2;
		//if (sleeptime == 350)
		//{
		//	add_score = 1;      /*由于速度为350时，加分减到0，所以要保证最小的加分*/
		//}
	}
}

/*
 * 不按键时蛇的方向控制
 */
void SnakeMove()
{
	snake_s *nexthead; /* 存放新的蛇头节点的中间变量 */

	nexthead = (snake_s*)malloc(sizeof(snake_s));
	if (!nexthead)
	{
		MessageBoxA(NULL,"中间节点分配失败","警告",MB_OK);
		return;
	}
	if (snake_para.run_status == UP)          
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1; /* 向上移动是，纵坐标减1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//如果下一位置上有食物
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			snake_para.score += snake_para.add_score;
			speedup();
			creatfood();
		}
		else
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			//经过上面的遍历，在前方没有食物的情况下q已经指向蛇尾，需要将移动前的蛇尾节点释放并以■填充
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}
	if (snake_para.run_status == DOWN)          
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1; 
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//如果下一位置上有食物
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			snake_para.score += snake_para.add_score;
			speedup();
			creatfood();
		}
		else
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			//经过上面的遍历，在前方没有食物的情况下q已经指向蛇尾，需要将移动前的蛇尾节点释放并以■填充
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}
	if (snake_para.run_status == LEFT)          
	{
		nexthead->x = head->x - 2;
		nexthead->y = head->y; /* 向上移动是，纵坐标减1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//如果下一位置上有食物
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			snake_para.score += snake_para.add_score;
			speedup();
			creatfood();
		}
		else
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			//经过上面的遍历，在前方没有食物的情况下q已经指向蛇尾，需要将移动前的蛇尾节点释放并以■填充
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}
	if (snake_para.run_status == RIGHT)          
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y; /* 向上移动是，纵坐标减1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//如果下一位置上有食物
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			snake_para.score += snake_para.add_score;
			speedup();
			creatfood();
		}
		else
		{
			while (q->next->next != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("☆");
				q = q->next;
			}
			//经过上面的遍历，在前方没有食物的情况下q已经指向蛇尾，需要将移动前的蛇尾节点释放并以■填充
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("■");
			free(q->next);
			q->next = NULL;
		}
	}

	if (biteself() == 1)
	{
		snake_para.gameoverstatus = 2;
		GameOver();
	}
}

/*
 * 按键控制
 */
void keyboardcontrol()
{
	while (1)
	{
		scoreandtips();
		if (GetAsyncKeyState(VK_UP) && (snake_para.run_status != DOWN))
		{
			snake_para.run_status = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN) && ( snake_para.run_status != UP))
		{
			snake_para.run_status = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && (snake_para.run_status != RIGHT))
		{
			snake_para.run_status = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && (snake_para.run_status != LEFT))
		{
			snake_para.run_status = RIGHT;
		}

		if(GetAsyncKeyState(VK_SPACE))
		{
			while (1)
			{
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE))
				{
					break;
				}
			}
		}
		
		if (GetAsyncKeyState(VK_F1))
		{
			speedup();
		}
		else if(GetAsyncKeyState(VK_F2))
		{
			speeddown();
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			snake_para.gameoverstatus = 3; /* ESC键退出游戏 */
			GameOver();
			break;
		}	
		Sleep(snake_para.sleeptime);
		cantcrosswall();
		SnakeMove();
	}

}


/*
 * 游戏说明
 */
void GameExplanation()
{
	int choose_num = 0;
	system("cls");
	gotoxy(45,4);
	color(3);
	printf("游戏说明");
	for (int i = 20; i < 80; i += 2)
	{
		for (int j = 9; j < 25; j++)
		{
			gotoxy(i,j);
			if (i == 20 || i == 78)
			{
				printf("卍");
			}
			else if(j == 9 || j == 24)
			{
				printf("卍");
			}
		}
		
	}
	gotoxy(35,12);
	printf("tip1:不能撞墙，不能咬到自己");
	gotoxy(35,14);
	printf("tip2:分别使用↑↓←→控制蛇的移动方向");
	gotoxy(35,16);
	printf("tip3:按F1加速，F2减速");
	gotoxy(35,18);
	printf("tip4:空格键暂停游戏，再按一次继续游戏");
	gotoxy(35,20);
	printf("tip5:Esc键退出游戏到主界面");
	gotoxy(45,6);
	color(6);
	printf("作者：周浩");
	gotoxy(45,7);
	printf("日期：2019/10/19");
	gotoxy(30,26);
	color(13);
	printf("1--退出      2--回到主界面");
	gotoxy(30,28);
	printf("请选择：");
	scanf("%d",&choose_num);
	switch (choose_num)
	{
	case 1:
		exit(0);
		break;
	case 2:
		system("cls");
		PrintSnake();
		WelcomeToGame();
		break;
	default:
		break;
	}
}


void heigscore_read()
{
	FILE *fp;
	fp = fopen("save.txt","a+");
	fscanf(fp,"%d",&snake_para.heighscores);
	fclose(fp);
}


void heigscore_save()
{
	FILE *fp;
	fp = fopen("save.txt","w+");
	fprintf(fp,"%d",snake_para.score);
	fclose(fp);
}


int main()
{
	windowsize(105,35);
	PrintSnake();
	WelcomeToGame();
	heigscore_read();
	GameOver();
	system("pause");
	return 0;
}