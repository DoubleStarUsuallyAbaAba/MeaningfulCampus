﻿#include <iostream>
#include <string>
using namespace std;

string queen="0";

void born(int num)//生成num*num长度的string字符串，二维的方阵排列在本程序中是转换为一维字符串来执行的
{
	for (int i = 1; i < num*num; i++)
		queen += "0";//初始化字符串，不摆皇后的位置用字符 0 表示
}
int insert_up(int y,int x,int num)//判断正上方是否已经摆放了皇后，递归实现
{
	if (y == 0 && queen[x] != '*')//如果直到(x，0)的位置都没有皇后，那么正上方就没有皇后，递归结束，返回1
		return 1;
	else if (y == 0 && queen[x] == '*')//如果(x,0)的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else if (queen[(y - 1) * num + x] == '*')//如果(x,y-1)的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else
		return insert_up(y-1,x,num);//如果(x,y)的位置没有皇后，那么就继续考察(x,y-1)位置，往正上方考察
}
int insert_left_up(int y, int x, int num)//判断左上方是否已经摆放了皇后，递归实现
{
	if ((y == 0||x==0) && queen[y*num+x] != '*')//如果直到左边缘的位置都没有皇后，那么左上方就没有皇后，递归结束，返回1
		return 1;
	else if ((y == 0||x==0) && queen[y*num+x] == '*')//如果左边缘的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else if (queen[(y - 1) * num + x-1] == '*')//如果(x-1,y-1)的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else
		return insert_left_up(y - 1, x-1, num);//如果(x,y)的位置没有皇后，那么就继续考察(x-1,y-1)位置，往左上方考察
}
int insert_right_up(int y, int x, int num)//判断右上方是否已经摆放了皇后，递归实现
{
	if ((y == 0 || x == num-1) && queen[y * num + x] != '*')//如果直到右边缘的位置都没有皇后，那么右上方就没有皇后，递归结束，返回1
		return 1;
	else if ((y == 0 || x == num-1) && queen[y * num + x] == '*')//如果右边缘的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else if (queen[(y - 1) * num + x + 1] == '*')//如果(x-1,y+1)的位置有皇后，那么(x,y)位置就不能再摆皇后，递归结束，返回0
		return 0;
	else
		return insert_right_up(y - 1, x + 1, num);//如果(x,y)的位置没有皇后，那么就继续考察(x-1,y+1)位置，往右上方考察
}

int main()
{
	int num,count=0,success=0,x, y, x_start=0,clear=0;
	string bin;//bin专门用来吃掉无效输入，功能类似于垃圾桶一般，故命此名
	cout << "现有N×N的棋盘，放入N个皇后，要求所有皇后不在同一行、列和同一斜线上!" << endl<<endl;
	while (1)
	{
		cout << "请输入皇后的个数(1<=num<=15): ";//综合考虑程序运行时间以及每增加一个皇后，结果数量的增长量级，将皇后数量上限定在15
		cin >> num;
		if (cin.fail() || num <= 0||num>15)//若输入非数字字符或数字不在[1,15]的范围内，则重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << endl << "! 无效输入 !" << endl;
			continue;
		}
		else
			break;
	}
	born(num); //生成num*num长度的string字符串queen，二维的方阵排列在本程序中是转换为一维字符串来执行的,不摆皇后的位置用字符 0 表示
	cout << endl << "皇后摆法:" << endl;

	//*****************************************************************************************************
	//*****************************************************************************************************
	//算法基本思路：先在(0,0)位置摆放皇后，接下来在下一行寻找符合规则的可以摆放下一个皇后的位置，
	//若找到则继续下一行，否则退回到上一行调整上一个皇后的位置，
	//那么循环结束的条件便是第0行已经全部试完，只能调整不存在的第-1行的皇后位置
	//*****************************************************************************************************
	//*****************************************************************************************************

	for (y = 0;;)//将排列方阵看作矩阵且与坐标系对应的话，y表示的是第y行
	{
		if (y < 0)//循环终止条件，y<0意味着需要调整不存在的-1行的皇后棋子的摆放位置
			break;
		for (x = x_start;; x++)
		{
			if (x >= num)//表示第y行目前已近不存在可以摆放皇后的位置
			{
				y--;//回退到y-1行调整该行的皇后位置

				if (y < 0)//此处与最外层循环的终止条件原理相同
					break;
				//下面两句是用来把已经摆放好的第y-1行的皇后删除，在程序中就是用字符 0 覆盖 字符 *
				clear = queen.find("*", y * num);//定位y-1行皇后棋子的位置
				queen[clear] = '0';//用0覆盖原来的皇后位置

				x_start = clear % num + 1;//接下来尝试y-1行原来摆放皇后的下一个位置

				break;
			}
			if (insert_up(y, x, num) && insert_left_up(y, x, num) && insert_right_up(y, x, num))//由于本程序中是按照从上往下的顺序摆放皇后，所以
			{                                                                                   //只要正上方、左上方、右上方无皇后即可
				queen[y * num + x] = '*';//在(x,y)位置摆放皇后
				y++;
				x_start = 0;//这两行表示从下一行的第一个位置开始尝试

				if (y == num)//表明此时，已经完成了一种情况的摆放
				{
					count++;//摆放方法计数+1
					//*********************输出这种摆放方式********************
					cout << endl;
					for (int i = 0; i < num * num; i++)
					{
						cout << queen[i];
						if (((i + 1) % num) != 0)
							cout << ' ';
						else
							cout << endl;
					}
					cout << endl;
					
					//如果已经完成了一种摆放，那么下一次的尝试位置应该从(x+1,y)开始，而不是(0,y+1)
					y=num-1;
					x_start = x + 1;
					//下面两句是用来把上一个皇后的位置用字符 0 覆盖掉
					clear = queen.find("*", y * num);
					queen[clear] = '0';
				}	
				break;
			}
			else
				continue;
		}
		
	}
	cout << "共有 " << count << " 种解法!" << endl;
	//******************************************************************************************************
    //******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	_ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************
	return 0;
}



































































//@author:刘治华 2052134