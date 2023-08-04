#pragma once
#include "Graph.h"
#include<iostream>
using namespace std;

template<class T, class E>
class Graphmtx : public Graph<T, E>
{

public:
	Graphmtx(int sz = DefaultVertices);							//构造
	~Graphmtx()													//析构
	{
		delete[]VerticesList;
		delete[]Edge;
	}
	T getValue(int i)											//取顶点i的值，若i不合理返回NULL
	{
		if (i >= 0 && i < numVertices) return VerticesList[i];
		else return NULL;
	}
	E getWeight(int v1, int v2)									//取边（v1,v2）的权值，不合理返回0
	{
		if (v1 >= 0 && v2 >= 0&&v1<numVertices&& v2 < numVertices)
			return Edge[v1][v2];
		else
			return 0;
	}
	int getFirstNeighbor(int v);
	int getNextNeighbor(int v, int w);
	bool insertVertex(const T& vertex);
	bool insertEdge(int v1, int v2, E cost);
	bool removeVertex(int v);
	bool removeEdge(int v1, int v2);
	void inputGraph();
	void outputGraph();
	int getVertexPos(T vertex)									//给出顶点在图中的位置
	{
		for (int i = 0; i < numVertices; i++)
			if (VerticesList[i] == vertex)return i;
		return -1;												//找不到返回-1
	}
private:
	T* VerticesList;											//顶点表
	E** Edge;													//邻接矩阵

};

template<class T, class E>
Graphmtx<T, E>::Graphmtx(int sz)	//构造函数
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	int i, j;
	VerticesList = new T[maxVertices];
	Edge = new E * [maxVertices];
	for (i = 0; i < maxVertices; i++)
		Edge[i] = new E[maxVertices];
	for (i = 0; i < maxVertices; i++)
		for (j = 0; j < maxVertices; j++)
			Edge[i][j] = (i == j) ? 0 : maxWeight;
}

template<class T, class E>
int Graphmtx<T, E>::getFirstNeighbor(int v)//返回v的第一个邻接顶点的位置
{
	if (v>=0&&v<maxVertices)
	{
		for (int col = 0; col < numVertices; col++)
			if (Edge[v][col] > 0 && Edge[v][col] < maxWeight)
				return col;
	}
	return -1;
}

template<class T, class E>
int Graphmtx<T, E>::getNextNeighbor(int v, int w)//返回v的邻接顶点w的下一个邻接顶点
{
	if (v >= 0 && w >= 0&&v<numVertices&&w<numVertices)
	{
		for (int col = w + 1; col < numVertices; col++)
		{
			if (Edge[v][col] > 0 && Edge[v][col] < maxWeight)
				return col;
		}
	}
	return -1;
}

template<class T, class E>
bool Graphmtx<T, E>::insertVertex(const T& vertex) //插入一个顶点
{
	if (numVertices == maxVertices)return false; //顶点表已满，返回false
	VerticesList[numVertices++] = vertex;
	return true;
}

template<class T, class E>
bool Graphmtx<T, E>::insertEdge(int v1, int v2, E cost)//插入一条边
{
	if (v1 > -1 && v1<numVertices && v2>-1 && v2 < numVertices) //检查条件
	{
		if (Edge[v1][v2] == maxWeight)
		{
			Edge[v1][v2] = Edge[v2][v1] = cost;
			numEdges++;
			return true;
		}
		else
		{
			cout << "该边已存在，添加失败" << endl;
			return false;
		}
	}
	else return false;
}

template<class T, class E>
bool Graphmtx<T, E>::removeVertex(int v)				//删除一个顶点
{
	if (v<0 || v>=numVertices)	return false;			//v不在图中
	if (numVertices == 1)	return false;				//只剩一个顶点，不删除
	int i, j;
	VerticesList[v] = VerticesList[numVertices - 1];	//顶点表中删除
	for (i = 0; i < numVertices; i++)						//边数调整
		if (Edge[i][v] > 0 && Edge[i][v] < maxWeight)
			numEdges--;
	for (i = 0; i < numVertices; i++)
		Edge[i][v] = Edge[i][numVertices - 1];
	numVertices--;									//顶点数调整
	for (j = 0; j < numVertices; j++)
		Edge[v][j] = Edge[numVertices][j];
	return true;
}

template<class T, class E>
bool Graphmtx<T, E>::removeEdge(int v1, int v2)		//删除边
{
	if (v1 > -1 && v1<numVertices && v2>-1 && v2 < numVertices && Edge[v1][v2]>0 && Edge[v1][v2] < maxWeight)
	{
		Edge[v1][v2] = Edge[v1][v2] = maxWeight;
		numEdges--;
		return true;
	}
	else return false;
};

template<class T, class E>
void Graphmtx<T, E>::inputGraph()
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
void Graphmtx<T, E>::outputGraph()
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