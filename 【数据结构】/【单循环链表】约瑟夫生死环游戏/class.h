#pragma once
#include <iostream>
using namespace std;

template <class T>
class List;

template <class T>
class ListNode//链表节点类
{
	friend class List<T>;
private:
	T data;
	ListNode<T>* link;
public:
	ListNode();
	ListNode(const T& item);
	T get_data()//返回该节点的数值
	{
		return data;
	}
};

template <class T>
class List        //链表类
{
private:
	ListNode<T>* first, * last;//头指针、尾指针
public:
	List(const T& value, const T& next_value)//构造函数，实例化类时建立头结点并讲头结点与后面第一个节点连接
	{
		last = first = new ListNode<T>(value);
		last = first->link = new ListNode<T>(next_value);//动态开辟内存存储新节点，在实例化类时即将头结点与第一个节点连接
	}
	~List();
	void Create_Circle()//尾指针指向头指针，构造单循环链表
	{
		last->link = first;
	}
	long long Length() const;//统计链表长度
	long long FindData(T value);//返回值为value的链表节点的位置
	ListNode<T>* FindNum(long long i);//返回链表第i个位置的节点的地址
	ListNode<T>* CreateNode(const T& item, ListNode<T>* next = NULL);//建立新节点
	int Insert(T value, long long i);//在位置i插入值为value的链表节点
	int Remove(long long i);//移除链表中位置为i的节点
	T* Get(long long i);//获取链表i位置的节点值
	void Joseph(T start,T count,T killer);//模拟约瑟夫生死环规则删除链表节点
};

template <class T>
ListNode<T>::ListNode() :link(NULL) {}
template <class T>
ListNode<T>::ListNode(const T& item) : data(item), link(NULL) {}

template <class T>
List<T>::~List()
{
	ListNode<T>* q;
	while (first->link != last)//在本程序的单循环链表中，last指向的就是first，因此只要链表中头指针不指向尾指针，即头指针自身，那么链表就不空，就一直循环执行删除操作
	{
		q = first->link;//指向头结点的下一个节点，并用q暂存其地址
		first->link = q->link;//令头结点指向下下个节点，与紧挨头结点的下一个节点脱钩，以便于对其执行删除操作
		delete q;//删除q指向的节点
	}
	last = first;//循环执行完毕后，链表中只余下头结点，将last指针与first指针对齐，以便于删除
	delete first;//删除头结点
}
template <class T>
ListNode<T>* List<T>::CreateNode(const T& item, ListNode<T>* next)//建立新节点
{
	ListNode<T>* newnode = new ListNode<T>(item);
	newnode->link = next;  //建立新节点并将新节点指针指向下一个节点
	return newnode;
}
template <class T>
long long List<T>::Length() const  //统计链表长度
{
	ListNode<T>* p = first->link; //从头结点指向的下一个节点开始计数
	long long count = 0;
	while (p != first&&p!=NULL)//只要节点不空且p没有越过last循环回到first，即未到链表末尾，则一直循环计数，并将指针指向当前节点的下一个节点
	{
		p = p->link;
		count++;
	}
	return count;//返回节点数量，即链表长度
}
template <class T>
long long List<T>::FindData(T value)//返回值为value的链表节点的位置
{
	ListNode<T>* p = first->link;//从头结点指向的下一个节点开始
	long long count = 0;
	while (p != first && p->get_data()!= value)
	{
		p = p->link;
		count++;
	}
	if (p == first||p==NULL)//如果p==NULL，或者p越过last循环回到first,那说明在链表中没找到拥有该值的节点，返回-1，表示未找到
		count = -1;
	return count;
}
template <class T>
ListNode<T>* List<T>::FindNum(long long i)//返回链表第i个位置的节点的地址
{
	if (i < -1)//非法的无效输入处理
		return NULL;
	if (i == -1)//头结点在链表中的位置用-1标识
		return first;
	ListNode<T>* p = first->link;
	int j = 0;
	while (p!=first&&p != NULL && j < i)//在链表长度范围内一直循环第i个位置或最后一个节点
	{
		p = p->link;
		j = j++;
	}
	return p;//如果第i个位置在链表长度范围内，返回的是节点地址，如i已经超出了链表长度，此时p的值恰好是NULL，返回NULL表示该位置不存在有效节点
}
template <class T>
int List<T>::Insert(T value, long long i)//在位置i插入值为value的链表节点
{
	ListNode<T>* p = FindNum(i - 1);//在第i个位置插入节点当然需要定位i-1位置的节点，以使其指针指向插入的新节点
	if (p == NULL)//对非法无效位置的处理
		return 0;//返回0表示插入失败
	ListNode<T>* newnode = CreateNode(value, p->link);//先动态开辟内存存储新建立的节点，并让新建立的节点指向原来在i位置的节点
	if (p->link == NULL)
		last = newnode;//如果p已经是尾结点的话，那么新插入的节点成为新的尾结点
	p->link = newnode;//原来i-1位置的节点指针指向新建节点，此时，新建节点成为第i位置的节点，原来i位置及后面的节点位置随着这一步整体自然后移1位
	return 1;//返回1表示插入成功
}
template <class T>
int List<T>::Remove(long long i)//移除链表中位置为i的节点
{
	ListNode<T>* q, * p = FindNum(i - 1);//在第i个位置删除节点当然需要定位i-1位置的节点，以更改其指针指向
	if (p == NULL || p->link == NULL)//如果第i位置本身不存在节点，自然就不需要额外的删除操作
		return 0;
	q = p->link;
	p->link = q->link;//i-1位置的指针指向下下个节点，跳过要删除的i位置节点
	if (q == last)
	{
		last = p;//如果要删除的节点是尾节点的话，尾结点指向应前移一位
		last->link = first;//保持单循环链表结构
	}
	delete q;//删除i位置节点
	return 1;//返回1表示删除成功
}
template <class T>
T* List<T>::Get(long long i)//获取链表i位置的节点值
{
	ListNode<T>* p = FindNum(i);//先找到i位置的节点
	if (p == NULL || p == first)//对于非法位置的节点的处理
		return NULL;
	else
		return &p->data;//返回i位置节点的数值，即旅客序号
}
template <class T>
void List<T>::Joseph(T start,T count,T killer)//模拟约瑟夫生死环规则删除链表节点
{
	long long len = Length();
	ListNode<T>* current = FindNum(start),*copy=current;//定位报数的起始位置
	int num = 0;
	cout << endl << endl;
	for (int i = 0; i < count; i++)//要淘汰的人数，外层循环
	{
		for (int j = 0; j < killer; j++)//要淘汰的数字，内层循环
		{
			if(j!=0)//前一轮报数的最后已经将指针指向了下一个，因此j==0时不用继续指向像一个旅客
			    current = current->link;
			if (current == first)//头结点之后的第一个节点才是代表第一个旅客的节点
				current = current->link;
		}
		cout << "第" << i+1 << "个死者的位置是:  " << current->data << endl;//输出淘汰的旅客序号
		copy = current->link;
		Remove(FindData(current->data));//执行删除节点操作
		current = copy;
	}
	len = Length();//最后剩下的旅客数
	cout <<endl<<endl<< "最后剩下: "<<len<<" 人"<<endl;
	cout << "剩余的生者位置为: " << endl;//遍历链表输出剩余的旅客序号
	for (int i = 0; i < len; i++)
	{
		num++;
		cout << *Get(i) << "     ";
		if (num == 6)//每输出6个序号就换行
		{
			num = 0;
			cout << endl;
		}
	}
}