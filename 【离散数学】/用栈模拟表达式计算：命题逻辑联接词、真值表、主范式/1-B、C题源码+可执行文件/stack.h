#pragma once
#include <iostream>
using namespace std;
template <class T> class Stack;
template <class T>
class StackNode//栈节点类
{
	friend class Stack<T>;
private:
	T data;//储存节点信息
	StackNode<T>* link;//储存节点链接节点的地址
public:
	StackNode(T d=NULL, StackNode<T>* L = NULL) :data(d), link(L) {}//构造函数
	StackNode<T>* GetLink() { return link; }//返回连接的节点地址
};
template <class T>
class Stack//栈类
{
private:
	StackNode<T>* top;//栈顶指针
public:
	Stack() :top(NULL) {}
	~Stack();
	void Push(const T& item);//入栈
	T Pop();//出栈
	T GetTop();//返回栈顶元素
	int IsEmpty() const//判断栈是否为空
	{
		return top == NULL;
	}
};
template<class T>
Stack<T>::~Stack()
{
	StackNode<T>* p;
	while (top != NULL)//析构函数，删除栈中的所有元素
	{
		p = top;
		top = top->link;
		delete p;
	}
}
template<class T>
void Stack<T>::Push(const T& item)//入栈
{
	top = new StackNode<T>(item, top);//头结点插入法，新插入节点永远是在已有节点之前，并将top指针在插入之后指向新节点，从而做到后进先出
}
template<class T>
T Stack<T>::Pop()//出栈
{
	if (IsEmpty())//若表达式导致栈空，那么说明存在表达式不规范的问题此时利用exit函数结束程序
	{
		cout << endl << "! 表达式不规范 ！" << endl;
		system("pause");
		exit(0);
	}
	StackNode<T>* p = top;
	T retvalue = p->data;
	top = top->link;
	delete p;     //这里删除了栈顶元素，把栈顶指针指向新的栈顶节点，并返回原来栈顶元素的值
	return retvalue;
}
template<class T>
T Stack<T>::GetTop()//返回栈顶节点的值
{
	if (IsEmpty())//若表达式导致栈空，那么说明存在表达式不规范的问题此时利用exit函数结束程序
	{
		cout << endl << "! 表达式不规范 ！" << endl;
		system("pause");
		exit(0);
	}
	return top->data;
}