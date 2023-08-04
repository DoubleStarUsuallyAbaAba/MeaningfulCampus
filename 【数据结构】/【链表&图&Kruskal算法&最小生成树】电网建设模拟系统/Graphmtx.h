#pragma once
#include "List.h"
#include<iostream>
#define INF 100000 //表示正无穷
const int DefaultVertices = 30;//默认的最大size
using namespace std;

template<class T, class E>
class Graphmtx //矩阵表示的图
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
		if (i >= 0 && i < numVertices) return VerticesList[i];  //防止数组越界
		else
		{
			cout << "error:VerticesList[]数组越界" << endl;
			exit(0);
		}
	}
	E getWeight(int v1, int v2)									//取边（v1,v2）的权值，不合理返回0
	{
		if (v1 >= 0 && v2 >= 0 && v1 < numVertices && v2 < numVertices)//防止数组越界
			return Edge[v1][v2];
		else
			return 0;
	}
	int NumberOfVertices() { return numVertices; }	//返回当前顶点数
	int NumberOfEdges() { return numEdges; }		//返回当前边数
	int getFirstNeighbor(int v);                    //取顶点v的第一个邻接顶点
	int getNextNeighbor(int v, int w);              //返回v的邻接顶点w的下一个邻接顶点
	bool insertVertex(const T vertex);              //插入顶点
	bool insertEdge(int v1, int v2, E cost);        //插入边
	bool removeVertex(int v);                       //删除顶点v和所有与之关联的边
	bool removeEdge(int v1, int v2);                //删除边（v1,v2）
	void inputGraph();  //输入图
	void outputGraph(); //输出图
	int getVertexPos(T vertex)									//给出顶点在图中的位置
	{
		for (int i = 0; i < numVertices; i++)
			if (VerticesList[i] == vertex)return i;
		return -1;												//找不到返回-1
	}
	int DFS(int head,int tail);//深度搜索
	void DFS(int v,int visited[]);
	void Kruskal(List<T, E>& listOfVertex);//Kruskal最小生成树算法
private:
	T* VerticesList;											//顶点表
	E** Edge;													//邻接矩阵
	int maxVertices,numEdges, numVertices;

};

template<class T, class E>
Graphmtx<T, E>::Graphmtx(int sz)	//构造函数
{
	maxVertices = sz;//设置最大size
	numVertices = 0;
	numEdges = 0;
	int i, j;
	VerticesList = new T[maxVertices];
	Edge = new E * [maxVertices];
	for (i = 0; i < maxVertices; i++)//两个for循环动态内存开辟二维数组
		Edge[i] = new E[maxVertices];
	for (i = 0; i < maxVertices; i++)
		for (j = 0; j < maxVertices; j++)
			Edge[i][j] = (i == j) ? 0 : INF;
}

template<class T, class E>
int Graphmtx<T, E>::getFirstNeighbor(int v)//返回v的第一个邻接顶点的位置
{
	if (v >= 0 && v < maxVertices)
	{
		for (int col = 0; col < numVertices; col++)
			if (Edge[v][col] > 0 && Edge[v][col] < INF)
				return col;
	}
	return -1;
}

template<class T, class E>
int Graphmtx<T, E>::getNextNeighbor(int v, int w)//返回v的邻接顶点w的下一个邻接顶点
{
	if (v >= 0 && w >= 0 && v < numVertices && w < numVertices)
	{
		for (int col = w + 1; col < numVertices; col++)
		{
			if (Edge[v][col] > 0 && Edge[v][col] < INF)
				return col;
		}
	}
	return -1;
}

template<class T, class E>
bool Graphmtx<T, E>::insertVertex(const T vertex) //插入一个顶点
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
		if (Edge[v1][v2] == INF)
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
	if (v < 0 || v >= numVertices)	return false;			//v不在图中
	if (numVertices == 1)	return false;				//只剩一个顶点，不删除
	int i, j;
	VerticesList[v] = VerticesList[numVertices - 1];	//顶点表中删除
	for (i = 0; i < numVertices; i++)						//边数调整
		if (Edge[i][v] > 0 && Edge[i][v] < INF)
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
	if (v1 > -1 && v1<numVertices && v2>-1 && v2 < numVertices && Edge[v1][v2]>0 && Edge[v1][v2] < INF)
	{
		Edge[v1][v2] = Edge[v1][v2] = INF;
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
	cin >> n >> m;	//输入点数n、边数m
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
	cout << "顶点数和边数分别为：";
	cout << n << "," << m << endl;		//输出点数和边数
	cout << "各边依次为：" << endl;
	for (i = 0; i < n; i++)//遍历二维数组
	{
		for (j = i + 1; j < n; j++)
		{
			weight = this->getWeight(i, j);
			if (weight > 0 && weight < INF)
			{
				e1 = this->getValue(i);
				e2 = this->getValue(j);
				cout << "(" << e1 << "," << e2 << "," << weight << ")" << endl;
			}
		}
	}
}
template<class T, class E>
int Graphmtx<T, E>::DFS(int head,int tail)//深度搜索，递归实现
{
	int* visited = new int[numVertices];
	for (int i = 0; i < numVertices; i++)//用visited数组标记是否访问
		visited[i] = 0;
	DFS(head, visited);
	int exist = visited[tail];
	delete[]visited;
	return !exist;
}
template<class T, class E>
void Graphmtx<T, E>::DFS(int v, int visited[])
{
	visited[v] = 1;
	int w = getFirstNeighbor(v);
	while (w != -1)//如果还有邻接顶点的话
	{
		if (!visited[w])//如果没访问过，就访问
			DFS(w, visited);
		w = getNextNeighbor(v, w);
	}
}
template<class T, class E>
void Graphmtx<T, E>::Kruskal(List<T, E>& listOfVertex)//Keuskal最小生成树算法
{
	while (listOfVertex.Length())
	{
		long long len = listOfVertex.Length();//注意，边链表中从头到位已经是按权值从小到大排好序的
		int q = 0;
		for (int i = 1; i < numVertices; i++)
			q += DFS(0, i);
		if (q == 0)//如果0节点到所有其他节点通过深度搜索发现都有通路，即现在已经构成连通图，循环结束
			break;
		else
		{
			int x = getVertexPos(listOfVertex.FindNum(0)->get_head());
			int y = getVertexPos(listOfVertex.FindNum(0)->get_tail());
			for (int i = 0; i < numVertices; i++)
			{
				if (Edge[x][i]>0&& Edge[x][i] < INF &&(!DFS(i,y)))//如果新找的边会形成死锁，删除该边
				{
					listOfVertex.Remove(0);
					break;
				}

			}
			if (len == listOfVertex.Length())//若不会形成死锁，插入该边
			{
				insertEdge(x, y, listOfVertex.FindNum(0)->get_cost());
				listOfVertex.Remove(0);
			}
		}
	}
}