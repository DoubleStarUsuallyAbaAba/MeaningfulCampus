#include <iostream>
#include <string>
#include "class.h"
using namespace std;

int main()
{
	int number, start, killer, survivor;//分别表示游戏总人数、报数起始位置，死亡数字、幸存者人数
	string bin;//作用是吃掉多余的非法输入，功能类似于垃圾桶，故命此名
	List<int> list(-1,1);
	cout << "现在有N个人围成一圈，从第S个人开始依次报数，报M的人出局，再由下一人开始报数，"
		"如此循环，直至剩下K个人为止"<< endl;
	cout << "请输入约瑟夫生死游戏的总人数N (2<=N<=1000): ";

	while (1)//处理非法输入
	{
		cin >> number;
		if (cin.fail() || number < 2||number>1000)//输入非数字字符或输入的人数是负数或者参加游戏的人只有1个，则没必要进行游戏，重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入非数字字符或数字小于2或数字太大,无效,请重新输入游戏人数N: ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	cout << "请输入报数开始的位置S (1<=S<=N) :";
	while (1)//处理非法输入
	{
		cin >> start;
		if (cin.fail() || start < 1||start>number)//输入非数字字符或输入的起始位置是非正数或者报数起始位置已经超过游戏总人数，重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入非数字字符或不在游戏人数范围内,无效，请重新输入报数开始位置S: ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	cout << "请输入死亡数字M (2<=M<=2147483647): ";
	while (1)//处理非法输入
	{
		cin >> killer;
		if (cin.fail() || killer < 2)//输入非数字字符或者死亡数字是1，那就没有进行游戏的必要，重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入非数字字符或小于2的死亡数字,无效, 请重新输入死亡数字M: ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	cout << "请输入剩余的生者人数K (1<=K<N): ";
	while (1)//处理非法输入
	{
		cin >> survivor;
		if (cin.fail() || survivor >=number||survivor<1)//输入非数字字符、幸存者人数不少于游戏总人数、幸存者人数是负数，不合常理，重新输入
		{
			cin.clear();
			getline(cin, bin);
			cout << "输入非数字字符或幸存者人数不少于游戏总参与人数或者输入了非正数,无效, 请重新输入生者人数K: ";
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	for (int i = 1; i < number; i++)//为游戏参与者建立链表
		list.Insert(i + 1, i);
	list.Create_Circle();//构造链表为单循环链表
	list.Joseph(start-1, number - survivor, killer);//模拟约瑟夫游戏规则，删除该淘汰者的节点

	//******************************************************************************************************
    //******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************

	return 0;
}