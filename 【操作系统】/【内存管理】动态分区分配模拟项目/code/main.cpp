#include <iostream>
#include <vector>
#include <string>
#include "function.h"
using namespace std;
#define CLEARNUM 13   //需要清空的行数

struct ListNode {  //一个空闲区表或已分配区表的节点
	int start;     //始值
	int length;    //长度
	string status; //标志

	ListNode(int a = -1, int b = -1, string c = "\0") {
		start = a;
		length = b;
		status = c;
	}
};
struct Process {  //一个进程
	string name;  //进程标识名
	int size;     //需要的内存大小
	Process(string a, int b) {
		name = a;
		size = b;
	}
};
void SortAddress(vector<ListNode>* array) //将已分配区表中节点按照始值升序排列
{
	int len = array->size();
	for (int i = 1; i < len; i++)
	{
		ListNode temp = (*array)[i];
		int j = i;
		while (j > 0 && temp.start < (*array)[j - 1].start)
		{
			(*array)[j] = (*array)[j - 1];
			j--;
		}
		if (j != i)
			(*array)[j] = temp;
	}
}
void InsertSort(vector<ListNode>* array,string flag="up")//将空闲区区表中节点按照长度排列
{                                                        //最佳适配算法则升序排列
	int len = array->size();                             //最坏适配算法则降序排列
	for (int i = 1; i < len; i++)
	{
		ListNode temp = (*array)[i];
		int j = i;
		if (flag == "up") {
			while (j > 0 && temp.length < (*array)[j - 1].length)
			{
				(*array)[j] = (*array)[j - 1];
				j--;
			}
			if (j != i)
				(*array)[j] = temp;
		}
		else {
			while (j > 0 && temp.length > (*array)[j - 1].length)
			{
				(*array)[j] = (*array)[j - 1];
				j--;
			}
			if (j != i)
				(*array)[j] = temp;
		}
	}
}
void worstFit(vector<ListNode>* EList, vector<ListNode>* BList, Process* process) { //最坏适配算法
	if (EList->size() == 0)               //将空闲区表按照长度降序排列，优先将进程放入最大的空闲区
		return;
	int num = EList->size();
	InsertSort(EList,"down");
	if ((*EList)[0].length >= process->size) {
		BList->push_back(ListNode((*EList)[0].start, process->size, process->name));
		if ((*EList)[0].length == process->size) {
			EList->erase(EList->begin() + 0, EList->begin() + 1);
		}
		else {
			(*EList)[0].start += process->size;
			(*EList)[0].length -= process->size;
		}
	}
	SortAddress(BList);
}
void bestFit(vector<ListNode>* EList, vector<ListNode>* BList, Process* process) { //最佳适配算法
	if (EList->size() == 0)     //将空闲区表按照长度升序排列，优先将进程放入第一个不小于自身所需空间的空闲区
		return;
	if (EList->size() == 1) {
		if ((*EList)[0].length >= process->size) {
			BList->push_back(ListNode((*EList)[0].start, process->size, process->name));
			if ((*EList)[0].length == process->size) {
				EList->erase(EList->begin() + 0, EList->begin() + 1);
			}
			else {
				(*EList)[0].start += process->size;
				(*EList)[0].length -= process->size;
			}
		}
		SortAddress(BList);
		return;
	}
	int num = EList->size();
	InsertSort(EList,"up");
	int location = -1;
	for (int i = 0; i < num; ++i) {
		if ((*EList)[i].length >= (*process).size) {
			location = i;
			break;
		}
	}
	if (location != -1) {
		BList->push_back(ListNode((*EList)[location].start, process->size, process->name));
		if ((*EList)[location].length == process->size) {
			EList->erase(EList->begin() + location, EList->begin() + location + 1);
		}
		else {
			(*EList)[location].start += process->size;
			(*EList)[location].length -= process->size;
		}
	}
	SortAddress(BList);
}
void killProcess(vector<ListNode>* EList, vector<ListNode>* BList,string flag) { //紧凑算法
	int bnum = BList->size();                //当一个进程结束之后，需要释放所占用的内存空间
	int location = -1;                       //若即将要释放的空间可以和未分配的空闲区间相邻
	int start = -1;                          //则将两个区间合并，形成一个较大容量的空闲区间
	int length = -1;
	int end = -1;
	if (bnum == 0) {
		return;
	}
	for (int i = 0; i < bnum; ++i) {
		if ((*BList)[i].status == flag) {
			location = i;
			break;
		}
	}
	if (location != -1) {
		start = (*BList)[location].start;
		length = (*BList)[location].length;
		end = start + length;
		BList->erase(BList->begin() + location, BList->begin() + location + 1);
		for (unsigned int i = 0; i < EList->size(); ++i) {
			if ((*EList)[i].start == end) {
				length += (*EList)[i].length;
				end += (*EList)[i].length;
				EList->erase(EList->begin() + i, EList->begin() + i + 1);
				--i;
			}
			else if ((*EList)[i].start + (*EList)[i].length == start) {
				start = (*EList)[i].start;
				length += (*EList)[i].length;
				EList->erase(EList->begin() + i, EList->begin() + i + 1);
				--i;
			}
		}
		EList->push_back(ListNode(start, length, flag));
	}
	SortAddress(BList);
}
void clear() { //对输出信息做清屏操作
	cct_gotoxy(0, 13);
	for (int j = 0; j < CLEARNUM; ++j) {
		for (int k = 0; k < 8; ++k) {
			cout << "          "; //10个空格
		}
		cout << endl;
	}
	cct_gotoxy(0, 13);
}
void upDateBuffer(vector<ListNode>* BusyList) {  //更新内存空间的画面
	int len = BusyList->size();
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);     //下面是清除旧的画面
	cct_gotoxy(3, 5);
	for (int i = 0; i < 7; ++i) {
		cout << "          ";  //10个空格
	}
	cout << "         ";  //9个空格
	cct_gotoxy(3, 6);
	for (int i = 0; i < 7; ++i) {
		cout << "          ";  //10个空格
	}
	cout << "         ";  //9个空格
	cct_gotoxy(3, 7);
	for (int i = 0; i < 7; ++i) {
		cout << "          ";  //10个空格
	}
	cout << "         ";  //9个空格
	cct_gotoxy(3, 9);
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	for (int i = 0; i < 7; ++i) {
		cout << "          ";  //10个空格
	}
	cout << "         ";  //9个空格
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);       //下面是重新绘画新的画面
	for (int i = 1; i <= len; ++i) {
		cct_gotoxy(2 + ((*BusyList)[i - 1].start) / 32 * 4, 5);
		cout << "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start) / 32 * 4, 6);
		cout << "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start) / 32 * 4, 7);
		cout << "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start) / 32 * 4, 9);
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		cout << (*BusyList)[i - 1].start;
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		cct_gotoxy(2 + ((*BusyList)[i-1].start*2+ (*BusyList)[i - 1].length)*4/64+1, 6);
		cout << (*BusyList)[i - 1].status;
		cct_gotoxy(2 + ((*BusyList)[i - 1].start + (*BusyList)[i - 1].length) / 32 * 4, 5);
		cout<< "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start + (*BusyList)[i - 1].length) / 32 * 4, 6);
		cout << "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start + (*BusyList)[i - 1].length) / 32 * 4, 7);
		cout << "┃";
		cct_gotoxy(2 + ((*BusyList)[i - 1].start + (*BusyList)[i - 1].length) / 32 * 4, 9);
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		cout << (*BusyList)[i - 1].start + (*BusyList)[i - 1].length;
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
string chooseAlgorithm() { //供用户选择要使用的动态分配算法种类
	clear();
	string choice;
	cct_gotoxy(0, 12);
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	cout << "选择最坏适配算法请输入1后敲回车键，选择最佳适配算法请输入2后敲回车键，结束程序请输入数字3后敲回车键:";
	while (1) {
		getline(cin,choice);
		if (choice != "1" && choice != "2" && choice != "3") { //非法的无效输入，不予响应
			cct_gotoxy(0, 12);
			for (int i = 0; i < 12; ++i) {
				cout << "          "; //10个空格
			}
			cct_gotoxy(0, 12);
			cout << "选择最坏适配算法请输入1后敲回车键，选择最佳适配算法请输入2后敲回车键，结束程序请输入数字3后敲回车键:";
		}
		else {
			cct_gotoxy(0, 12);
			for (int i = 0; i < 12; ++i) {
				cout << "          "; //10个空格
			}
			cct_gotoxy(0, 12);
			if (choice == "1") {
				cout << "最坏适配算法模拟演示:";
			}
			else if (choice == "2") {
				cout << "最佳适配算法模拟演示:";
			}
			break;
		}
	}
	cct_setcursor(CURSOR_INVISIBLE);
	return choice;
}
int main() {
	cout << "操作系统内存管理之动态分区分配模拟演示程序(可用内存空间初始值：640K)"<<endl;
	int capacity = 0;
	vector<ListNode> EmptyList;
	vector<ListNode> BusyList;
	vector<Process> ProcessList;               //为了方便体现与演示，设置了一个进程序列
	ProcessList.push_back(Process("A", 64));   //A--64K,B--320K,C--128K,D--128K,E--160K,F--32K,G--64K,H--32K,I--224K
	ProcessList.push_back(Process("B", 320));
	ProcessList.push_back(Process("C", 128));
	ProcessList.push_back(Process("D", 128));
	ProcessList.push_back(Process("E", 160));
	ProcessList.push_back(Process("F", 32));
	ProcessList.push_back(Process("G", 64));
	ProcessList.push_back(Process("H", 32));
	ProcessList.push_back(Process("I", 224));
	cout << "进程序列: ";
	for (unsigned int i = 0; i < ProcessList.size(); ++i) {
		cout << ProcessList[i].name << "--" << ProcessList[i].size << "K  ";
	}
	cout << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HGREEN);
	cout << "敲击回车键一次，程序单步执行,分配一个进程";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	draw_buffer();
	int num = ProcessList.size();
	while (1) {
		EmptyList.clear();
		BusyList.clear();
		EmptyList.push_back(ListNode(0, 640));
		string choice = chooseAlgorithm();
		if (choice == "3") {
			break;
		}
		for (int i = 0; i < num; ++i) { //按照用户选择的分配算法顺序分配进程
			if (choice == "1") {
				worstFit(&EmptyList, &BusyList, &(ProcessList[i]));
				InsertSort(&EmptyList, "down");
			}
			else {
				bestFit(&EmptyList, &BusyList, &(ProcessList[i]));
				InsertSort(&EmptyList, "up");
			}
			upDateBuffer(&BusyList);
			clear();
			cout << "空闲区表"; //显示未分配的空闲区表
			if (i < num - 1) {
				cout << "(即将分配:" << ProcessList[i + 1].name << "---" << ProcessList[i + 1].size << "K)" << endl;
			}
			else {
				cout << "进程分配完毕" << endl;
			}
			cout << "始址    长度      标志" << endl;
			for (unsigned int j = 0; j < EmptyList.size(); ++j) {
				if (EmptyList[j].start < 10) {
					cout << "  ";
				}
				else if (EmptyList[j].start < 100) {
					cout << ' ';
				}
				cout << EmptyList[j].start << (EmptyList[j].length < 100 ? "      " : "     ") << EmptyList[j].length << "      未分配" << endl;
			}
			if (i == 2) {  //B进程结束，释放B进程所占用的内存空间
				int _ = getchar();
				killProcess(&EmptyList, &BusyList, "B");
				if (choice == "1") {
					InsertSort(&EmptyList, "down");
				}
				else{
					InsertSort(&EmptyList, "up");
				}
				upDateBuffer(&BusyList);
				clear();
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "空闲区表(B进程结束)" << endl; //更新显示未分配的空闲区表
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cout << "始址    长度      标志" << endl;
				for (unsigned int j = 0; j < EmptyList.size(); ++j) {
					if (EmptyList[j].start < 10) {
						cout << "  ";
					}
					else if (EmptyList[j].start < 100) {
						cout << ' ';
					}
					cout << EmptyList[j].start << (EmptyList[j].length < 100 ? "      " : "     ") << EmptyList[j].length << "      未分配" << endl;
				}
			}
			else if (i == 3) {
				cct_gotoxy(0, 11);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "在不发生替换时，最坏适配算法已无法分配超过192K的进程，但最佳适配算法可以分配不超过320K的进程";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
			else if (i == 6) {
				int _ = getchar();     //A进程结束，释放A进程所占用的内存空间
				killProcess(&EmptyList, &BusyList, "A");
				if (choice == "1") {
					InsertSort(&EmptyList, "down");
				}
				else {
					InsertSort(&EmptyList, "up");
				}
				upDateBuffer(&BusyList);
				clear();
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "空闲区表(A进程结束)" << endl;  //更新显示未分配的空闲区表
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cout << "始址    长度      标志" << endl;
				for (unsigned int j = 0; j < EmptyList.size(); ++j) {
					if (EmptyList[j].start < 10) {
						cout << "  ";
					}
					else if (EmptyList[j].start < 100) {
						cout << ' ';
					}
					cout << EmptyList[j].start << (EmptyList[j].length < 100 ? "      " : "     ") << EmptyList[j].length << "      未分配" << endl;
				}
			}
			else if (i == 7) {
				int _ = getchar();          //D、E进程结束，释放D、E进程所占用的内存空间
				killProcess(&EmptyList, &BusyList, "D");
				killProcess(&EmptyList, &BusyList, "E");
				if (choice == "1") {
					InsertSort(&EmptyList, "down");
				}
				else {
					InsertSort(&EmptyList, "up");
				}
				upDateBuffer(&BusyList);
				clear();
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "空闲区表(DE进程结束)" << endl;  //更新显示未分配的空闲区表
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cout << "始址    长度      标志" << endl;
				for (unsigned int j = 0; j < EmptyList.size(); ++j) {
					if (EmptyList[j].start < 10) {
						cout << "  ";
					}
					else if (EmptyList[j].start < 100) {
						cout << ' ';
					}
					cout << EmptyList[j].start << (EmptyList[j].length < 100 ? "      " : "     ") << EmptyList[j].length << "      未分配" << endl;
				}
			}
			else if (i == 8) {
				cct_gotoxy(0, 11);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "刚刚最坏适配算法只需要查找1次，但最佳适配算法需要3次";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
			int _ = getchar();
			cct_gotoxy(0, 11);
			for (int j = 0; j < 12; ++j) {
				cout << "          ";  //10个空格
			}
		}
	}
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	system("pause");
	return 0;
}