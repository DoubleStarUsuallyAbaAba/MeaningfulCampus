#include <iostream>
#include <string>
using namespace std;
#define SIZE 100//宏定义关系矩阵的最大尺寸
void output(int s[][SIZE]);//输出关系矩阵
void zifan(int s2[][SIZE]);//求自反闭包
void duichen(int s2[][SIZE]);//求对称闭包
void chuandi(int s2[][SIZE]);//求传递闭包
void select();//选择是否继续执行程序以及计算何种闭包
int s[SIZE][SIZE], s1[SIZE][SIZE], choice;
int num, i, j;
string bin;
int main()
{
	select();
	system("pause");
	return 0;
}

void select()
{
	cout << "请选择是否执行计算程序，继续则输入Y,否则输入N:  ";
	while (1)//此处的while循环用于判断是否存在非法输入，并进行处理
	{
		cin >> bin;
		if (bin!="Y"&&bin!="N")//无效输入，需要重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入不规范，请重新输入:  ";
			continue;
		}
		else
		{
			if (bin == "Y")
			{
				getline(cin, bin);//此句用于吃掉可能存在的多余无效输入
				break;
			}
			else
			{
				system("pause");
				exit(0);//退出程序
			}
		}
	}
	cout << "请输入方阵的维数("<< SIZE<<"维以内): ";
	while (1)//此处的while循环用于判断是否存在非法输入，并进行处理
	{
		cin >> num;
		if (cin.fail() || num > SIZE||num<1)//无效输入，需要重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入不规范，请重新输入:  ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}
	cout << "请输入关系矩阵:" << endl<<endl;
	for (i = 0; i < num; i++)
	{
		cout << "请输入矩阵的第" << i << "行元素(元素以空格分隔) :";
		for (j = 0; j < num; j++)
			cin >> s[i][j];
		cout << endl;
	}
	cout << "输入对应序号选择算法\nl:自反闭包\n2:传递闭包\n3:对称闭包\n4:退出\n";
	cin >> choice;
	switch (choice)
	{
	    case 1:
			zifan(s); //求自反闭包
			break;
	    case 2:
			chuandi(s); //求传递闭包
			break;
	    case 3:
			duichen(s); //求对称闭包
			break;
	    default:
			system("pause");
			exit(0); //退出程序
	}
}
void clear(int s[][SIZE])//重置矩阵
{
	for (int i = 0; i < num; i++)
		for (int j = 0; j < num; j++)
			s[i][j] = 0;
}
void output(int s[][SIZE])//遍历二维数组，输出关系矩阵
{
	cout << "所求关系矩阵为: "<<endl;
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
			cout << s[i][j];
		cout << endl;
	}
	cout << endl << endl;
}

void zifan(int s2[][SIZE])//构造自反闭包只需要在主对角线元素逻辑加1
{
	for (i = 0; i < num; i++)
		s2[i][i] = 1;
	output(s2);//输出结果
	clear(s2);//重置矩阵
	select();
}

void duichen(int s2[][SIZE])
{
	for (i = 0; i < num; i++)//构造转置矩阵
		for (j = 0; j < num; j++)
			s1[j][i] = s2[i][j];
	for (i = 0; i < num; i++)//原矩阵和转置矩阵的对应位置元素进行逻辑加
		for (j = 0; j < num; j++)
		{
			s2[i][j] = s2[i][j] + s1[i][j];
			if (s2[i][j] > 1)
				s2[i][j] = 1;
		}
	output(s2);//输出结果
	clear(s1);//重置矩阵
	clear(s2);
	select();
}

void chuandi(int s2[][SIZE])//构造传递闭包
{
	int k, h;
	for (h = 0; h < num; h++)//n维矩阵的次幂只有n种情况，也就是说，n+1次幂一定和1-n次幂中的一种相同
	{                        //又因为逻辑加两个相同矩阵结果还是原来的矩阵，所以累加n次幂必然有结果
		for (i = 0; i < num; i++)//对矩阵n次幂与前n-1次幂累加的模拟
			for (j = 0; j < num; j++)
				if (s2[i][j] == 1)
				{
					for (k = 0; k < num; k++)
						if (s2[j][k] == 1)
							s2[i][k] = 1;
				}
	}
	output(s2);//输出结果
	clear(s2);//重置矩阵
	select();
}
