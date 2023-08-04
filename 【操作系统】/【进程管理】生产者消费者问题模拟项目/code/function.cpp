#include <iostream>
#include "function.h"
using namespace std;

extern int fullNum;
extern int emptyNum;
void input_headline() { //此函数功能：输出开头一段提示信息
	cout << "生产者消费者操作系统模拟项目:" << endl;
	cout << "五个生产者:P1、P2、P3、P4、P5，五个消费者:C1、C2、C3、C4、C5" << endl;
	cout << "C1只消费P1生产的数字，C2-C5与P2-P5的对应关系同理" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
	cout << "按一次回车键，模拟程序演示执行一步,红色状态表明当前存在阻塞进程，绿色状态表示当前无进程阻塞"<<endl;
	cout << "为了方便您验证程序对PV机制的功能模拟，已预设了一组进程队列，当然您也可以随时修改进程队列" << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void draw_buffer() { //此函数用于在屏幕上画出buffer框
	cct_gotoxy(2, 7);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "┎";
	for (int i = 0; i < MAX_BUFFER-1; ++i) {
		cout << " ─ "; 
		cout << "┰";
	}
	cout << " ─ ";
	cout << "┒ " << endl;
	cct_gotoxy(2, 8);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK); 
	for (int i = 0; i < MAX_BUFFER; ++i) {
		cout << "┃   ";
	}
	cout << "┃ " << endl;
	cct_gotoxy(2, 9);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "┖";
	for (int i = 0; i < MAX_BUFFER-1; ++i) {
		cout << " ─ ";
		cout << "┸";
	}
	cout << " ─ ";
	cout << "┚ " << endl << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

