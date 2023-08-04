#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#define Y 7
#define X 7//宏定义迷宫尺寸，便于修改
#define SPACE 2
#define Y_START 0//起点坐标y
#define X_START 1//起点坐标x
#define Y_END 6 //终点坐标y
#define X_END 5 //终点坐标x
#define FLAG '*' //标记路线的标记
#define CLEAR ' ' //表示迷宫中不是路障的地方的标识符
#define START 'S' //路线起点标记
#define END 'F' //路线终点标记
#define STONE '#' //路障标识符

template<class T>
class Maze
{
private:
	T map[Y][X];//迷宫数组地图
	string route;//用于记录路线
public:
	Maze(T a,char b)//构造函数
	{
		for (int i = 0; i < Y; i++)
			for (int j = 0; j < X; j++)
				map[i][j] = a;
		route = b;
	}
	void set_stone(int y, int x, T a)//在坐标(x,y)设置迷宫路障
	{
		map[y][x] = a;
	}
	int find_route(int y, int x,T flag,T clear,T end, T stone);//寻路函数
	void show_route();//输出路径
};
template<class T>
int Maze<T>::find_route(int y, int x,T flag, T clear, T end,T stone)//寻路函数
{
	if (map[y][x] == end)//递归终止条件，如果找到终点，则递归结束，并标记该坐标，成为路径的一部分，返回1
	{
		map[y][x] = flag;
		return 1;
	}
	else if (map[y][x] == stone)//如果目标位置被路障挡住，则此路不通，返回0
		return 0;
	else
	{
		map[y][x] = flag;//目标位置未被路障挡住
		route +=to_string(y);//路径中加入该步的y
		route +=to_string(x);//路径中加入该步的x
		if (y - 1 >= 0 && map[y - 1][x] != flag && find_route(y - 1, x, flag,clear, end,stone))//向上寻路
			return 1;
		else if (y + 1 >= 0 && map[y + 1][x] != flag && find_route(y + 1, x, flag, clear, end, stone))//向下寻路
			return 1;
		else if (x - 1 >= 0 && map[y][x - 1] != flag && find_route(y, x - 1, flag, clear, end, stone))//向左寻路
			return 1;
		else if (x + 1 >= 0 && map[y][x + 1] != flag && find_route(y, x + 1, flag, clear, end, stone))//向右寻路
			return 1;
		else
		{//如果走入死胡同则回退，尝试其他路径，并且在路径中清除错误路线的标记
			route = route.substr(0, route.length() - 2);
			map[y][x] = clear;
			return 0;
		}
	}
}
template<class T>
void Maze<T>::show_route()
{
	//标记路线起止点并且在控制台显示迷宫与路线
	set_stone(Y_START, X_START, START);
	set_stone(Y_END, X_END, END);
	cout << setiosflags(ios::left);
	cout << "迷宫地图:" << endl;
	cout << setw(SPACE) << ' ';
	for (int i = 0; i < X; i++)
		cout << setw(SPACE) << i;
	cout << endl;
	for (int i = 0; i < Y; i++)
	{
		cout << setw(SPACE) << i;
		for (int j = 0; j < X; j++)
			cout << setw(SPACE) << map[i][j];
		cout << endl;
	}

	//输出迷宫路径
	cout << endl << "迷宫路径:" << endl << endl;
	for (int i = 1, j = route.length(); i < j; i+=2)
	{
		cout << " <" << route[i] << ',' << route[i + 1] << "> ";
		if (i != j - 2)
			cout << "-->";
		if ((i+1)/2 % 5 == 0)
			cout << endl;
	}
	cout << endl << resetiosflags(ios::left);
}