#ifndef _DATATYPE_H
#define _DATATYPE_H


//蛇的运动方向
#define UP     1
#define DOWN   2
#define LEFT   3
#define RIGHT  4


typedef struct snake    
{
	int x;
	int y;
	struct snake *next;

}snake_s;

typedef struct snake_p
{
	int heighscores;
	int score;
	int add_score;	
	int sleeptime;
	char gameoverstatus;
	char run_status;
}snake_para_s;



#endif 