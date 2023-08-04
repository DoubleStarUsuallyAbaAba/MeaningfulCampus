#pragma once
#include "Graph.h"

template<class T, class E>
struct Edge		//边界点的定义
{
	int dest;	//边的另一顶点位置
	E  cost;	//权值
	Edge<T, E>* link;//下一条边链指针
	Edge() {}		//构造函数
	Edge(int num, E weight) :dest(num), cost(weight), link(NULL) {} //构造函数
	bool operator != (Edge<T, E>& R)const {	//判边不等否
		return (dest != R.dest) ? true : false;
	}
};

template<class T, class E >
struct Vertex {	//顶点的定义
	T data;		//顶点名字
	Edge<T, E>* adj;	//边链表的头指针
};

template <class T, class E>
class Graphlnk : public Graph<T, E>
{
public:
	Graphlnk(int sz = DefaultVertices);
	~Graphlnk();
	T getValue(int i)
	{
		return (i >= 0 && i < numVertices) ? NodeTable[i].data : 0;
	}
	E getWeight(int v1, int v2);
	int getFirstNeighbor(int v);
	int getNextNeighbor(int v, int w);
	bool insertVertex(const T& vertex);
	bool insertEdge(int v1, int v2, E cost);
	bool removeVertex(int v);
	bool removeEdge(int v1, int v2);
	void inputGraph();
	void outputGraph();
	int getVertexPos(T vertex) {
		for (int i = 0; i < numVertices; i++)
		{
			if (NodeTable[i].data == vertex)
				return i;
		}
		return -1; //找不到就返回-1
	}

private:
	Vertex<T, E>* NodeTable; //顶点表

};

template <class T, class E>
Graphlnk<T, E>::Graphlnk(int sz)  //构造函数
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = new Vertex<T, E>[maxVertices];
	if (NodeTable == NULL) { cerr << "存储分配错！" << endl; exit(1); }
	for (int i = 0; i < maxVertices; i++)
	{
		NodeTable[i].adj = NULL;
	}
}

template<class T, class E>
Graphlnk<T, E>::~Graphlnk() //析构函数
{
	for (int i = 0; i < numVertices; i++)
	{
		Edge<T, E>* p = NodeTable[i].adj;
		while (p != NULL)
		{
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}

	}
	delete[]NodeTable;
}

template <class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2) //返回边（v1，v2）的权重，边不存在则返回0
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E>* p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
			p = p->link;
		if (p != NULL)
			return p->cost;
	}
	return 0;
}

template<class T, class E>
int Graphlnk<T, E>::getFirstNeighbor(int v) //获得v的第一个邻接顶点，找不到则返回-1
{
	if (v != -1)
	{
		Edge<T, E>* p = NodeTable[v].adj;
		if (p != NULL)
			return p->dest;
	}
	return -1;
}
template<class T, class E>
int Graphlnk<T, E>::getNextNeighbor(int v, int w) //获得v的邻接顶点w的下一个邻接顶点
{
	if (v != -1)
	{
		Edge<T, E>* p = NodeTable[v].adj;
		while (p != NULL && p->dest != w)		//寻找邻接顶点w
			p = p->link;
		if (p != NULL && p->link != NULL)	//找到w且存在下一个邻接顶带你
			return p->link->dest;
	}
	return -1;
}


template<class T, class E>
bool Graphlnk<T, E>::insertVertex(const T& vertex)	//插入点
{
	if (numVertices == maxVertices)	return false;	//图已满，插入失败
	NodeTable[numVertices].data = vertex;
	numVertices++;
	return true;
}

template<class T, class E>
bool Graphlnk<T, E>::removeVertex(int v)			//删除点
{
	if (numVertices == 1 || v < 0 || v >= numVertices)		return false;
	Edge<T, E>* p, * s, * t;
	int  k;
	while (NodeTable[v].adj != NULL)		//删除该顶点，以及与之邻接的顶点中的记录
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;	//以找对称存放的边节点
		t = NULL;
		while (s != NULL && s->dest != v) //在对称点的邻接表里面找v，删除掉
		{
			t = s;
			s = s->link;
		}
		if (s != NULL)
		{
			if (t == NULL) //第一个邻接顶点就是v
				NodeTable[k].adj = s->link;
			else
				t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;
		delete p;
		numEdges--;
	}
	numVertices--;
	NodeTable[v].data = NodeTable[numVertices].data;
	p = NodeTable[v].adj = NodeTable[numVertices].adj;
	while (p != NULL)
	{
		s = NodeTable[p->dest].adj;
		while (s != NULL)
		{
			if (s->dest == numVertices)
			{
				s->dest = v;
				break;
			}
			else
				s = s->link;
		}
		p = p->link;
	}
	return true;
}

template<class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2, E weight)	//插入一条边,若边已存在，或参数不合理，返回false
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E>* q, * p = NodeTable[v1].adj;
		//先检查该边是否已经存在
		while (p != NULL && p->dest != v2)
			p = p->link;
		if (p != NULL)//找到该边
		{
			cout << "该边已经存在，插入失败！" << endl;
			return false;
		}
		p = new Edge<T, E>;
		q = new Edge<T, E>;
		p->dest = v2;
		p->cost = weight;
		p->link = NodeTable[v1].adj;
		NodeTable[v1].adj = p;	//插入到邻接表表头
		q->dest = v1;
		q->cost = weight;
		q->link = NodeTable[v2].adj;
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}

template<class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
	{
		Edge<T, E>* p = NodeTable[v1].adj, * q = NULL, * s = p;
		while (p != NULL && p->dest != v2) //先找该边
		{
			q = p;
			p = p->link;
		}
		if (p != NULL) //找到该边
		{
			if (p == s)//第一个节点就找到
				NodeTable[v1].adj = p->link;
			else
				q->link = p->link;
			delete p;
		}
		else return false; //找不到边
		p = NodeTable[v2].adj; q = NULL; s = p;
		while (p != NULL && p->dest != v1)
		{
			q = p;
			p = p->link;
		}
		if (p == s)
			NodeTable[v2].adj = p->link;
		else
			q->link = p->link;
		delete p;
		return true;
	}
	return false;
}

template<class T, class E>
void Graphlnk<T, E>::inputGraph()
{
	//通过从输入流对象in输入n的顶点和e条五项边的信息建立邻接矩阵表示的图G。邻接矩阵初始化工作在构造函数完成
	int i, j, k, m, n;
	T e1, e2;
	E weight;
	cout << "请输入顶点数和边数(空格隔开):" << endl;
	cin >> n >> m;	//输入点数n的边数m
	cout << "请依次输入顶点:" << endl;
	for (i = 0; i < n; i++)//输入顶点，建立顶点表
	{
		cin >> e1;
		this->insertVertex(e1);
		//G.insertVertex(e1);
	}
	cout << "请依次输入边，形如 v1 v2 weight ：" << endl;
	i = 0;
	while (i < m)
	{
		cin >> e1 >> e2 >> weight;
		j = this->getVertexPos(e1);//查顶点号
		k = this->getVertexPos(e2);
		if (j == -1 || k == -1)
		{
			cout << "边两端点信息有误，重新输入！" << endl;
		}
		else
		{
			if (this->insertEdge(j, k, weight))
				i++;
		}
	}

}
template<class T, class E>
void Graphlnk<T, E>::outputGraph()
{
	//输出图的所有顶点和边信息
	int i, j, n, m;
	T e1, e2;
	E weight;
	n = this->NumberOfVertices();	 //点数
	m = this->NumberOfEdges();		//边数
	cout << "顶点数的边数为：";
	cout << n << "," << m << endl;		//输出点数和边数
	cout << "各边依次为：" << endl;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			weight = this->getWeight(i, j);
			if (weight > 0 && weight < maxWeight)
			{
				e1 = this->getValue(i);
				e2 = this->getValue(j);
				cout << "(" << e1 << "," << e2 << "," << weight << ")" << endl;
			}
		}
	}
}