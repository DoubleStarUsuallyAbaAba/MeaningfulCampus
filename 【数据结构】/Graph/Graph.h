#pragma once
#include<iostream>
using namespace std;


#define INF 100000 //表示正无穷
const int DefaultVertices = 30;

template<class T, class E>
class Graph
{
public:
	static const E maxWeight = INF;
	Graph(int size = DefaultVertices) {};
	virtual ~Graph() {};
	bool GraphEmpty()const //检查为空
	{
		if (numEdges == 0)return true;
		else return false;
	}
	bool GraphFull()const //检查为满
	{
		if (numVertices == maxVertices || numEdges == maxVertices * (maxVertices - 1) / 2)
			return true;
		else
			return false;
	}
	int NumberOfVertices() { return numVertices; }	//返回当前顶点数
	int NumberOfEdges() { return numEdges; }		//返回当前边数
	virtual T getValue(int i) = 0;					//取顶点i的值，i不合理返回0
	virtual E getWeight(int v1, int v2) = 0;		//取边（v1，v2）的权值
	virtual int getFirstNeighbor(int v) = 0;		//取顶点v的第一个邻接顶点
	virtual int getNextNeighbor(int v, int w) = 0;	//取邻接顶点w的下一个邻接顶点
	virtual bool insertVertex(const T& vertex) = 0;	//插入一个顶点vertex
	virtual bool insertEdge(int v1, int v2, E cost) = 0;//插入边（v1,v2）,权值cost
	virtual bool removeVertex(int v) = 0;			//删除顶点v和所有与之关联的边
	virtual bool removeEdge(int v1, int v2) = 0;		//删除边（v1,v2）
	virtual int getVertexPos(T vertex) = 0;

protected:
	int maxVertices;
	int numEdges;
	int numVertices;

};