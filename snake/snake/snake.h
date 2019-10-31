#ifndef _SNAKE_H
#define _SNAKE_H



void gotoxy(int x, int y);
int color(int c);
void windowsize(int w,int h);
void SnakeParaInit();
void heigscore_read();
void heigscore_save();
void creatgamemap();
void initsnake();
void creatfood();
void snakemvoe();
void keyboardcontrol();
void GameExplanation();


#endif  