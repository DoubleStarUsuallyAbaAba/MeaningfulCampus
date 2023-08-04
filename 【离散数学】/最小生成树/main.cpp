#include <iostream>
#include <string>
#include "Graphmtx.h"
#include "List.h"
using namespace std;

int main()
{
	string bin, vertex;//bin专门用来吃掉多余的无效输入,vertex用来暂存图的顶点
	int num_vertices, num_edges;//顶点数、边数

	cout << "======================================================" << endl;
	cout << "**           Kruskal最小生成树模拟系统              **" << endl;
	cout << "======================================================" << endl;

	while (1)//输入顶点个数
	{
		cout << "请输入顶点的个数[2,100]:  ";
		cin >> num_vertices;
		if (cin.fail() || num_vertices < 2|| num_vertices>100)//若输入的顶点数是非数字字符或者数字超过了上下限，需要重新输入
		{
			cout << "! 无效输入 !" << endl;
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}

	Graphmtx<string, double> graph(num_vertices);//建立图
	List<string, double> vertex_list("*", "*", 0);//建立边链表

	cout << "请依次输入各顶点的名称(中间以空格隔开): " << endl;//输入并建立各顶点
	for (int i = 0; i < num_vertices; i++)
	{
		cin >> vertex;
		if (graph.getVertexPos(vertex) == -1)
			graph.insertVertex(vertex);
		else//如果输入了重名顶点，则无效
			i--;
	}
	getline(cin, bin);

	while (1)//输入边的条数
	{
		cout << "请输入边的条数(为保证形成连通图，最终建立的边数不得少于" << num_vertices - 1 << "条):  ";
		cin >> num_edges;
		if (cin.fail() || num_edges < num_vertices - 1)//无效输入，重新输入
		{
			cout << "! 无效输入 !" << endl;
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
		{
			getline(cin, bin);
			break;
		}
	}
	for (int i = 0; i < num_edges; i++)//输入并建立各边
	{
		string h, t;//边两端的顶点
		double c;//边权值
		while (1)
		{
			cout << "请输入第" << i + 1 << "条边的两个顶点和权值:  ";
			cin >> h >> t >> c;
			if (cin.fail() || c <= 0 || graph.getVertexPos(h) == -1 || graph.getVertexPos(t) == -1)
			{
				cout << "! 无效边 !" << endl;
				cin.clear();
				getline(cin, bin);
				continue;
			}
			else
			{
				long long pos = vertex_list.FindEdge(h, t);
				if (pos != -1)//如果是已经存在的边，只是权值不同，则视为对已有边的权值进行更新，而不是插入新边
				{
					cout << "边 " << vertex_list.FindNum(pos)->get_head() << "-("
						<< vertex_list.FindNum(pos)->get_cost() << ")-"
						<< vertex_list.FindNum(pos)->get_tail() << "的权值已更新为: " << c << endl;
					vertex_list.FindNum(pos)->set_cost(c);
					i--;
				}
				else
					vertex_list.Insert(h, t, c, vertex_list.FindBigger(c));//将新边插入图中
				getline(cin, bin);
				break;
			}
		}
	}

	graph.Kruskal(vertex_list);//利用Kruskal算法建立最小生成树
	graph.outputGraph();//输出最小生成树
	cout << endl;

	system("pause");
	return 0;
}