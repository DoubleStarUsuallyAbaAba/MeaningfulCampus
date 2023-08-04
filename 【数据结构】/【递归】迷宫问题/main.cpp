#include <iostream>
#include "maze.h"
using namespace std;

int main()
{
	Maze<char> maze(CLEAR,'*');//实例化迷宫类，用空格初始化迷宫数组，用字符*作为路线的开头标识符

	//设置四周围墙
	for (int i = 0; i < X; i++)
	{
		maze.set_stone(0, i, STONE);
		maze.set_stone(Y-1, i, STONE);
	}
	for (int i = 1; i < Y-1; i++)
	{
		maze.set_stone(i, 0, STONE);
		maze.set_stone(i, X-1, STONE);
	}
	//设置路障、迷宫起点与终点
	maze.set_stone(1, 2, STONE);
	maze.set_stone(2, 2, STONE);
	maze.set_stone(4, 2, STONE);
	maze.set_stone(5, 2, STONE);
	maze.set_stone(2, 4, STONE);
	maze.set_stone(3, 4, STONE);
	maze.set_stone(5, 4, STONE);
	maze.set_stone(2, 5, STONE);
	maze.set_stone(Y_START, X_START, START);
	maze.set_stone(Y_END, X_END, END);
	//寻路
	maze.find_route(Y_START, X_START, FLAG, CLEAR, END, STONE);
	//展示路线
	maze.show_route();

	//******************************************************************************************************
    //******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************
	return 0;
}