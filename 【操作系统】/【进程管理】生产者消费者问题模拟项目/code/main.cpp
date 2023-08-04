#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <windows.h>
#include "function.h"
using namespace std;

struct Process {//进程
	bool state;    //进程状态，阻塞为false，就绪或执行为true
	string address;//被阻塞到哪个信号量阻塞队列
	string PID;    //进程标识符
	Process(string pid, bool a = true, string b = "\0") {
		PID = pid;
		state = a;
		address = b;
	}
};
struct Buffer {//缓冲区单元
	int num;      //缓冲区单元存储的数字
	string owner; //操作缓冲区单元的进程标号
	Buffer(int a = 0, string b = "\0") {
		num = a;
		owner = b;
	}
};

int emptyNum = 20;    //信号量，表示缓冲区空格数目
queue<string> Empty;  //empty对应的阻塞队列
int fullNum = 0;      //信号量，表示缓冲区被填充的单元数目
queue<string> Full;   //full对应的阻塞队列
int mutex = 1;        //信号量，用于保持进程间的互斥关系
queue<string> Mutex;  //mutex对应的阻塞队列
int sindex[5] = { 0 };//用数组表示s1-s5五个信号量
queue<string> S1, S2, S3, S4, S5;//信号量s1-s5对应的5个阻塞队列
                                 //5个生产者进程: P1-P5,5个消费者进程C1-C5
Process P1("P1"), C1("C1"), P2("P2"), C2("C2"), P3("P3"), C3("C3"), P4("P4"), C4("C4"), P5("P5"), C5("C5");
unordered_map<int, Process*> producer;//生产者进程P1-P5分别用数字1-5进行映射
unordered_map<int, Process*> consumer;//消费者进程C1-C5分别用数字1-5进行映射
unordered_map<int, queue<string>*> S; //阻塞队列S1-S5分别用数字1-5进行映射
Buffer buffer[20];     //用数组表示缓冲区
queue<string> Ready;   //就绪队列
queue<int> ReadyLevel; //就绪队列中各进程即将执行时的信号量等级
queue<string> copyTmp; //一个中间队列，方便队列排序
int showBlue = 0;      //记录当前被从阻塞队列中重新唤醒的进程数量

template <typename T>
void ReSort(queue<T>* q) {   //队列重排序，把队列中的最后一个进程提到最前面
	queue<T> tmp;
	if ((*q).size() < 2) {   //如果队列中只有一个或没有元素，重不重新排序效果一样
		return;
	}
	else {
		tmp.push((*q).back());      //先在新队列压入最后一个元素
		while ((*q).size() > 1) {   //然后再按照原队列元素顺序将各元素压入新队列
			tmp.push((*q).front());
			(*q).pop();
		}
		(*q).pop();
		while (!tmp.empty()) {      //相当于把排好序的中间队列复制给原队列
			(*q).push(tmp.front());
			tmp.pop();
		}
	}
}

string P(int* flag, queue<string>* q,string q_name,Process* process,int level) {//模拟P原语
	if (ReadyLevel.front() <= level) {      //此处level变量的意义：例如:进程A被阻塞在empty信号量的阻塞队列
		(*flag) -= 1;                       //当A进程被唤醒后下一步触发的应该是下一条P原语，如：P(mutex)
	}                                       //而不是再触发一次P(empty)
	if ((*flag) < 0 && ReadyLevel.front()<=level) {
		(*q).push((*process).PID);          //当信号量<0，该进程被阻塞到信号量对应的阻塞队列中
		Ready.pop();                //阻塞进程被从就绪队列中弹出
		ReadyLevel.pop();           //阻塞进程对应的level相应弹出
		(*process).state = false;   //阻塞进程的状态被设置为false
		(*process).address = q_name;//阻塞进程存储被阻塞的信号量队列标识名称
		return (*process).PID;      //返回阻塞进程的标识符
	}
	else { //当信号量≥0,该进程继续执行
		return "\0";
	}
}

string V(int* flag, queue<string>* q, queue<string>* qnext,int level) { //模拟V原语
	(*flag) += 1;
	if ((*flag) <= 0) {            //如果信号量≤0
		string tmp = (*q).front(); //将信号量对应的阻塞队列中的头一个进程弹出唤醒
		(*qnext).push(tmp);        //被唤醒的进程进入就绪队列
		ReadyLevel.push(level + 1);
		ReSort<string>(&Ready);    //被新唤醒的进程优先执行
		ReSort<int>(&ReadyLevel);
		++showBlue;                //被唤醒的进程数目加一
		(*q).pop();
		for (int i = 0; i < 5; ++i) {        
			if (tmp == producer[i]->PID) {
				producer[i]->address = "\0"; //被唤醒的进程不再有之前阻塞队列的标识名
				producer[i]->state = true;   //设置被唤醒的进程状态为true
				return tmp;
			}
		}
		for (int i = 0; i < 5; ++i) {
			if (tmp == consumer[i]->PID) {
				consumer[i]->address = "\0";  //被唤醒的进程不再有之前阻塞队列的标识名
				consumer[i]->state = true;    //设置被唤醒的进程状态为true
				return tmp;
			}
		}
		return tmp;
	}
	else { //若信号量>0，该进程继续执行
		return "\0";
	}
}

void PAction(int index,int* si) {//执行生产者进程
	if (P(&emptyNum, &Empty, "Empty", producer[index],0) == "\0") {  //相当于P(empty)
		if (P(&mutex, &Mutex, "Mutex", producer[index],2) == "\0") { //相当于P(mutex)
			for (int i = 0; i < 20; ++i) {//对buffer数组中的数据进行更新
				if (buffer[i].num == 0) {
					buffer[i].num = i + 1;
					buffer[i].owner = producer[index]->PID;
					//******************************************用小黄点指示一下当前进程操作的buffre单元位置
					cct_setcursor(CURSOR_INVISIBLE);
					cct_gotoxy(4 * (i + 1), 10);
					cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
					cout << "●";
					Sleep(350);
					cct_gotoxy(4 * (i + 1), 10);
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					cout << "  ";
					//*****************************************
					Ready.pop();      //一个进程被执行完
					ReadyLevel.pop();
					break;
				}
			}
			V(&mutex, &Mutex, &Ready,2);//相当于V(mutex)
			V(si, S[index], &Ready, 1); //相当于V(Si)
			V(&fullNum, &Full,&Ready,0);//相当于V(full)
		}
	}
}

void CAction(int index, int* si) {//执行消费者进程
	if (P(&fullNum, &Full, "Full", consumer[index],0) == "\0") {  //相当于P(full)
		if (P(si, S[index], "S" + to_string(index + 1), consumer[index], 1) == "\0") {  //相当于P(Si)
			if (P(&mutex, &Mutex, "Mutex", consumer[index], 2) == "\0") {   //相当于P(mutex)
				for (int i = 0; i < 20; ++i) {  //对buffer数组中的数据进行更新
					if (buffer[i].owner == "P"+to_string(index+1)) {
						buffer[i].num = 0;
						buffer[i].owner = "\0";
						//******************************************用小黄点指示一下当前进程操作的buffre单元位置
						cct_setcursor(CURSOR_INVISIBLE);
						cct_gotoxy(4 * (i + 1), 10);
						cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
						cout << "●";
						Sleep(350);
						cct_gotoxy(4 * (i + 1), 10);
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						cout << "  ";
						//*****************************************
						Ready.pop();       //一个进程被执行完
						ReadyLevel.pop();
						break;
					}
				}
				V(&mutex, &Mutex,&Ready,2);    //相当于V(mutex)
				V(&emptyNum, &Empty,&Ready,0); //相当于V(empty)
			}
		}
	}
}

void Push(string pname) { //将就绪状态的进程压入就绪队列
	Ready.push(pname);
	ReadyLevel.push(0);
}
void InterAction() {  //此函数用于使得用户能够通过输入进程标识符在就绪队列中新增进程
	while (1) {       //同时检查用户是否输入了有效合法的进程标识名
		//******************************************在屏幕上擦除上一次的信息
		cct_gotoxy(0,23 + Ready.size() / 18);
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 100; ++j) {
				cout << ' ';
			}
		}
		//******************************************在屏幕上写下这一次的信息
		cct_gotoxy(0,23 + Ready.size() / 18);
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		cout << "请输入一个您想要新增的进程名(P1-P5、C1-C5)，若不再需要输入则直接敲击回车键" << endl;
		cout << "若您输入了不存在的进程名,则程序不会响应,当就绪队列为空时程序结束:";
		//******************************************
		string tmpName = "\0";
		getline(cin, tmpName);
		if (tmpName.length() == 0) {//此情况说明用户不需要再输入内容，直接敲击了回车键
			return;
		}
		else {  //此情况用户输入了无效的进程标识名
            if (tmpName.length() != 2 || (tmpName[0] != 'P' && tmpName[0] != 'C') || tmpName[1] < '1' || tmpName[1] > '5') {
				continue;
			}
			else { //此情况用户输入了正确有效的标识名
				Ready.push(tmpName); //将用户新增的进程压入就绪队列末尾
				ReadyLevel.push(0);  //未被执行过的进程level为0
				update_buffer();     //更新屏幕上的buffer画面
				continue;
			}
		}
	}
}
void update_buffer() { //更新屏幕上的buffer画面，211-326行只涉及对输出内容进行格式化输出，与主干逻辑无关
	for (int i = 0; i < 20; ++i) {               //不影响程序理解，可以忽略
		if (emptyNum >= 0 && fullNum >= 0) {
			cct_setcolor(COLOR_HWHITE, COLOR_HGREEN);
		}
		else {
			cct_setcolor(COLOR_HWHITE, COLOR_HRED);
		}
		cct_gotoxy(4 * (i + 1), 8);
		if (buffer[i].num == 0) {
			cout << "  ";
		}
		else {
			cout << buffer[i].num;
		}
		if (emptyNum >= 0 && fullNum >= 0) {
			cct_setcolor(COLOR_BLACK, COLOR_HGREEN);
		}
		else {
			cct_setcolor(COLOR_BLACK, COLOR_HRED);
		}
		cct_gotoxy(4 * (i + 1), 6);
		if (buffer[i].owner == "\0") {
			cout << "  ";
		}
		else {
			cout << buffer[i].owner;
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
	cct_gotoxy(0, 11);
	for (unsigned int i = 0; i < 14+Ready.size()/18; ++i) {
		for (int j = 0; j < 100; ++j) {
			cout << ' ';
		}
		cout << endl;
	}
	cct_gotoxy(0, 11);
	cout << "各信号量阻塞队列情况:";
	if (showBlue>0) {
		cct_setcolor(COLOR_BLACK, COLOR_HBLUE);
		cout << "(被唤醒的进程将在下一步继续执行，未对齐的信号量将在此进程完成后对齐)";
		cct_setcolor(COLOR_BLACK, COLOR_WHITE);
		--showBlue;
	}
	cout<< endl;
	cout << "full: " << fullNum << "   阻塞进程: ";
	copyTmp = Full;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "empty: " << emptyNum << "   阻塞进程: ";
	copyTmp = Empty;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "mutex: " << mutex << "   阻塞进程: ";
	copyTmp = Mutex;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s1: " << sindex[0] << "   阻塞进程: ";
	copyTmp = (*S[0]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s2: " << sindex[1] << "   阻塞进程: ";
	copyTmp = (*S[1]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s3: " << sindex[2] << "   阻塞进程: ";
	copyTmp = (*S[2]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s4: " << sindex[3] << "   阻塞进程: ";
	copyTmp = (*S[3]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "s5: " << sindex[4] << "   阻塞进程: ";
	copyTmp = (*S[4]);
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
	}
	cout << endl;
	cout << "就绪进程队列:"<<endl;
	copyTmp = Ready;
	int endline = 1;
	while (!copyTmp.empty()) {
		cout << copyTmp.front() << "  ";
		copyTmp.pop();
		if (endline % 18 == 0) {
			cout << endl;
		}
		++endline;
	}
	endline = 1;
	cout << endl;
}

void Run() { //按序执行就绪队列中的进程
	while (!Ready.empty()) {  //只要就绪队列不空，就一直执行
		update_buffer();    //更新buffer画面
		InterAction();      //和用户交互
		string tmp = Ready.front();
		if (tmp[0] == 'P') {     //执行生产者进程
			for (int i = 0; i < 5; ++i) {
				if (tmp == producer[i]->PID) {
					PAction(i, &sindex[i]);
					break;
				}
			}
		}
		else if (tmp[0] == 'C') { //执行消费者进程
			for (int i = 0; i < 5; ++i) {
				if (tmp == consumer[i]->PID) {
					CAction(i, &sindex[i]);
					break;
				}
			}
		}
		else {
			cout << "未声明进程" << endl;
			break;
		}
	}
	update_buffer(); //更新buffer画面
	InterAction();   //和用户交互
}

int main() {
	//构建映射关系，程序逻辑需要
	producer[0] = &P1;
	producer[1] = &P2;
	producer[2] = &P3;
	producer[3] = &P4;
	producer[4] = &P5;
	consumer[0] = &C1;
	consumer[1] = &C2;
	consumer[2] = &C3;
	consumer[3] = &C4;
	consumer[4] = &C5;
	S[0] = &S1;
	S[1] = &S2;
	S[2] = &S3;
	S[3] = &S4;
	S[4] = &S5;
	//输出提示信息
	input_headline();
	draw_buffer();

	//为方便验证各信号量对应的阻塞队列功能，预设了一个就绪进程序列
	//c1 c2 p2 p1 20个p1 p2 c1 p1 c1 c1 c1 c1 p1 c1
	Push("C1");
	Push("C2");
	Push("P2");
	Push("P1");
	for (int i = 0; i < 20; ++i) {
		Push("P1");
	}
	Push("P2");
	Push("C1");
	Push("P1");
	Push("C1");
	Push("C1");
	Push("C1");
	Push("C1");
	Push("P1");
	Push("C1");
	//Push("P1");

	Run();//按照PV机制执行就绪队列中的进程
	
	system("pause");
	return 0;
}