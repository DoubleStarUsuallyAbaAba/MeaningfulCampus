#include <iostream>
#include <Windows.h>//延时操作 需要
#include <conio.h>
#include <string.h>
#include "function.h"
using namespace std;

City city;

int control()
{
	int state = 0, x0 = -1, y0 = -1, x1 = -1, y1 = -1, x2 = -1, y2 = -1, action = -1, key_1 = -1, key_2 = -1, num = 1, judge = 1, quit = 0;
	while (1)
	{
		state = my_read_keyboard_and_mouse(x0, y0, action, key_1, key_2);//读鼠标、键盘行为，并作出响应 
		if (state == -1)//表示：如果按ESC键，则退出
			break;
		else if ((state == -2 && num == 1) || (state == -2 && num == 2 && city.map[(y0 - 1) / 2][(x0 - 1) / 4] != INI))//表示选择要移动的珠子，
		{                                                                                                    //在未选择移动目的地之前，可点击其他珠子以更换要移动的珠子
			if (x0 < 1 || x0>4 * Y_NUM || y0 < 1 || y0 > 2 * X_NUM - 1 || y0 % 2 == 0)//鼠标点击非法位置（如：棋盘外或具有二义性的位置）
				continue;
			else
			{
				if (city.map[(y0 - 1) / 2][(x0 - 1) / 4] >= INI)//玩家在没选要移动的珠子之前就选择了空棋盘格作为珠子移动的目的地，违反游戏规则
					continue;
				else//玩家选择了要移动的珠子
				{
					if (num == 2)//如果这不是玩家第一次选择的珠子
					{//那么之前在玩家选择要移动的珠子时，已经有突出显示的珠子，所以需要重画棋盘和棋子
						city.Draw_a_board();//重画棋盘
						city.Show_all_pieces();//重画棋子
					}
					num = 2;
					x1 = ((y0 - 1) / 2);//把鼠标坐标换算成对应的map数组元素的坐标
					y1 = ((x0 - 1) / 4);
					city.Especially_show(x1,y1,1);//突出显示玩家选择的要移动的珠子
					continue;
				}
			}
		}
		else if (state == -2 && num == 2)//玩家选择了珠子要移动到的目的地
		{
			if (x0 < 1 || x0>4 * Y_NUM || y0 < 1 || y0 > 2 * X_NUM - 1 || y0 % 2 == 0)//鼠标点击非法位置（如：棋盘外或具有二义性的位置）
				continue;
			else
			{
				x2 = ((y0 - 1) / 2);//把鼠标坐标换算成对应的map数组元素的坐标
				y2 = ((x0 - 1) / 4);
				num = 3;
				city.Especially_show(x2, y2, 0);//突出显示玩家选择的珠子要移动到的目的地
				judge = city.shortest_way(x1, y1, x2, y2);//计算最短路径
				if (judge != NOT)
				{
					city.Show_way(x1,y1,x2,y2);//画路线
					city.Move(x1,y1,x2,y2);//起点珠子和终点空棋盘图形对调位置
					city.continuity(1);//判断连珠并计分
					city.Current_score();//更新当前游戏的实时分数
					city.Kill_pieces(num);//消除连珠
				}
				city.Draw_a_board();//重画棋盘
				if (num == 3 && judge != NOT)//表示移动操作未形成连珠
					quit = city.born(GAMING_NUM);//生成随机珠子
				city.Show_all_pieces();//重画珠子
				break;
			}
		}
	}
	if (state == -1)//按了ESC键，退出当前盘游戏
		return 0;
	else if (quit)//棋盘被棋子占满，退出当前盘游戏
		return 0;
	else//继续当前盘游戏
		return 1;
}

void first_instruction()//输出游戏玩法说明
{
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "当前分数 : 0 分";
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "玩法说明(~游戏选择棋子或棋盘格时请";
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cout << "鼠标左键单击棋子或棋盘格正中心";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cout<<"哦~)" << endl;
	cout << "同色五子连珠消除游戏,珠子共六种颜色,连珠消除越多,得分越高,每珠2分,横、竖、斜向消除均可,按ESC键可退出当前棋局。" << endl;
	cout << "先左键单击选择要移动的目标珠子，再左键单击选择空棋盘格作为移动终点,存在可移动路径时方可移动,且会显示最短移动路径。" << endl;
	cout << "每次移动若不能实现连珠消除,棋盘会增加三个随机颜色的珠子,若随机生成的珠子恰好连珠则自动消失,但游戏者不得分。";
}
void last_instruction()//输出当前盘游戏结束，是重开一局还是退出游戏的选择提示
{
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cout << "                                                                                                                  " << endl;
	cct_gotoxy(0, 2 * X_NUM + 4);
	cout << "GAME OVER !!!  Dear gamer : " << endl;
	cout << "选择重新开局请输入 0,选择退出游戏请输入 1 (如果选择退出游戏,将不再保留您的分数排行榜数据) ";
}

int main()
{
	int x = 0, y = 0, xx = 0, yy = 0;
	char back = '0';//此值用于判断是否跳出外层循环
	cct_getconsoleborder(x, y, xx, yy);//获取玩家初始时cmd窗口的大小设置，以便改变窗口大小后为玩家恢复原状
	cct_setconsoleborder(X_SET, Y_SET, XX_SET,YY_SET);//改变cmd窗口的大小及缓冲区的大小，以使窗口大小适应游戏界面显示
	while (1)
	{
		int judge = 1;//此值用于判断是否跳出内层循环
		cct_setcursor(CURSOR_INVISIBLE);//设置光标状态为不显示，以使界面美观
		city.set();//初始化数据层数组
		city.Draw_a_board();//画棋盘
		city.born(BEGIN_NUM);//生成随机数目、随机颜色的棋子
		city.Show_all_pieces();//画棋子
		city.show_score();//展示分数榜
		first_instruction();//输出游戏玩法说明
		cct_enable_mouse();//允许使用鼠标
		while (1)
		{
			judge = control();
			if (judge)
				continue;
			else
				break;
		}
		last_instruction();//输出当前盘游戏结束，是重开一局还是退出游戏的选择提示
		cct_setcursor(CURSOR_VISIBLE_NORMAL);//设置光标状态为通常显示，以使输入提示明显
		while (1)
		{
			back = _getche();
			if (back == '1')
				break;
			else if (back == '0')
			{
				cct_cls();//清屏
				city.sort();//排序玩家的历次分数
				break;
			}
			else//屏蔽0、1之外的其他非法输入
			{
				cct_gotoxy(strlen("选择重新开局请输入 0,选择退出游戏请输入 1,选择退出将清空您的历史分数记录 "), 2 * X_NUM + 9);
				cout << "                            ";
				cct_gotoxy(strlen("选择重新开局请输入 0,选择退出游戏请输入 1,选择退出将清空您的历史分数记录 "), 2 * X_NUM + 9);
				continue;
			}
		}
		if (back == '1')
			break;
		else
			continue;
	}
	cct_setconsoleborder(x, y, xx, yy);//为玩家恢复自己的窗口大小
	return 0;
}
