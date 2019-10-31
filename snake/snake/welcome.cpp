#include "welcome.h"
#include "snake.h"
#include<stdio.h>
#include<windows.h>

/*
 * 绘制开始界面蛇型
 *
 */
void PrintSnake()
{
	gotoxy(35,1);
	color(6);
	printf("/^\\/^\\");

	gotoxy(34,2);
	printf("|○_|○|");

	gotoxy(33,2);
	color(2);
	printf("_");

	gotoxy(25,3);
	color(12);
	printf("\\/");

	gotoxy(31,3);
	color(2);
	printf("/");

	gotoxy(37,3);
	color(6);
	printf("\\_/");

	gotoxy(41,3);
	color(10);
	printf(" \\");

	gotoxy(26,4);
	color(12);
	printf("\\__");

	gotoxy(32,4);
	printf("________/");

	gotoxy(31,4);
	color(2);
	printf("|");

	gotoxy(43,4);
	color(10);
	printf("\\");

	gotoxy(32,5);
	color(2);
	printf("\\_______");

	gotoxy(44,5);
	color(10);
	printf("\\");

	gotoxy(39,6);
	printf("|       |                  \\");       /*蛇身*/

	gotoxy(38,7); 
	printf("/       /                    \\");       

	gotoxy(37,8);
	printf("/       /                     \\  \\");    

	gotoxy(35,9);
	printf("/       /                        \\  \\");    

	gotoxy(34,10);
	printf("/       /                          \\  \\");   

	gotoxy(33,11);
	printf("/       /           _---_             \\  \\");    

	gotoxy(32,12);
	printf("/       /       _-～       ～-_         |  |");    

	gotoxy(31,13);
	printf("(       (    _-～    _--_       ～-_    _/   |");    

	gotoxy(30,14);
	printf("\\      ～-__-～   _-～   ～-_     ～-__-～   /");    

	gotoxy(33,15);
	printf("～-_         _-～        ～-_         _-～ ");    

	gotoxy(35,16);
	printf("～--____-～                ～-___-～   \n");    
}


/*
 * 绘制欢迎界面
 */
void WelcomeToGame()
{
	int n;
	gotoxy(50,20);
	color(11);
	printf("贪食蛇大作战\n");
	color(14);
	for (int i = 23; i < 33; i++)
	{
		for (int j = 40; j < 71; j++)
		{
			gotoxy(j,i);
			if(i == 23 || i == 32)
			{
				printf("-");
			}
			else if (j == 40 || j == 70)
			{
				printf("|");
			}
		}
	}

	color(12);/*红字*/
	gotoxy(50,25);
	printf("1：开始游戏");
	gotoxy(50,28);
	printf("2：游戏说明");
	gotoxy(50,31);
	printf("3：退出游戏\n");
	color(3);
	gotoxy(40,33);
	printf("请选择[1 2 3]：[ ]\b\b"); /* \b为退格，是的光标处于[ ]中间*/
	color(14);
	scanf("%d",&n);
	switch (n)
	{
		case 1:
			system("cls");
			SnakeParaInit();
			creatgamemap();
			initsnake();
			creatfood();
			keyboardcontrol();
			break;  
		case 2:
			GameExplanation();
			break;
		case 3:exit(0);break;
		default:
			break;
	}
}
