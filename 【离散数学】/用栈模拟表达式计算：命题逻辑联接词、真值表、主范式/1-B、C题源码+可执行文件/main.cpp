#include <iostream>
#include <string>
#include <cmath>
#include "stack.h"
using namespace std;

string op_set = "!&|^~()#";
char op_level[8][8] =
{
	// !   &   |   ^   ~   (   )   #
	  '>','>','>','>','>','<','>','>',//!
	  '<','>','>','>','>','<','>','>',//&
	  '<','<','>','>','>','<','>','>',//|
	  '<','<','<','>','>','<','>','>',//^
	  '<','<','<','<','>','<','>','>',//~
	  '<','<','<','<','<','<','=','>',//(
	  '>','>','>','>','>','<','>','>',//)
	  '<','<','<','<','<','<','>','=',//#
};

int cal_rule(int x, int y, char op)//运算规则
{
	if (op == '&')
		return x && y;
	else if (op == '|')
		return x || y;
	else if (op == '^')
		return (!x) || y;
	else 
		return ((!x) || y)&& ((!y) || x);
}
int calculator(string s)//运算
{
	int len = s.length();
	s += "#"; //标志s的末尾
	int x, y;
	char op;
	string mid;
	Stack<int>P;
	Stack<char>Q;
	Q.Push('#');//标志Q的末尾

	//s没到尾部还能继续往P中放；Q没到尾部还能继续处理
	for(int i=0;s[i] != '#' || Q.GetTop() != '#';)
	{
		if (s[i] == '1' || s[i] == '0')
		{
			mid = s[i];
			P.Push(stoi(mid));//遇到数字就往P里放
			i++;
		}
		else
		{
			switch (op_level[op_set.find(Q.GetTop())][op_set.find(s[i])])//若是运算符则和Q栈的栈顶运算符比较优先级
			{
			case '<':  // 栈顶元素优先级低，则当前元素入栈
				Q.Push(s[i]);
				i++;
				break;
			case '=': //当一对括号相遇时表示括号内已运算完成，脱括号并接收下一字符
				Q.Pop();
				i++;
				break;
			case '>':  // 退栈并将运算结果入栈
				if (Q.GetTop() == '!')
				{   // 栈顶元素为 ！，单目操作符
					x = P.GetTop();
					P.Pop();
					P.Push(!x);
					Q.Pop();
				}
				else
				{   // 栈顶元素不是 ！
					x = P.GetTop();
					P.Pop();
					y = P.GetTop();
					P.Pop();
					op = Q.GetTop();
					Q.Pop();//计算结果并入栈
					P.Push(cal_rule(y, x, op));
				}
				break;
			}
		}
	}
	return P.GetTop();
}

int main()
{
	string sz, value = "#", sz_copy, value_copy = "#", hequ = "#", xiqu = "#";
	int len = 0, result = -1;
	cout << "***************************************" << endl;
	cout << "**                                   **" << endl;
	cout << "**         欢迎进入逻辑运算程序      **" << endl;
	cout << "**   (可运算真值表,主范式,支持括号)  **" << endl;
	cout << "** 以下符号请务必在英文输入法下输入! **" << endl;
	cout << "**              用!表示非            **" << endl;
	cout << "**              用&表示与            **" << endl;
	cout << "**              用|表示或            **" << endl;
	cout << "**             用^表示蕴含           **" << endl;
	cout << "**             用~表示等值           **" << endl;
	cout << "**                                   **" << endl;
	cout << "***************************************" << endl << endl;
	cout << "请输入一个合法的命题公式:" << endl;
	while (1)//判断命题公式是否合法并在命题公式非法时提示用户重新输入
	{
		int left = 0, right = 0;
		cin >> sz;//输入表达式
		len = sz.length();
		for (int i = 0; i < len; i++)
		{
			if (sz[i] == '(')//统计左括号数量
				left++;
			else if (sz[i] == ')')//统计右括号数量
				right++;
			else if ((sz[i] >= 'a' && sz[i] <= 'z') || (sz[i] >= 'A' && sz[i] <= 'Z'))
			{
				if (i > 0)
				{
					if ((sz[i - 1] >= 'a' && sz[i - 1] <= 'z') || (sz[i - 1] >= 'A' && sz[i - 1] <= 'Z'))//连续出现连个变元，中间没有运算符连接
					{
						len = 0;
						break;
					}
				}
				if (i < len - 1)
				{
					if ((sz[i + 1] >= 'a' && sz[i + 1] <= 'z') || (sz[i + 1] >= 'A' && sz[i + 1] <= 'Z'))//连续出现连个变元，中间没有运算符连接
					{
						len = 0;
						break;
					}
				}
				if (value.find(sz[i]) == -1)
					value += sz[i];
			}
			else if (op_set.find(sz[i])<0|| op_set.find(sz[i])>=op_set.length()-1)//出现非法的运算符
			{
				len = 0;
				break;
			}
		}
		if (len == 0 || left != right)//满足此条件则说明命题公式非法，需要重新输入
		{
			cout << endl << "命题公式非法,请重新输入: ";
			value = "#";
			continue;
		}
		else
			break;
	}
	len = value.length();
	value_copy = value;
	cout << "该式子中的变量个数为: " << len - 1 << endl;
	cout << "输出真值表如下:" << endl << endl;
	for (int i = 1; i < len; i++)
		cout << value[i] << ' ';
	cout << sz << endl;
	for (int i = 0; i < static_cast<int>(pow(2, len - 1)); i++)//模拟真值表，并判断命题真假
	{
		sz_copy = sz;
		if (i == 0)//真值表中第一行，所有变元的赋值均为0，此后只要对二进制0、1序列不断加一，不断执行从0到1，从1到0的反转即可模拟真值表
		{
			for (int j = 1; j < len; j++)
				value_copy[j] = '0';
		}
		else
		{
			for (int j = len - 1; j >= 1; j--)//不断执行二进制数加1，模拟真值表
			{
				if (value_copy[j] == '0')
				{
					value_copy[j] = '1';
					break;
				}
				else
				{
					value_copy[j] = '0';
					continue;
				}
			}
		}
		for (int j = 1; j < len; j++)
		{
			while (1)
			{
				int k = sz_copy.find(value[j]);//把0、1值赋值给相应的变量
				if (k == -1)
					break;
				else
				{
					sz_copy[k] = value_copy[j];
					continue;
				}
			}
		}
		result = calculator(sz_copy);//判断命题真假
		if (result == 0)//根据真假结果添加合取项、析取项到最终结果
		{
			hequ += to_string(i);
			hequ += "#";
		}
		else if (result == 1)
		{
			xiqu += to_string(i);
			xiqu += "#";
		}
		for (int j = 1; j < len; j++)
			cout << value_copy[j] << ' ';
		cout << result << endl;
	}
	if (hequ.length() == 1)
		cout << endl << "该命题公式不存在主合取范式" << endl;
	else
	{
		cout << endl << "该命题公式的主合取范式为:" << endl;//输出主合取范式
		for (unsigned int j = 1; j < hequ.length(); j++)
		{
			if (j > 1)
				cout << "∨";
			cout << "M(";
			while (hequ[j] != '#')
			{
				cout << hequ[j];
				j++;
			}
			cout << ")";
		}
		cout << endl;
		hequ = "#";
	}
	if (xiqu.length() == 1)
		cout << endl << "该命题公式不存在主析取范式" << endl;
	else
	{
		cout << endl << "该命题公式的主析取范式为:" << endl;//输出主析取范式
		for (unsigned int j = 1; j < xiqu.length(); j++)
		{
			if (j > 1)
				cout << "∧";
			cout << "m(";
			while (xiqu[j] != '#')
			{
				cout << xiqu[j];
				j++;
			}
			cout << ")";
		}
		cout << endl;
		xiqu = "#";
	}
	cout << endl << "程序运算结束,欢迎下次使用!" << endl;

	system("pause");
	return 0;
}