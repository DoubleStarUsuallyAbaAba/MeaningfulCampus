#pragma once
#include <iostream>
using namespace std;
template <class T, class E>
class List;
template <class T, class E>
class ListNode//链表节点类
{
	friend class List<T, E>;
private:
	T head, tail;//头节点、尾节点
	E cost;//路径权值
	ListNode<T, E>* link;
public:
	ListNode() :link(NULL) {};
	ListNode(T h, T t, E c, ListNode<T, E>* L = NULL) :head(h), tail(t), cost(c), link(L) {};
	T get_head() { return head; }//返回头节点
	T get_tail() { return tail; }//返回尾节点
	E get_cost() { return cost; }//返回边权值
	ListNode<T, E>* get_link() { return link; }//返回连接的下一个节点
	void set_cost(E c) { cost = c; }//设置权值
};
template <class T, class E>
class List//链表类
{
private:
	ListNode<T, E>* first, * last;//头指针、尾指针
public:
	List(T h, T t, E c)//构造函数
	{
		last = first = new ListNode<T, E>(h, t, c);//动态开辟内存存储新节点，并将头、尾指针指向该节点
	}
	~List();
	long long Length();//统计链表长度
	long long FindData(E value, T head, T tail);//返回值为value、头节点为head、尾节点为tail的链表节点的位置
	long long FindEdge(T head, T tail);//寻找头节点为head、尾节点为tail的边
	long long FindBigger(E value);//寻找第一个比value值大的边
	ListNode<T, E>* FindNum(long long i);//返回链表第i个位置的节点的地址
	ListNode<T, E>* CreateNode(T h, T t, E c, ListNode<T, E>* next = NULL);//建立新链表节点
	int Insert(T h, T t, E c, long long i);//在位置i插入新链表节点
	int Remove(long long i);//移除链表中位置为i的节点
	ListNode<T, E>* Get(long long i);//获取链表i位置的节点值
};

template <class T, class E>
List<T, E>::~List()
{
	ListNode<T, E>* q;
	while (first->link != last && first->link != NULL)//在本程序的单循环链表中，链表不空，就一直循环执行删除操作
	{
		q = first->link;//指向头结点的下一个节点，并用q暂存其地址
		first->link = q->link;//令头结点指向下下个节点，与紧挨头结点的下一个节点脱钩，以便于对其执行删除操作
		delete q;//删除q指向的节点
	}
	last = first;//循环执行完毕后，链表中只余下头结点，将last指针与first指针对齐，以便于删除
	delete first;//删除头结点
}
template <class T, class E>
ListNode<T, E>* List<T, E>::CreateNode(T h, T t, E c, ListNode<T, E>* next)//建立新节点
{
	ListNode<T, E>* newnode = new ListNode<T, E>(h, t, c);
	newnode->link = next;//建立新节点并将新节点指针指向下一个节点
	return newnode;
}
template <class T, class E>
long long List<T, E>::Length() //统计链表长度
{
	ListNode<T, E>* p = first->link;//从头结点指向的下一个节点开始计数
	long long count = 0;
	while (p != NULL)//只要节点不空即未到链表末尾，则一直循环计数，并将指针指向当前节点的下一个节点
	{
		p = p->link;
		count++;
	}
	return count;//返回节点数量，即链表长度
}
template <class T, class E>
long long List<T, E>::FindData(E value, T head, T tail)//返回权值为value，头结点为head,尾节点为tail的链表节点的位置
{
	ListNode<T, E>* p = first->link;//从头结点指向的下一个节点开始
	long long count = 0;
	while (p != NULL && p->get_cost() != value && p->get_head() != head && p->get_tail() != tail)
	{
		p = p->link;
		count++;
	}
	if (p == NULL)//如果p==NULL，那说明在链表中没找到拥有该值的节点，返回-1，表示未找到
		count = -1;
	return count;
}
template <class T, class E>
long long List<T, E>::FindEdge(T head, T tail)//返回头节点为head,尾节点为tail的边的位置
{
	ListNode<T, E>* p = first->link;//从头结点指向的下一个节点开始
	long long count = 0;
	while (p != NULL && (!(p->get_head() == head && p->get_tail() == tail)) && (!(p->get_head() == tail && p->get_tail() == head)))
	{                            //注意此程序的边为无向边
		p = p->link;
		count++;
	}
	if (p == NULL)//未找到，表示-1
		count = -1;
	return count;
}
template <class T, class E>
long long List<T, E>::FindBigger(E value)//寻找第一条权值比value大的边的位置
{
	if (first->link == NULL)
		return 0;
	ListNode<T, E>* p = first->link;//从头结点指向的下一个节点开始
	long long count = 0;
	while (p != NULL && p->get_cost() < value)//未找到就一直循环
	{
		p = p->link;
		count++;
	}
	return count;
}
template <class T, class E>
ListNode<T, E>* List<T, E>::FindNum(long long i)//返回链表第i个位置的节点的地址
{
	if (i < -1)//非法的无效输入处理
		return NULL;
	if (i == -1)//头结点在链表中的位置用-1标识
		return first;
	ListNode<T, E>* p = first->link;
	int j = 0;
	while (p != NULL && j < i)//在链表长度范围内一直循环第i个位置或最后一个节点
	{
		p = p->link;
		j = j++;
	}
	return p;//如果第i个位置在链表长度范围内，返回的是节点地址，如i已经超出了链表长度，此时p的值恰好是NULL，返回NULL表示该位置不存在有效节点
}
template <class T, class E>
int List<T, E>::Insert(T h, T t, E c, long long i)//在位置i插入权值为value，头结点为h，尾节点为c的链表节点
{
	ListNode<T, E>* p = FindNum(i - 1);//在第i个位置插入节点当然需要定位i-1位置的节点，以使其指针指向插入的新节点
	if (p == NULL)//对非法无效位置的处理
		return 0;//返回0表示插入失败
	ListNode<T, E>* newnode = CreateNode(h, t, c, p->link);//先动态开辟内存存储新建立的节点，并让新建立的节点指向原来在i位置的节点
	if (p->link == NULL)
		last = newnode;//如果p已经是尾结点的话，那么新插入的节点成为新的尾结点
	p->link = newnode;//原来i-1位置的节点指针指向新建节点，此时，新建节点成为第i位置的节点，原来i位置及后面的节点位置随着这一步整体自然后移1位
	return 1;//返回1表示插入成功
}
template <class T, class E>
int List<T, E>::Remove(long long i)//移除链表中位置为i的节点
{
	ListNode<T, E>* q, * p = FindNum(i - 1);//在第i个位置删除节点当然需要定位i-1位置的节点，以更改其指针指向
	if (p == NULL || p->link == NULL)//如果第i位置本身不存在节点，自然就不需要额外的删除操作
		return 0;
	q = p->link;
	p->link = q->link;//i-1位置的指针指向下下个节点，跳过要删除的i位置节点
	if (q == last)
	{
		last = p;//如果要删除的节点是尾节点的话，尾结点指向应前移一位
	}
	delete q;//删除i位置节点
	return 1;//返回1表示删除成功
}
template <class T, class E>
ListNode<T, E>* List<T, E>::Get(long long i)//获取链表i位置的节点值
{
	ListNode<T, E>* p = FindNum(i);//先找到i位置的节点
	if (p == NULL || p == first)//对于非法位置的节点的处理
		return NULL;
	else
		return p;//返回i位置节点地址
}

