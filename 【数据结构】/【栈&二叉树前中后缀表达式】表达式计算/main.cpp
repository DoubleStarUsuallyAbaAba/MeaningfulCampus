#include <iostream>
#include <string>
#include "binarytree.h"
#include "stack.h"
using namespace std;

string op_set = "*/+-()#";//运算符集合，#用作结尾标识符
char op_level[7][7] =     //此处的二维数组用于中缀表达式中运算符优先级的比较
{
	// *   /   +   -   (    )  #
	  '>','>','>','>','<','>','>',//  *
	  '>','>','>','>','<','>','>',//  /
	  '<','<','>','>','<','>','>',//  +
	  '<','<','>','>','<','>','>',//  -
	  '<','<','<','<','<','=','>',//  (
	  '>','>','>','>','<','>','>',//  )
	  '<','<','<','<','<','>','='//  #
};

int main()
{
	string s;
	BinaryTree<string> Tree("START_POINT");
	TreeNode<string>* Start=NULL, * End=NULL;
	Stack<string> Num, Sign;//这里的两个栈是用来追踪中缀表达式运算顺序的
	Stack<TreeNode<string>*> Build;//这个栈是用来建二叉树的
	string x, y;
	char op;

	Sign.Push("#");//标志符号栈的末尾

	cout << "程序使用注意事项：" << endl;
	cout << "当程序输出结果中出现 -. 即减号后面跟一个小点时，此时减号在用户输入的表达式中用于取反，成为一元运算符" << endl;
	cout << "请不要在表达式中输入空格来表示分隔，运算符紧挨着运算符就好，否则程序会将这种带空格的表达式格式判定为不规范"<< endl;
	cout << "当输入的表达式不规范，如：连续出现两个乘号或者存在非法输入时，程序给出提示并退出" << endl<<endl;

	cout << "请输入表达式:   ";
	cin >> s;
	s += "#"; //标志s的末尾
	cout << endl;
	//表达式没到尾部还能继续往数字栈中放；符号栈没到尾部还能继续处理
	for (int i = 0; s[i] != '#' || Sign.GetTop() != "#";)
	{
		//数字进数字栈，同时进Build栈
		if ((s[i] >= '0' && s[i] <= '9') || (i == 0 && s[i] == '-' && s[i + 1] >= '0' && s[i + 1] <= '9')//对纯数字、带正负号数字的识别
			|| (i >= 1 && s[i] == '-' && (s[i - 1] < '0' || s[i - 1]>'9') && s[i + 1] >= '0' && s[i + 1] <= '9'))
		{
			int j = i;
			++i;
			while (s[i] >= '0' && s[i] <= '9')//把数字从字符串中截取出来
			{
				i++;
			}
			string key = s.substr(j, i - j);
			Num.Push(key);//遇到数字就往数字栈里放
			Build.Push(new TreeNode<string>("N"));//往Build栈里放入表征数字的子树根节点
			Build.GetTop()->SetLeft(new TreeNode<string>(key));//在根节点下插入数字节点

			Start = Build.GetTop()->GetLeft();
			while (Start->GetLeft() != NULL)//中序遍历当前树是第一个输出的节点，用来定位左括号插入的位置
				Start = Start->GetLeft();
			End = Build.GetTop()->GetLeft();
			while (End->GetRight() != NULL)//中序遍历当前树是最后一个输出的节点，用来定位右括号插入的位置
				End = End->GetRight();
		}
		else
		{
			//符号进符号栈，同时进Build栈
			//这里是对中缀表达式计算顺序的判断，以便于随着中缀表达式运算顺序建立二叉树的顺序
			op = op_level[op_set.find(Sign.GetTop())][op_set.find(s[i])];//若是运算符则和符号栈的栈顶运算符比较优先级
			if (op == '<')// 栈顶元素优先级低，则当前元素入栈
			{
				string key(1, s[i]);//用1个s[i]字符初始化string字符串key，因为string本质上是一个类，这里的语法与实例化类一样
				Sign.Push(key);//预算符号进符号栈
				Build.Push(new TreeNode<string>("S"));//Build栈中进入一个表征符号的节点
				Build.GetTop()->SetLeft(new TreeNode<string>(key));//根节点之后插入存储实际运算符的左子节点
				i++;
			}
			else if (op == '=')//当一对括号相遇时表示括号内已运算完成，脱括号并接收下一字符
			{
				Sign.Pop();//弹出符号栈的栈顶左括号
				TreeNode<string>* temp = Build.Pop();//先弹出Build栈里，表示数字、或中间运算结果数字的节点或子树
				TreeNode<string>* kill = Build.Pop();//弹出左括号
				delete kill->GetLeft();//删除左括号及其根节点
				delete kill;
				string left_sign(1, '('), right_sign(1, ')');//表示左括号、右括号的string型字符(串)
				TreeNode<string>* LSign = new TreeNode<string>(left_sign), *RSign = new TreeNode<string>(right_sign);//建立表示左右括号的树节点
				Start->SetLeft(LSign);//左括号插入的位置应当是中序遍历第一个要输出节点的前驱
				LSign->SetParent(Start);
				Start = Start->GetLeft();//更新start指针
				End->SetRight(RSign);//右括号插入的位置应当是中序遍历最后一个要输出节点的后继
				RSign->SetParent(End);
				End = End->GetRight();//更新end指针
				Build.Push(temp);//将处理好括号的子树压入Build栈中
				i++;
			}
			else// 退栈并把运算结果入栈
			{
				string op_sign = Sign.Pop();//符号栈栈顶元素出栈
				if (op_sign == "-")//负号有两种可能：一元取负运算符、二元减法运算符
				{
					TreeNode<string>* temp_1 = Build.Pop();//获取一个运算符、两个操作数的信息
					TreeNode<string>* temp_2 = Build.Pop();
					TreeNode<string>* temp_3 = Build.GetTop();//后一个操作数先不弹出，确定了负号表示减法后再弹出
					TreeNode<string>* temp_4 = temp_3->GetLeft();
					while (temp_4->GetRight() != NULL)
						temp_4 = temp_4->GetRight();
					if (op_set.find(temp_4->GetData()) >= 0 && op_set.find(temp_4->GetData()) < op_set.length()//此时的负号是取负符号
						&& op_set.find(Start->GetData()) >= 0 && op_set.find(Start->GetData()) < op_set.length())
					{
						TreeNode<string>* contrary = temp_2->GetLeft();
						contrary->SetRight(temp_1->GetLeft());//通过调整树节点顺序使得在输出取负符号时输在正确的位置
						temp_1->GetLeft()->SetParent(contrary);
						Start = contrary;
						contrary->SetData("-.");
						Build.Push(temp_2);
						temp_1->SetLeft();
						delete temp_1;
					}
					else//此时的负号是减法符号
					{
						temp_3 = Build.Pop();
						y = Num.Pop();//数字栈中弹出相应的操作数
						x = Num.Pop();
						Num.Push(to_string(stold(x) - stold(y)));

						temp_2->GetLeft()->SetLeft(temp_3->GetLeft());//左操作数插入符号左子节点
						temp_3->GetLeft()->SetParent(temp_2->GetLeft());
						temp_3->SetLeft();
						delete temp_3;

						temp_2->GetLeft()->SetRight(temp_1->GetLeft());//右操作数插入符号节点右子节点
						temp_1->GetLeft()->SetParent(temp_2->GetLeft());
						temp_1->SetLeft();
						delete temp_1;
						Build.Push(temp_2);

						Start = Build.GetTop()->GetLeft();//更新Start指针位置
						while (Start->GetLeft() != NULL)
							Start = Start->GetLeft();
						End = Build.GetTop()->GetLeft();//更新End指针位置
						while (End->GetRight() != NULL)
							End = End->GetRight();
					}
				}
				else
				{
					TreeNode<string>* temp_1 = Build.Pop();//弹出两个操作数与一个运算符
					TreeNode<string>* temp_2 = Build.Pop();
					TreeNode<string>* temp_3 = Build.Pop();
					y = Num.Pop();
					x = Num.Pop();

					if (op_sign == "+")//加法
						Num.Push(to_string(stold(x) + stold(y)));
					else if (op_sign == "*")//乘法
						Num.Push(to_string(stold(x) * stold(y)));
					else//除法
						Num.Push(to_string(stold(x) / stold(y)));

					temp_2->GetLeft()->SetLeft(temp_3->GetLeft());//左操作数插入符号左子节点
					temp_3->GetLeft()->SetParent(temp_2->GetLeft());
					temp_3->SetLeft();
					delete temp_3;

					temp_2->GetLeft()->SetRight(temp_1->GetLeft());//右操作数插入符号节点右子节点
					temp_1->GetLeft()->SetParent(temp_2->GetLeft());
					temp_1->SetLeft();
					delete temp_1;

					Build.Push(temp_2);

					Start = Build.GetTop()->GetLeft();//更新Start指针位置
					while (Start->GetLeft() != NULL)
						Start = Start->GetLeft();
					End = Build.GetTop()->GetLeft();//更新End指针位置
					while (End->GetRight() != NULL)
						End = End->GetRight();
				}
			}
		}
	}

	TreeNode<string>* first = Build.Pop();//表达式二叉树建完之后及时清空暂存结构Build栈
	Tree.GetRoot()->SetLeft(first->GetLeft());//将树连到根节点下
	first->GetLeft()->SetParent(Tree.GetRoot());
	first->SetLeft();
	delete first;

	cout << "波兰表达式: ";//前序遍历，不输出左右括号
	Tree.ForwardTravel(Tree.GetRoot()->GetLeft(), "(", ")");
	cout << endl << endl;
	
	cout << "中缀表达式: ";//中序遍历，输出左右括号
	Tree.MiddleTravel(Tree.GetRoot()->GetLeft(), " ", " ");
	cout << endl << endl;
	
	cout << "逆波兰表达式: ";//后序遍历，不输出左右括号
	Tree.BackwardTravel(Tree.GetRoot()->GetLeft(), "(", ")");
	cout << endl << endl;

	//******************************************************************************************************
    //******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	_ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************
	return 0;
}
//测试案例_1:  18*23+(-(-100*0-(3*(1+2))))+2*(-3)
//测试案例_2:  5+3*(7-8/2)+6