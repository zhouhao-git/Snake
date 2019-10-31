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
snake_s *q; /* ������ʱ�õ���ָ�� */


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
 * ���ÿ���̨�����С
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),c);  /*����������ɫ*/
	return 0;
}

/*
 * ���ù���λ��
 */
void gotoxy(int x, int y)
{
	COORD cursor;
	cursor.X = x;
	cursor.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cursor); /*��λ���λ��*/
}


/*
 * ������Ϸ��ͼ
 */
void creatgamemap()
{
	for (int i = 0; i < 61; i+=2)  /*��ӡ���±߿�*/
	{
		gotoxy(i, 0);
		color(19);
		printf("��");
		gotoxy(i,30);
		printf("��");
	}
	for (int j = 0; j < 31; j++)  /*��ӡ���ұ߿�*/
	{
		gotoxy(0,j);
		color(18);
		printf("��");
		gotoxy(60,j);
		printf("��");
	}

	for (int i = 2; i < 60; i+=2)      /*������Ϸ�м���*/
	{
		for (int j = 1; j < 30; j++)
		{
			gotoxy(i,j);
			color(3);
			printf("��\n\n");
		}
	}

	for (int i = 0; i < 31; i++)/*���Ʒָ���*/
	{
		gotoxy(62,i);
		color(6);
		printf("��");
	}
}

/*
 * ������Ϸ�Ҳ�÷ֺ���ʾ����
 */
void scoreandtips()
{
	heigscore_read();

	for (int i = 65; i < 100; i+=2)
	{
		gotoxy(i,10);
		color(1);
		printf("�T");
		gotoxy(i,30);
		printf("�T");
	}
	for (int j = 11; j < 30; j++)
	{
		gotoxy(64,j);
		color(1);
		printf("�U");
		gotoxy(100,j);
		printf("�U");
	}

	gotoxy(75,2);
	color(11);
	printf("����߷ּ�¼��%d",snake_para.heighscores);
	gotoxy(80,6);
	color(14);
	printf("�÷֣�%d\n",snake_para.score);

	color(13);
	gotoxy(80,13);
	printf("С��ʾ��");
	gotoxy(66,17);
	printf("ÿ��ʳ��÷֣�10��");
	gotoxy(66,19);
	printf("�á����� ���ı��ߵ��ƶ�����");
	gotoxy(66,21);
	printf("����ײǽ������ҧ���Լ�");
	gotoxy(66,23);
	printf("F1����   F2����");
	gotoxy(66,25);
	printf("Space����ͣ��Ϸ");
	gotoxy(66,27);
	printf("Esc���˳���Ϸ");
}




/*
 * ��������
 */
void initsnake()
{
	snake_s *tail;
	tail = (snake_s*)malloc(sizeof(snake_s));  //��β
	if (!tail)
	{
		MessageBoxA(NULL,"��β�ڴ����ʧ��","����",MB_OK);
		return;
	}
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (int i = 1; i <= 4; i++)
	{
		head = (snake_s*)malloc(sizeof(snake_s));  //��ͷ
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
		//	printf("��");
		//}
		//else
		{
			gotoxy(tail->x,tail->y);
			color(14);
			printf("��");
		}
		tail = tail->next;
	}
}


/* 
 * �����������(1�������������м䣬���ܳ������������� 2��ʳ�ﲻ���������غ�)
 */
void creatfood()
{
	snake_s *food_info;

	srand((unsigned)time(NULL)); 
	food_info = (snake_s*)malloc(sizeof(snake_s));
	if (!food_info)
	{
		MessageBoxA(NULL,"ʳ���ڴ����ʧ��","����",MB_OK);
		return;
	}
	while ((food_info->x % 2) != 0)      /*��֤ʳ�����������м�����Ǳ�����,�����м������ֵΪ����*/
	{
		food_info->x = rand() % 57 + 2;   
	}	
	food_info->y = rand() % 29 + 1;      
	q = head;
	while (q->next != NULL)      /*ÿ����һ��ʳ�����������бȽ�*/
	{
		if ((q->x == food_info->x) && (q->y == food_info->y)) /* �ж�ʳ���Ƿ��������غ� */
		{
			free(food_info);           
			creatfood();
		}
		q = q->next;
	}
	gotoxy(food_info->x,food_info->y);
	food = food_info;
	color(12);
	printf("��");
}


/**************��Ϸ�߼��������*******************/
/*��Ϸ�����жϣ�
 * 1���ж��Ƿ�ҧ���Լ� 2��ײǽ  */


/* 
 * ���٣��ٵ�ʳ���Զ����ٻ�F1����
 */
void speedup()
{
	if (snake_para.sleeptime > 50)
	{
		//sleeptime -= 10;
		//add_score +=2;    /* ÿ��һ��ʳ��ӷ�ֵ��2*/
		snake_para.sleeptime -= 10;
	}
}

/*
 * ���٣�F2����
 */
void speeddown()
{
	if(snake_para.sleeptime < 350)
	{
		snake_para.sleeptime += 30;
		//add_score -= 2;
		//if (sleeptime == 350)
		//{
		//	add_score = 1;      /*�����ٶ�Ϊ350ʱ���ӷּ���0������Ҫ��֤��С�ļӷ�*/
		//}
	}
}

/*
 * ������ʱ�ߵķ������
 */
void SnakeMove()
{
	snake_s *nexthead; /* ����µ���ͷ�ڵ���м���� */

	nexthead = (snake_s*)malloc(sizeof(snake_s));
	if (!nexthead)
	{
		MessageBoxA(NULL,"�м�ڵ����ʧ��","����",MB_OK);
		return;
	}
	if (snake_para.run_status == UP)          
	{
		nexthead->x = head->x;
		nexthead->y = head->y - 1; /* �����ƶ��ǣ��������1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//�����һλ������ʳ��
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("��");
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
				printf("��");
				q = q->next;
			}
			//��������ı�������ǰ��û��ʳ��������q�Ѿ�ָ����β����Ҫ���ƶ�ǰ����β�ڵ��ͷŲ��ԡ����
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("��");
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

		//�����һλ������ʳ��
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("��");
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
				printf("��");
				q = q->next;
			}
			//��������ı�������ǰ��û��ʳ��������q�Ѿ�ָ����β����Ҫ���ƶ�ǰ����β�ڵ��ͷŲ��ԡ����
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("��");
			free(q->next);
			q->next = NULL;
		}
	}
	if (snake_para.run_status == LEFT)          
	{
		nexthead->x = head->x - 2;
		nexthead->y = head->y; /* �����ƶ��ǣ��������1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//�����һλ������ʳ��
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("��");
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
				printf("��");
				q = q->next;
			}
			//��������ı�������ǰ��û��ʳ��������q�Ѿ�ָ����β����Ҫ���ƶ�ǰ����β�ڵ��ͷŲ��ԡ����
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("��");
			free(q->next);
			q->next = NULL;
		}
	}
	if (snake_para.run_status == RIGHT)          
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y; /* �����ƶ��ǣ��������1 */
		nexthead->next = head;  
		head = nexthead;
		q = head;

		//�����һλ������ʳ��
		if ((q->x == food->x) && (q->y == food->y))
		{
			while (q != NULL)
			{
				gotoxy(q->x,q->y);
				color(14);
				printf("��");
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
				printf("��");
				q = q->next;
			}
			//��������ı�������ǰ��û��ʳ��������q�Ѿ�ָ����β����Ҫ���ƶ�ǰ����β�ڵ��ͷŲ��ԡ����
			gotoxy(q->next->x,q->next->y);
			color(3);
			printf("��");
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
 * ��������
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
			snake_para.gameoverstatus = 3; /* ESC���˳���Ϸ */
			GameOver();
			break;
		}	
		Sleep(snake_para.sleeptime);
		cantcrosswall();
		SnakeMove();
	}

}


/*
 * ��Ϸ˵��
 */
void GameExplanation()
{
	int choose_num = 0;
	system("cls");
	gotoxy(45,4);
	color(3);
	printf("��Ϸ˵��");
	for (int i = 20; i < 80; i += 2)
	{
		for (int j = 9; j < 25; j++)
		{
			gotoxy(i,j);
			if (i == 20 || i == 78)
			{
				printf("�d");
			}
			else if(j == 9 || j == 24)
			{
				printf("�d");
			}
		}
		
	}
	gotoxy(35,12);
	printf("tip1:����ײǽ������ҧ���Լ�");
	gotoxy(35,14);
	printf("tip2:�ֱ�ʹ�á������������ߵ��ƶ�����");
	gotoxy(35,16);
	printf("tip3:��F1���٣�F2����");
	gotoxy(35,18);
	printf("tip4:�ո����ͣ��Ϸ���ٰ�һ�μ�����Ϸ");
	gotoxy(35,20);
	printf("tip5:Esc���˳���Ϸ��������");
	gotoxy(45,6);
	color(6);
	printf("���ߣ��ܺ�");
	gotoxy(45,7);
	printf("���ڣ�2019/10/19");
	gotoxy(30,26);
	color(13);
	printf("1--�˳�      2--�ص�������");
	gotoxy(30,28);
	printf("��ѡ��");
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