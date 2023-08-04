#include "Graphmtx.h"
#include "Graphlnk.h"

void test_Graphmtx()
{
	char ch1, ch2;
	int weight;
	Graphmtx<char, int> g(30);
	g.inputGraph();		//构造图
	g.outputGraph();	//显示图

	cout << "顶点数和边数：" << g.NumberOfVertices() << " " << g.NumberOfEdges() << endl;
	cout << "查看第一个邻接顶点:";
	cin >> ch1;
	cout << g.getValue(g.getFirstNeighbor(g.getVertexPos(ch1))) << endl;
	cout << "查看后一个邻接顶点:";
	cin >> ch1 >> ch2;
	cout << g.getValue(g.getNextNeighbor(g.getVertexPos(ch1), g.getVertexPos(ch2))) << endl;

	cout << "插入顶点:";
	cin >> ch1;
	g.insertVertex(ch1);  //插入点
	cout << "插入边：" << endl;
	cin >> ch1 >> ch2 >> weight;
	g.insertEdge(g.getVertexPos(ch1), g.getVertexPos(ch2), weight);//插入边
	g.outputGraph();

	cout << "删除边：";
	cin >> ch1 >> ch2;
	g.removeEdge(g.getVertexPos(ch1), g.getVertexPos(ch2)); //删除边

	cout << "删除点：";
	cin >> ch1;
	g.removeVertex(g.getVertexPos(ch1));
	g.outputGraph();

}

void test_Graphlnk()
{
	cout << "-----------Grapglnk Test-----------" << endl;
	char ch1, ch2;
	int weight;
	Graphmtx<char, int> g(30);
	g.inputGraph();		//构造图
	g.outputGraph();	//显示图

	cout << "顶点数和边数：" << g.NumberOfVertices() << " " << g.NumberOfEdges() << endl;
	cout << "查看第一个邻接顶点:";
	cin >> ch1;
	cout << g.getValue(g.getFirstNeighbor(g.getVertexPos(ch1))) << endl;
	cout << "查看后一个邻接顶点:";
	cin >> ch1 >> ch2;
	cout << g.getValue(g.getNextNeighbor(g.getVertexPos(ch1), g.getVertexPos(ch2))) << endl;

	cout << "插入顶点:";
	cin >> ch1;
	g.insertVertex(ch1);  //插入点
	cout << "插入边：" << endl;
	cin >> ch1 >> ch2 >> weight;
	g.insertEdge(g.getVertexPos(ch1), g.getVertexPos(ch2), weight);//插入边
	g.outputGraph();

	cout << "删除边：";
	cin >> ch1 >> ch2;
	g.removeEdge(g.getVertexPos(ch1), g.getVertexPos(ch2)); //删除边

	cout << "删除点：";
	cin >> ch1;
	g.removeVertex(g.getVertexPos(ch1));
	g.outputGraph();
}