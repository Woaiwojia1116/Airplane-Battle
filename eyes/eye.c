#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "OLED_Data.h"
#include <time.h>
#include <stdlib.h>
#include "Key.h"

#define Bullet_Unlook_offset 5
#define Enemy_Unlook_offset 5
#define Plant_Unlook_offset 5
//起始位置= Enemy_X_L Enemy_Y_U
typedef struct{
	uint8_t Enemy_X_L;
	uint8_t Enemy_X_R;
	uint8_t Enemy_Y_U;
	uint8_t Enemy_Y_D;
}Enemy_Pos;
typedef struct Enemy
{
	Enemy_Pos enemypos;
	struct Enemy *next;
}Enemy;
typedef struct 
{
	Enemy *head;
	Enemy *tail;
}List;
List list = {NULL,NULL};

static uint8_t x;
static uint8_t y;
static uint8_t B_X;
static uint8_t B_Y;
static uint16_t Enemy_slow_10;
static uint8_t game_state; //0开始界面，1开始游戏,2结束界面
static uint8_t Key_Num;

void AddEnemy(List *list)
{
	Enemy *new_enemy = (Enemy*)malloc(sizeof(Enemy));
	if(!new_enemy)return;
	new_enemy->enemypos.Enemy_X_L = 0;
	new_enemy->enemypos.Enemy_Y_U = 0;
	new_enemy->next = NULL;
	if(list->head)
	{
		list->tail->next = new_enemy;
		list->tail = new_enemy;
	}
	else
	{
		list->head = new_enemy;
		list->tail = new_enemy;
	}
}
void Game_Init(void)
{
	AD_Init();
	game_state = 0;
	AddEnemy(&list);
}
void Key_Pro(void)
{
	Key_Num = Key_GetNum();       
	if(game_state==0&&Key_Num==1)
		game_state=1;		
}                                                                                                                                                                                                                                                                                                                                   
void AD_Conver(void)    //将ad值转化为屏幕的宽度
{
	x =  AD_Value[0]*123/4095;
	y =  AD_Value[1]*59/4095;
}
typedef struct{
	uint8_t x_L;
	uint8_t x_R;
	uint8_t y_U;
	uint8_t y_D;
}Plant_Pos;
Plant_Pos P;
void plant_Show(void)
{
	P.x_L = x;
	P.x_R = x +Plant_Unlook_offset;
	P.y_U = y;
	P.y_D = y+Plant_Unlook_offset;
	OLED_ShowImage(x,y,5,5,plant);
}
typedef struct{
	uint8_t B_X_L;
	uint8_t B_X_R;
	uint8_t B_Y_U;
	uint8_t B_Y_D;
}Bullet_Pos;
Bullet_Pos B;
void Bullet_Show(void)   //显示子弹
{
	B.B_X_L = B_X;
	B.B_X_R = B_X+Bullet_Unlook_offset;
	B.B_Y_U = B_Y;
	B.B_Y_D = B_Y+Bullet_Unlook_offset;
	if(Key_Num)
	{
		B_X = x;
		B_Y = y;	
	}
	OLED_ShowImage(B_X,B_Y-Bullet_Unlook_offset,5,5,bullet);
}
void Enemy_Show(void)
{
	if(Enemy_slow_10<10)return;
	Enemy_slow_10 = 0;
	Enemy *current = list.head;
	while(current!=NULL)
	{
		current->enemypos.Enemy_X_R = current->enemypos.Enemy_X_L+Enemy_Unlook_offset;
		current->enemypos.Enemy_Y_D = current->enemypos.Enemy_Y_U+Enemy_Unlook_offset;
		
		if(current->enemypos.Enemy_Y_U<59)
		{
            current->enemypos.Enemy_Y_U++;  // 敌人向下移动
            current->enemypos.Enemy_Y_D++;  // 更新下边界			
		}
		else if(current->enemypos.Enemy_Y_U==59)
		{
			current->enemypos.Enemy_Y_U = 0;
			current->enemypos.Enemy_X_L = rand()%(128-Enemy_Unlook_offset);		
		}
		OLED_ShowImage(current->enemypos.Enemy_X_L,current->enemypos.Enemy_Y_U,6,5,enemy);	
		current = current->next;
	}
}
void Check_crush(void)
{   
	Enemy *current = list.head;
	while(current!=NULL)
	{
		if((B.B_X_L<current->enemypos.Enemy_X_R&&B.B_X_R>current->enemypos.Enemy_X_R&&B.B_Y_U<current->enemypos.Enemy_Y_D)
			||(B.B_X_L<current->enemypos.Enemy_X_L&&B.B_X_R>current->enemypos.Enemy_X_L&&B.B_Y_U<current->enemypos.Enemy_Y_D))		
		{
			B_Y = 0;
			current->enemypos.Enemy_Y_U = 0;
		}		
		current = current->next;
	}
    if(current!=NULL)
	{
		if((B.B_X_L<current->enemypos.Enemy_X_R&&B.B_X_R>current->enemypos.Enemy_X_R&&B.B_Y_U<current->enemypos.Enemy_Y_D)
			||(B.B_X_L<current->enemypos.Enemy_X_L&&B.B_X_R>current->enemypos.Enemy_X_L&&B.B_Y_U<current->enemypos.Enemy_Y_D))	
		{
			game_state  = 2;
		}		
	}
}
void Eyes_Show(void)
{  
	OLED_Clear();
	Key_Pro();
	if(game_state==1)
	{
		AD_Conver();
		Bullet_Show();
		Check_crush();
		plant_Show();	
	}
    else if(game_state==0)
	{
		OLED_ShowString(48,23,"START",OLED_8X16);
	}
    else if(game_state==2)
	{
		OLED_ShowString(48,23,"GAMEOVER",OLED_8X16);
	}
	OLED_Update();
} 
void Eyes_Tick(void)  //0.01秒定时器
{
	Key_Tick();
	Enemy_slow_10++;
	if(B_Y>0)B_Y--;		
	else B_Y = 0;			
}
