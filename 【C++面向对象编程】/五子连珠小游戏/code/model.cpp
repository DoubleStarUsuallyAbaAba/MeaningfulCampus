#include <iostream>
#include <stdlib.h>  //生成随机数需要
#include <time.h>   //生成随机数需要
#include <Windows.h>//延时操作 需要
#include "function.h"
using namespace std;
extern class City city;

int City::score = 0,City::score_num=0;
City::City()
{
	for (int i = 0; i < X_NUM; i++)
		for (int j = 0; j < Y_NUM; j++)
			map[i][j] = INI;
	for (int i = 0; i < MARK; i++)
		score_sort[i] = -1;
}
void City::set()//重置map数组至初始状态
{
	for (int i = 0; i < X_NUM; i++)
		for (int j = 0; j < Y_NUM; j++)
			map[i][j] = INI;

}
void City::Draw_a_board()//画棋盘
{
	cct_gotoxy(0, 0);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int y = 0; y < Y_NUM * 2 + 1; y++)
	{
		for (int x = 0; x < X_NUM * 4 + 2; x += 2)
		{
			if (y == 0 && x == 0)
				cout << "┏";
			else if (y == Y_NUM * 2 && x == 0)
				cout << "┗";
			else if (y == 0 && x == X_NUM * 4)
				cout << "┓" << endl;
			else if (y == Y_NUM * 2 && x == X_NUM * 4)
				cout << "┛" << endl;
			else if (y == 0 && x % 4 == 0)
				cout << "┳";
			else if (y == Y_NUM * 2 && x % 4 == 0)
				cout << "┻";
			else if (y % 2 == 0 && x == 0)
				cout << "┣";
			else if (y % 2 == 0 && x == X_NUM * 4)
				cout << "┫" << endl;
			else if (y % 2 == 0 && x % 4 == 0)
				cout << "╋";
			else if (y % 2 == 0)
				cout << "━";
			else if (x % 4 == 0)
			{
				cout << "┃";
				if (x == X_NUM * 4)
					cout << endl;
			}
			else
				cout << "  ";
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Show_all_pieces()//画棋子
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if (map[i][j] < INI)
			{
				cct_setcolor(COLOR_HWHITE, city.map[i][j] + COLOR_DIS + ((map[i][j] < COLOR_MIN) ? COLOR_NUM : 0));
				cct_gotoxy(4 * (j + 1) - 2, 2 * i + 1);
				cout << "●";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
		}
	}
}
void City::Show_way(int x1, int y1, int x2, int y2)//画路线
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if ((!((i == x1 && j == y1) || (i == x2 && j == y2))) &&map[i][j] > 100)
			{
				cct_setcolor(COLOR_WHITE, COLOR_WHITE);
				cct_gotoxy(4 * j + 2, 2 * i + 1);
				cout << "  ";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
		}
	}
}
void City::Kill_pieces(int& number)//消除连珠
{
	for (int i = 0; i < X_NUM; i++)
	{
		for (int j = 0; j < Y_NUM; j++)
		{
			if (!(map[i][j] >= COLOR_MIN && map[i][j] < INI))
			{
				if (map[i][j] < COLOR_MIN)
					number = 4;
				map[i][j] = INI;
			}
		}
	}
}
void City::Move(int x1, int y1, int x2, int y2)//对调起点珠子和终点空棋盘图形的位置
{
	Sleep(200);
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cct_gotoxy(4 * y1 + 2, 2 * x1 + 1);
	cout << "  ";
	cct_setcolor(COLOR_WHITE, map[x2][y2] + COLOR_DIS);
	cct_gotoxy(4 * y2 + 2, 2 * x2 + 1);
	cout << "●";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Especially_show(int x, int y, int state)//突出显示起点与终点
{
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	cct_gotoxy(4 * y, 2 * x);
	cout << "┏━┓";
	cct_gotoxy(4 * y, 2 * x + 1);
	cout << "┃  ┃";
	cct_gotoxy(4 * y, 2 * x + 2);
	cout << "┗━┛";
	if (state)
	{
		cct_setcolor(COLOR_WHITE, city.map[x][y] + COLOR_DIS);
		cct_gotoxy(4 * y + 2, 2 * x + 1);
		cout << "●";
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void City::Current_score()//显示当前游戏的实时分数
{
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "                                                 ";
	cct_gotoxy(0, 2 * X_NUM + 2);
	cout << "当前分数 : " << score * 2 << "分";
}
void City::sort()//分数排序
{
	int temp_1 = -1, temp_2 = -1,state=0;
	score_num++;
	temp_1 = score* MARK + score_num;
	score *= 0;
		if (score_sort[0] == -1)
			score_sort[0] = temp_1;
		else
		{
			for (int i = 0;i<MARK-1; i++)
			{
				if (score_sort[i]/ MARK <= temp_1/MARK)
				{
					for (int j = i;j<MARK-1; j++)
					{
						temp_2 = score_sort[j];
						score_sort[j] = temp_1;
						temp_1 = temp_2;
						if (score_sort[j + 1] == -1)
						{
							score_sort[j + 1] = temp_2;
							state = 1;
							break;
						}
						else
							continue;
					}
				}
				else if (score_sort[i + 1] == -1)
				{
					score_sort[i + 1] = temp_1;
					break;
				}
				else
					continue;
				if (state)
					break;
			}
		}
}
void City::show_score()//展示分数
{
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cct_gotoxy(4*(X_NUM+1),0);
	cout << "请务必点击控制台左上角，在属性栏设置:使用旧版控制台";
	cct_gotoxy(4 * (X_NUM + 1), 1);
	cout<<"并关闭快速编辑和插入模式, 将输入法切换至英文";
	cct_setcolor(COLOR_BLACK,COLOR_HGREEN);
	cct_gotoxy(4 * (X_NUM + 1), 3);
	cout << "分数排行榜(分数从高到低排序,只显示最高的12个分数)" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HGREEN);
	for (int i = 0; i < MARK && score_sort[i] != -1; i++)
	{
		cct_gotoxy(4 * (X_NUM+1), i+4);
		cout << "第" << score_sort[i] % MARK << "次 :  " << score_sort[i] / MARK * 2 << " 分" << endl;
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
int City::Wall(int i, int j,int kind,int x_start,int y_start,int x_finish,int y_finish)//判断棋盘格是否被棋子包围，判断逻辑为先四角、再边界、后中间
{
	if ((i == 0 && j == 0 && map[i][j + 1] < INI && map[i + 1][j] < INI)
        && (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
	//&&(!(kind==1&&((x_start-1==x_finish&&y_start==y_finish)||(x_start+1==x_finish&&y_start==y_finish)||(x_start==x_finish&&y_start-1==y_finish)||(x_start==x_finish&&y_start+1==y_finish))))
		return NOT;
	else if ((i == 0 && j == Y_NUM - 1 && map[i][j - 1] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i == X_NUM - 1 && j == 0 && map[i][j + 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i == X_NUM - 1 && j == Y_NUM - 1 && map[i][j - 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((j >= 1 && j <= Y_NUM - 2 && i == 0 && map[i][j - 1] < INI && map[i][j + 1] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((j >= 1 && j <= Y_NUM - 2 && i == X_NUM - 1 && map[i][j - 1] < INI && map[i][j + 1] < INI && map[i - 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j == 0 && map[i - 1][j] < INI && map[i + 1][j] < INI && map[i][j + 1] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j == Y_NUM - 1 && map[i - 1][j] < INI && map[i + 1][j] < INI && map[i][j - 1] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else if ((i >= 1 && i <= X_NUM - 2 && j >= 1 && j <= Y_NUM - 2 && map[i][j + 1] < INI && map[i][j - 1] < INI && map[i - 1][j] < INI && map[i + 1][j] < INI)
		&& (!(kind == 1 && ((x_start - 1 == x_finish && y_start == y_finish) || (x_start + 1 == x_finish && y_start == y_finish) || (x_start == x_finish && y_start - 1 == y_finish) || (x_start == x_finish && y_start + 1 == y_finish)))))
		return NOT;
	else
		return INI;
}
int City::shortest_way(int x_start, int y_start, int x_finish, int y_finish)//计算最短路径
{
	//先判断路径能否走通
	if (x_start<0 || x_start>X_NUM - 1 || y_start<0 || y_start>Y_NUM - 1 || x_finish<0 || x_finish>X_NUM - 1 || y_finish<0 || y_finish>Y_NUM - 1)
		return -1;
	else if (map[x_start][y_start] >= INI || map[x_finish][y_finish] != INI)
		return -1;
	else if (Wall(x_start, y_start,0,x_start,y_start,x_finish,y_finish) == NOT || Wall(x_finish, y_finish,1, x_start, y_start, x_finish, y_finish) == NOT)
		return NOT;
	//再计算最优路径，逻辑为从终点开始反推，计算出终点到棋盘每一点的最短路径，
	//思路类似于Dijkstra算法，相比于广域搜索算法、优点在于面对棋局复杂的情况，计算量更小且保证了路径最短
	//而且可扩展性更强，通过相邻点间的路径加权以及松弛操作，现实导航地图计算、规划路线的功能
	else
	{
		int temp = map[x_start][y_start];
		map[x_finish][y_finish] = 0;//从终点反推，距离为0
		map[x_start][y_start] = INI;
		//计算出以终点为中心辐射出的最短路径，但在棋子遮挡的情况下，存在无法直接计算的点
		for (int grow = 1; !(x_finish - grow < 0 && x_finish + grow >X_NUM - 1 && y_finish - grow < 0 && y_finish + grow > Y_NUM - 1); grow++)
		{
			for (int i = 0; i < Y_NUM; i++)//向上、下扩展
			{
				if (!(x_finish - grow >= 0 && x_finish - grow <= X_NUM - 1))//判断是否超棋盘范围
					;
				else if (Wall(x_finish - grow, i,1, x_start, y_start, x_finish, y_finish) == NOT&& map[x_finish - grow][i]>=INI)//判断是否被其他棋子包围，！！此分支的优先级一定要高于下一分支，否则BUG
					map[x_finish - grow][i] = NOT;
				else if (map[x_finish - grow + 1][i] < 0 || map[x_finish - grow][i] < INI)//判断是否有棋子挡道
					;
				else//扩展最短距离
					map[x_finish - grow][i] = map[x_finish - grow + 1][i] + 1;

				if (!(x_finish + grow >= 0 && x_finish + grow <= X_NUM - 1))//判断是否超棋盘范围
					;
				else if (Wall(x_finish + grow, i,1, x_start, y_start, x_finish, y_finish) == NOT && map[x_finish + grow][i] >= INI)//判断是否被其他棋子包围，！！此分支的优先级一定要高于下一分支，否则BUG
					map[x_finish + grow][i] = NOT;
				else if (map[x_finish + grow - 1][i] < 0 || map[x_finish + grow][i] < INI)//判断是否有棋子挡道
					;
				else//扩展最短距离
					map[x_finish + grow][i] = map[x_finish + grow - 1][i] + 1;
			}
			for (int i = 0; i < X_NUM; i++)//向左、右扩展
			{
				if (!(y_finish - grow >= 0 && y_finish - grow <= Y_NUM - 1))//判断是否超棋盘范围
					;
				else if (Wall(i, y_finish - grow,1, x_start, y_start, x_finish, y_finish) == NOT&& map[i][y_finish - grow]>=INI)//判断是否被其他棋子包围，！！此分支的优先级一定要高于下一分支，否则BUG
					map[i][y_finish - grow] = NOT;
				else if (map[i][y_finish - grow + 1] < 0 || map[i][y_finish - grow] < INI)//判断是否有棋子挡道
					;
				else//扩展最短距离
					map[i][y_finish - grow] = map[i][y_finish - grow + 1] + 1;

				if (!(y_finish + grow >= 0 && y_finish + grow <= Y_NUM - 1))//判断是否超棋盘范围
					;
				else if (Wall(i, y_finish + grow,1, x_start, y_start, x_finish, y_finish) == NOT && map[i][y_finish + grow] >= INI)//判断是否被其他棋子包围，！！此分支的优先级一定要高于下一分支，否则BUG
					map[i][y_finish + grow] = NOT;
				else if (map[i][y_finish + grow - 1] < 0 || map[i][y_finish + grow] < INI)//判断是否有棋子挡道
					;
				else//扩展最短距离
					map[i][y_finish + grow] = map[i][y_finish + grow - 1] + 1;
			}
		}
		for (int k=1;;k++)//计算上一循环因为棋子遮挡未能计算的最短距离
		{//到此代码片段，最短距离向外走一步就可能不是最短距离，所以每外延一步都需要相邻比较，选择所有计算结果中最短的，才是最短路径
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if (map[i][j]==k)
					{//上、下、左、右扩展计算，直到终点反推到所有棋盘格的距离确定，循环结束
						if (map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] > map[i][j] + 1 || map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] == INI)
							map[i - ((i >= 1 && i < X_NUM) ? 1 : 0)][j] = map[i][j] + 1;
						if (map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] > map[i][j] + 1 || map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] == INI)
							map[i + ((i >= 0 && i < X_NUM - 1) ? 1 : 0)][j] = map[i][j] + 1;
						if (map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] > map[i][j] + 1 || map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] == INI)
							map[i][j - ((j >= 1 && j < Y_NUM) ? 1 : 0)] = map[i][j] + 1;
						if (map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] > map[i][j] + 1 || map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] == INI)
							map[i][j + ((j >= 0 && j < Y_NUM - 1) ? 1 : 0)] = map[i][j] + 1;
					}
				}
			}
			if (count(k, 360, 0, 0, 0) == 0)
			{
				for (int i = 0; i < X_NUM; i++)
				{
					for (int j = 0; j < Y_NUM; j++)
					{
						if (map[i][j] == INI)
							map[i][j] = NOT;
					}
				}
				break;
			}
		}
		if (map[x_start][y_start] == NOT)
		{
			map[x_start][y_start] = temp;
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if(!(map[i][j]<INI&&map[i][j]>=COLOR_MIN))
						map[i][j] = INI;
				}
			}
			return NOT;
		}
		else
		{
			//至此已计算得到起点到终点的最短距离，那么从起点开始找距离终点比自己小一的棋盘格，以此类推，直到到达终点，便是最短路径
			for (int i = x_start, j = y_start; !(i == x_finish && j == y_finish); )
			{//顺时针方向寻找，寻找方向不影响是否为最短距离
				if (i - 1 >= 0 && i - 1 <= X_NUM - 1 && map[i - 1][j] == map[i][j] - 1 && map[i - 1][j]>=0)
				{
					map[i][j] += 100;
					i -= 1;
				}
				else if (j + 1 >= 0 && j + 1 <= Y_NUM - 1 && map[i][j + 1] == map[i][j] - 1 && map[i][j + 1]>=0)
				{
					map[i][j] += 100;
					j += 1;
				}
				else if (i + 1 >= 0 && i + 1 <= X_NUM - 1 && map[i + 1][j] == map[i][j] - 1 && map[i + 1][j]>=0)
				{
					map[i][j] += 100;
					i += 1;
				}
				else if (j - 1 >= 0 && j - 1 <= Y_NUM - 1 && map[i][j - 1] == map[i][j] - 1 && map[i][j - 1] >=0)
				{
					map[i][j] += 100;
					j -= 1;
				}
				else
					continue;
			}
			map[x_finish][y_finish] = temp;
			return -2;
		}
	}
}
int City::count(int target, int angle, int x, int y, int state)//计算棋盘中目标棋子的数目、分担判断连珠与计分的功能
{                                                              //state为0表示非计分用途，1为计分用途，score增加
	int number_same = 0, number_score = 0, kill_x = 0, kill_y = 0;
	if (angle == 360)//全棋盘中目标棋子的数量
	{
		for (int x = 0; x < X_NUM; x++)
			for (int y = 0; y < Y_NUM; y++)
			{
				if (map[x][y] == target)
					number_same++;
			}
		return number_same;
	}
	else if (angle == 0)//0°方向连珠判断
	{
		for (int y = 0; y < Y_NUM - 1; y++)
		{
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x][y + 1] == target || map[x][y + 1] == target - COLOR_NUM))
			{
				number_same++;
				kill_y = y + 1;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[x][kill_y - i] == target)
					number_score++;
				map[x][kill_y - i] = target - COLOR_NUM;
			}
		}
		if (state)
			score += number_score;
		return number_same;
	}
	else if (angle == 90)//90°方向连珠判断
	{
		for (int x = 0; x < X_NUM - 1; x++)
		{
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x + 1][y] == target || map[x + 1][y] == target - COLOR_NUM))
			{
				number_same++;
				kill_x = x + 1;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[kill_x - i][y] == target)
					number_score++;
				map[kill_x - i][y] = target - COLOR_NUM;
			}
		}
		if (state&&number_same>= SCORE_NUM)
			score += number_score;
		return number_same;
	}
	else//45°、135°方向连珠判断
	{

		int x_add = -1, y_add = 1;
		if (x == X_NUM - 1 && angle == 135)
		{
			x_add = -1;
			y_add = -1;
		}
		else if (x == X_NUM - 1 && angle == 45)
		{
			x_add = -1;
			y_add = 1;
		}
		else if (x == 0 && angle == 45)
		{
			x_add = 1;
			y_add = -1;
		}
		else
		{
			x_add = 1;
			y_add = 1;
		}
		for (; x >= 0 && x <= X_NUM - 1 && y >= 0 && y <= Y_NUM - 1; x += x_add, y += y_add)
		{
			int x_next = x + x_add, y_next = y + y_add;
			if (x_next < 0)
				x_next = 0;
			else if (x_next > X_NUM - 1)
				x_next = X_NUM - 1;
			else if (y_next < 0)
				y_next = 0;
			else if (y_next > Y_NUM - 1)
				y_next = Y_NUM - 1;
			if ((map[x][y] == target || map[x][y] == target - COLOR_NUM) && (map[x_next][y_next] == target
				|| map[x_next][y_next] == target - COLOR_NUM))
			{
				number_same++;
				kill_x = x_next;
				kill_y = y_next;
			}
			else
			{
				if (number_same + 1 < SCORE_NUM)
					number_same = 0;
				else
					break;
			}
		}
		if (number_same + 1 >= SCORE_NUM)
		{
			number_same++;
			for (int i = 0; i < number_same; i++)
			{
				if (map[kill_x - x_add * i][kill_y - y_add * i] == target)
					number_score++;
				map[kill_x - x_add * i][kill_y - y_add * i] = target - COLOR_NUM;
			}
		}
		if (state)
			score += number_score;
		return number_same;
	}
}
int City::born(int num)//生成随机数目、随机颜色的棋子
{
	srand(static_cast<int>(time(0)));
	for (int i = 0; i < num;)
	{
		int x_rand = rand() % 9, y_rand = rand() % 9, original = score, result = 0;
		if (map[x_rand][y_rand] < INI)
			continue;
		else
		{
			map[x_rand][y_rand] = (rand() % COLOR_NUM + 5) * (-1);
			continuity(1);
			result = score;
			if (result != original)
			{
				score -= (result - original);
				if (num == BEGIN_NUM)
				{
					map[x_rand][y_rand] = INI;
					result = original;
					continue;
				}
				else
					i++;
			}
			else
				i++;
		}
		if (result != original)
		{
			for (int i = 0; i < X_NUM; i++)
			{
				for (int j = 0; j < Y_NUM; j++)
				{
					if (map[i][j] < INI)
					{
						cct_setcolor(COLOR_HWHITE, map[i][j] + COLOR_DIS + ((map[i][j] < COLOR_MIN) ? COLOR_NUM : 0));
						cct_gotoxy(4 * (j + 1) - 2, 2 * i + 1);
						cout << "●";
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						if (map[i][j] < COLOR_MIN)
						    map[i][j] = INI;
					}
				}
			}
			result = original;
			Sleep(700);
			Draw_a_board();
		}
		if (count(INI, 360, 0, 0, 0) == 0)
			break;
		else
			continue;
	}
	if (count(INI, 360, 0, 0, 0) == 0)
		return 1;
	else
		return 0;
}
void City::continuity(int state)//扫描全棋盘、判断连珠
{
	for (int color = -5; color >= -10; color--)
	{
		for (int x = 0; x <= X_NUM - 1; x++)
			count(color, 0, x, 0, state);
		for (int y = 0; y <= Y_NUM - 1; y++)
			count(color, 90, 0, y, state);
		for (int y = SCORE_NUM - 1; y < Y_NUM; y++)
			count(color, 45, 0, y, state);
		for (int y = 0; y <= Y_NUM - SCORE_NUM; y++)
			count(color, 45, X_NUM - 1, y, state);
		for (int y = SCORE_NUM - 1; y < X_NUM; y++)
			count(color, 135, X_NUM - 1, y, state);
		for (int y = 0; y <= Y_NUM - SCORE_NUM; y++)
			count(color, 135, 0, y, state);
	}
}