#include <iostream>
#include <Windows.h>
#include "function.h"
using namespace std;
#define MAX_BUFFER 20

//extern int fullNum;
//extern int emptyNum;

void draw_buffer() { //此函数用于在屏幕上画出buffer框
	cct_gotoxy(2, 7);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "┎";
	for (int i = 0; i < MAX_BUFFER - 1; ++i) {
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
	for (int i = 0; i < MAX_BUFFER - 1; ++i) {
		cout << " ─ ";
		cout << "┸";
	}
	cout << " ─ ";
	cout << "┚ " << endl << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}

void drawLeft() {
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	for (int j = 0; j < 4; ++j) {
		cct_gotoxy(0, j*3);
		cout << "┎";
		for (int i = 0; i < 8; ++i) {
			cout << "─";
		}
		cout << "┒" << endl;   //末尾坐标（19,0）
		cout << "┃";
		if (j == 0) {
			cout << "  回退上级目录  ";
		}
		else if (j == 1) {
			cout << "      X盘       ";
		}
		else if (j == 2) {
			cout << "      Y盘       ";
		}
		else if (j == 3) {
			cout << "      Z盘       ";
		}
		cout << "┃" << endl;
		cout << "┖";
		for (int i = 0; i < 8; ++i) {
			cout << "─";
		}
		cout << "┚";
	}
	for (int i = 0; i < 15; ++i) {
		cct_gotoxy(0, 12+i);
		cout << "                    "; //20个空格
	}
	cct_gotoxy(0, 21);
	cout << "┎";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┒" << endl;   //末尾坐标（19,0）
	cout << "┃";
	cout << " 上滑右侧文件区 ";
	cout << "┃" << endl;
	cout << "┖";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┚" << endl;
	cout << "┎";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┒" << endl;   //末尾坐标（19,0）
	cout << "┃";
	cout << " 下滑右侧文件区 ";
	cout << "┃" << endl;
	cout << "┖";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┚";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void padding() {
	cct_setcolor(COLOR_HWHITE, COLOR_HWHITE);
	for (int j = 3; j < 27; ++j) {
		cct_gotoxy(20, j);
		for (int i = 0; i < 12; ++i) {
			cout << "          "; //10个空格
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void drawHeadline() {
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	cct_gotoxy(20, 0);
	cout << "┎";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┒";   //末尾坐标（159,0）
	cct_gotoxy(20, 1);
	cout << "┃";
	cout << "当前目录:       ";
	for (int i = 0; i < 10; ++i) {
		cout << "          "; //10个空格
	}
	cout << "┃";
	cct_gotoxy(20, 2);
	cout << "┖";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┚";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void drawMiddle(int y) {
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cct_gotoxy(20, (y+1) * 3);
	cout << "┎";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┒";   //末尾坐标（159,0）
	cct_gotoxy(20, (y + 1) * 3+1);
	cout << "┃";
	cct_gotoxy(138, (y + 1) * 3 + 1);
	cout << "┃";
	cct_gotoxy(20, (y + 1) * 3 + 2);
	cout << "┖";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┚";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void drawRight() {
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	for (int j = 0; j < 5; ++j) {
		cct_gotoxy(140, j * 3);
		cout << "┎";
		for (int i = 0; i < 8; ++i) {
			cout << "─";
		}
		cout << "┒";   //末尾坐标（159,0）
		cct_gotoxy(140, j * 3+1);
		cout << "┃";
		if (j == 0) {
			cout << "      打开      ";
		}
		else if (j == 1) {
			cout << "      关闭      ";
		}
		else if (j == 2) {
			cout << "      删除      ";
		}
		else if (j == 3) {
			cout << "    新建文件    ";
		}
		else if (j == 4) {
			cout << "   新建文件夹   ";
		}
		cout << "┃" ;
		cct_gotoxy(140, j * 3 + 2);
		cout << "┖";
		for (int i = 0; i < 8; ++i) {
			cout << "─";
		}
		cout << "┚";
	}
	for (int i = 0; i < 12; ++i) {
		cct_gotoxy(140, 15 + i);
		cout << "                    "; //20个空格
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void drawAttribute() {
	cct_setcolor(COLOR_HCYAN, COLOR_BLACK);
	cct_gotoxy(0, 27);
	for (int i = 0; i < 80; ++i) {
		cout << "─";
	}
	cct_gotoxy(0, 28);
	cout << " 下方为文本输入框:";
	for (int i = 0; i < 15; ++i) {
		cout << "          "; //10个空格
	}
	cct_gotoxy(0, 29);
	for (int i = 0; i < 80; ++i) {
		cout << "─";
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
void drawTextField() {
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cct_gotoxy(0, 30);
	for (int i = 0; i < 80; ++i) {
		cout << "─";
	}
	for (int j = 0; j < 7; ++j) {
		cct_gotoxy(0, 31+j);
		for (int i = 0; i < 16; ++i) {
			cout << "          "; //10个空格
		}
		cout << endl;
	}
	cct_gotoxy(0, 38);
	for (int i = 0; i < 80; ++i) {
		cout << "─";
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}
int clickLocation(int x, int y) {
	if (x >= 1 && x <= 18) {
		if (y >= 0 && y <= 2) {
			return 0;
		}
		else if (y >= 3 && y <= 5) {
			return 3;
		}
		else if (y >= 6 && y <= 8) {
			return 6;
		}
		else if (y >= 9 && y <= 11) {
			return 9;
		}
		else if (y >= 21 && y <= 23) {
			return 21;
		}
		else if (y >= 24 && y <= 26) {
			return 24;
		}
		else {
			return -3;
		}
	}
	else if (x >= 141 && x <= 158) {
		if (y >= 0 && y <= 2) {
			return 140;
		}
		else if (y >= 3 && y <= 5) {
			return 143;
		}
		else if (y >= 6 && y <= 8) {
			return 146;
		}
		else if (y >= 9 && y <= 11) {
			return 149;
		}
		else if (y >= 12 && y <= 14) {
			return 152;
		}
		else {
			return -3;
		}
	}
	else if (x >= 21 && x <= 138 && y>=3 && y<=26) {
		return 200 + y / 3;
	}
	else {
		return -3;
	}
}
void especiallyShow(int xy) {
	int x = -1;
	int y = -1;
	int xMouse = -1;
	int yMouse = -1;
	cct_getxy(xMouse, yMouse);
	if (0<=xy && xy<=24) {
		x = 0;
		y = xy;
	}
	else if (140 <= xy && xy <= 158) {
		x = 140;
		y = xy - 140;
	}
	cct_setcolor(COLOR_HBLUE, COLOR_BLACK);
	cct_gotoxy(x, y);
	cout << "┎";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┒";
	cct_gotoxy(x, y + 1);
	cout << "┃";
	for (int i = 0; i < 16; ++i) {
		cout << ' ';
	}
	//cct_gotoxy(x + 18, y + 1);
	cout << "┃";
	cct_gotoxy(x, y + 2);
	cout << "┖";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┚";
	Sleep(200);
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	cct_gotoxy(x, y);
	cout << "┎";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┒";
	cct_gotoxy(x, y + 1);
	cout << "┃";
	for (int i = 0; i < 16; ++i) {
		cout << ' ';
	}
	//cct_gotoxy(x + 18, y + 1);
	cout << "┃";
	cct_gotoxy(x, y + 2);
	cout << "┖";
	for (int i = 0; i < 8; ++i) {
		cout << "─";
	}
	cout << "┚";
	if (0 <= xy && xy <= 24) {
		drawLeft();
	}
	else if (140 <= xy && xy <= 158) {
		drawRight();
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(xMouse, yMouse);
}
void selectFile(int state) {
	cct_setcolor(COLOR_HBLUE, COLOR_BLACK);
	cct_gotoxy(20, state * 3);
	cout << "┎";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┒";   //末尾坐标（159,0）
	cct_gotoxy(20, state* 3 + 1);
	cout << "┃";
	cct_gotoxy(138, state * 3 + 1);
	cout << "┃";
	cct_gotoxy(20, state * 3 + 2);
	cout << "┖";
	for (int i = 0; i < 58; ++i) {
		cout << "─";
	}
	cout << "┚";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 31);
}