#pragma once
#define INI -4//空棋盘格
#define X_NUM 9//棋盘行数
#define Y_NUM 9//棋盘列数
#define ESC_1 27//ESC虚拟建码1
#define ESC_2 -1//ESC虚拟建码2(自定义)
#define COLOR_NUM 6//棋子颜色种类
#define X_SET 160//控制台窗口行数设置
#define Y_SET 40//控制台窗口列数设置
#define XX_SET 160//控制台缓冲区行数设置
#define YY_SET 40//控制台缓冲区列数设置
#define SCORE_NUM 5//得分要求的最低连续棋子数
#define BEGIN_NUM 7//游戏起始开盘随机生成的棋子数
#define GAMING_NUM 3//游戏过程随机生成的棋子数
#define MARK 1000//记录分数的最大数量(数量级为3)
#define NOT -3//棋盘格被围，无法与外围联通的状态
#define COLOR_MIN -10//最初未转换前的最小颜色数字
#define COLOR_DIS 19//初始颜色数字转换到系统颜色数字需要额外加的值


/* 定义颜色（用宏定义取代数字，方便记忆） */
#define COLOR_BLACK		0	//黑
#define COLOR_BLUE		1	//蓝
#define COLOR_GREEN		2	//绿
#define COLOR_CYAN		3	//青
#define COLOR_RED		4	//红
#define COLOR_PINK		5	//粉
#define COLOR_YELLOW	6	//黄
#define COLOR_WHITE		7	//白
#define COLOR_HBLACK	8	//亮黑
#define COLOR_HBLUE		9	//亮蓝
#define COLOR_HGREEN	10	//亮绿
#define COLOR_HCYAN		11	//亮青
#define COLOR_HRED		12	//亮红
#define COLOR_HPINK		13	//亮粉
#define COLOR_HYELLOW	14	//亮黄
#define COLOR_HWHITE	15	//亮白

/* 定义鼠标键盘操作类型 */
#define CCT_MOUSE_EVENT				0
#define CCT_KEYBOARD_EVENT			1

/* 定义鼠标的操作 */
#define MOUSE_NO_ACTION						0x0000	//无鼠标操作
#define MOUSE_ONLY_MOVED					0x0001	//鼠标移动
#define MOUSE_LEFT_BUTTON_CLICK				0x0002	//按下左键
#define MOUSE_LEFT_BUTTON_DOUBLE_CLICK		0x0004	//双击左键
/* 定义光标的形态 */
#define CURSOR_VISIBLE_FULL					0	//光标显示，全高色块
#define CURSOR_VISIBLE_HALF					1	//光标显示，半高色块
#define CURSOR_VISIBLE_NORMAL				2	//光标显示，横线（缺省为此方式）
#define CURSOR_INVISIBLE					3	//光标不显示

/* cmd伪图形界面函数声明列表 */
//清屏
void cct_cls(void);
//允许使用鼠标
void cct_enable_mouse(void);
//功    能：将光标移动到指定位置
void cct_gotoxy(const int X, const int Y);
//设置光标状态（显示/不显示/全高/半高/横线等）
void cct_setcursor(const int options);
//功    能：设置指定的颜色
void cct_setcolor(const int bg_color, const int fg_color);
//获取当前cmd窗口的大小设置
void cct_getconsoleborder(int& cols, int& lines, int& buffer_cols, int& buffer_lines);
//读鼠标、键盘行为，并作出响应            
int my_read_keyboard_and_mouse(int& MX, int& MY, int& MAction, int& keycode1, int& keycode2);
//改变cmd窗口的大小及缓冲区的大小
void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines);
//获取当前鼠标坐标位置
void cct_getxy(int& x, int& y);
//画界面
void drawLeft();
void drawMiddle(int y);
void padding();
void drawRight();
void drawHeadline();
void drawAttribute();
void drawTextField();

int clickLocation(int x, int y);
void especiallyShow(int xy);
void selectFile(int state);