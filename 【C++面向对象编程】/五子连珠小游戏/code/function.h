#pragma once
#define INI -4//空棋盘格
#define X_NUM 9//棋盘行数
#define Y_NUM 9//棋盘列数
#define ESC_1 27//ESC虚拟建码1
#define ESC_2 -1//ESC虚拟建码2(自定义)
#define COLOR_NUM 6//棋子颜色种类
#define X_SET 120//控制台窗口行数设置
#define Y_SET 30//控制台窗口列数设置
#define XX_SET 120//控制台缓冲区行数设置
#define YY_SET 30//控制台缓冲区列数设置
#define SCORE_NUM 5//得分要求的最低连续棋子数
#define BEGIN_NUM 7//游戏起始开盘随机生成的棋子数
#define GAMING_NUM 3//游戏过程随机生成的棋子数
#define MARK 1000//记录分数的最大数量(数量级为3)
#define NOT -3//棋盘格被围，无法与外围联通的状态
#define COLOR_MIN -10//最初未转换前的最小颜色数字
#define COLOR_DIS 19//初始颜色数字转换到系统颜色数字需要额外加的值


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

class City//棋盘棋子间的各种行为类似于按地图在城市各地点间的流通，故命此名
{
public:
	City();
	void sort();//得分排序
	void show_score();//展示分数
	void Draw_a_board();//画棋盘
	void Show_all_pieces();//画棋子
	void set();//重置map数组至初始状态
	void Kill_pieces(int& number);//消除连珠
	void continuity(int state);//判断连珠并计分
	void Current_score();//显示当前游戏的实时分数
	int born(int num);//生成随机数目、随机颜色的棋子
	void Show_way(int x1,int y1,int x2,int y2);//画路线
	void Especially_show(int x, int y, int state);//突出显示起点与终点
	void Move(int x1, int y1, int x2, int y2);//对调起点珠子和终点空棋盘图形的位置
	int shortest_way(int x_start, int y_start, int x_finish, int y_finish);//计算最短路径
	int Wall(int i, int j, int kind, int x_start, int y_start, int x_finish, int y_finish);//判断棋盘格是否被棋子包围
	int count(int target, int angle, int x = 0, int y = 0, int state = 1);//计算棋盘中目标棋子的数目、分担判断连珠与计分的功能
	friend int control();//control函数被main函数调用
private:
	int map[X_NUM][Y_NUM],score_sort[MARK];
	static int score,score_num;
};