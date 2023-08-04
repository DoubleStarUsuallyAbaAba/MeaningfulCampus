#include <iostream>
#include "function.h"
using namespace std;

void draw_buffer() { //此函数用于在屏幕上画出内存框
	cct_gotoxy(2, 4);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "┎";
	for (int i = 0; i < MAX_BUFFER - 1; ++i) {
		cout << " ─ ─ ─ ─";
	}
	cout << "─ ─ ─ ─";
	cout << "┒ " << endl;
	cct_gotoxy(2, 5);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int i = 0; i < MAX_BUFFER; ++i) {
		if (i == 0) {
			cout << "┃";
		}
		else {
			cout << " ";
		}
		cout << "       ";  //7个空格
	}
	cout << "┃ " << endl;
	cct_gotoxy(2, 6);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int i = 0; i < MAX_BUFFER; ++i) {
		if (i == 0) {
			cout << "┃";
		}
		else {
			cout << " ";
		}
		cout << "       ";  //7个空格
	}
	cout << "┃ " << endl;
	cct_gotoxy(2, 7);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	for (int i = 0; i < MAX_BUFFER; ++i) {
		if (i == 0) {
			cout << "┃";
		}
		else {
			cout << " ";
		}
		cout << "       ";  //7个空格
	}
	cout << "┃ " << endl;
	cct_gotoxy(2, 8);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "┖";
	for (int i = 0; i < MAX_BUFFER - 1; ++i) {
		cout << " ─ ─ ─ ─";
	}
	cout << "─ ─ ─ ─";
	cout << "┚ " << endl << endl;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(2, 9);
	cout << 0;
	cct_gotoxy(2+4*20, 9);
	cout << 640;
}
