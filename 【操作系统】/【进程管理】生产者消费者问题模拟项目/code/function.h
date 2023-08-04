#pragma once
#define MAX_BUFFER 20
/* 系统中数字对应颜色 */
#define COLOR_BLACK		0	//黑
#define COLOR_WHITE		7	//白
#define COLOR_HBLUE		9	//亮蓝
#define COLOR_HGREEN	10	//亮绿
#define COLOR_HCYAN		11	//亮青
#define COLOR_HRED		12	//亮红
#define COLOR_HPINK		13	//亮粉
#define COLOR_HYELLOW	14	//亮黄
#define COLOR_HWHITE	15	//亮白

/* 定义光标的形态 */
#define CURSOR_VISIBLE_FULL					0	//光标显示，全高色块
#define CURSOR_VISIBLE_HALF					1	//光标显示，半高色块
#define CURSOR_VISIBLE_NORMAL				2	//光标显示，横线（缺省为此方式）
#define CURSOR_INVISIBLE					3	//光标不显示

/* cmd伪图形界面函数声明列表 */
//清屏
void cct_cls(void);
//功    能：将光标移动到指定位置
void cct_gotoxy(const int X, const int Y);
//设置光标状态（显示/不显示/全高/半高/横线等）
void cct_setcursor(const int options);
//功    能：设置指定的颜色
void cct_setcolor(const int bg_color, const int fg_color);
//获取当前cmd窗口的大小设置
void cct_getconsoleborder(int& cols, int& lines, int& buffer_cols, int& buffer_lines);
//改变cmd窗口的大小及缓冲区的大小
void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines);

void input_headline();
void draw_buffer();
void update_buffer();
