#include <iostream>
#include <string>
#include "Graphmtx.h"
#include "List.h"
using namespace std;

int main()
{
	string choice="\0", bin;//bin用来吃掉可能存在的多余无效输入
	cout << "**           电网造价模拟系统              **" << endl;
	cout << "=============================================" << endl;
	cout << "**           请选择要执行的操作            **" << endl;
	cout << "**           A --- 创建电网顶点            **" << endl;
	cout << "**           B --- 添加电网的边            **" << endl;
	cout << "**           C --- 构造最小生成树          **" << endl;
	cout << "**           D --- 显示最小生成树          **" << endl;
	cout << "**           E --- 退出程序                **" << endl;
	cout << "=============================================" << endl<<endl;//输出提示信息
	
	while (1)
	{
		if (choice == "\0")//用户的第一次操作
		{
			cout << "请选择操作:  ";
			cin >> choice;
			getline(cin, bin);
		}
		if (choice == "A")//创建电网顶点
		{
			int num_vertices;
			while (1)//对用书输入的顶点数的处理
			{
				cout << "请输入顶点的个数:  ";
				cin >> num_vertices;
				if (cin.fail() || num_vertices < 2|| num_vertices >100)//无效输入，重新输入，设定上限为100，防止遭遇大数攻击时宕掉
				{
					cout << "! 无效输入、数字小于下限2或大于上限100 !" << endl;
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

			Graphmtx<string, double> graph(num_vertices);
			List<string, double> vertex_list("*", "*", 0);
			string vertex;

			cout << "请依次输入各顶点的名称: " << endl;//建立顶点
			for (int i = 0; i < num_vertices; i++)
			{
				cin >> vertex;
				if (graph.getVertexPos(vertex) == -1)//防止顶点重复
					graph.insertVertex(vertex);
			}
			getline(cin, bin);

			while (1)//事实上，从A到D的是一个程序顺序执行的过程，例如：在显示最小生成树之前必须先构造最小生成树，构造之前必须先输入电网的边
			{
				cout << endl << "请选择操作:  ";
				cin >> choice;
				getline(cin, bin);
				if (choice != "A" && choice != "B" && choice != "E")
				{
					if (choice == "C" || choice == "D")//在C、D之前必须先
					{
						cout << "! 请先通过B操作添加电网的边 !" << endl;
						continue;
					}
					else
					{
						cout << "! 无效选项 !" << endl;
						continue;
					}
				}
				else
					break;
			}
			if (choice == "A")//表明用户要重建电网顶点
				continue;
			else if (choice == "E")//退出程序
				break;
			else if (choice == "B")//执行完A之后只能执行B或者重新执行一次A
			{                      //添加电网的边
				int num_edges;
				while (1)//处理用户输入的边的数量
				{
					cout << "请输入边的条数(为保证形成连通图，最终建立的边数不得少于" << num_vertices - 1 << "条):  ";
					cin >> num_edges;
					if (cin.fail() || num_edges < num_vertices - 1)//非法输入，重新输入
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
				for (int i = 0; i < num_edges; i++)
				{
					string h, t;
					double c;
					while (1)
					{
						cout << "请输入两个顶点和第" << i + 1 << "条边的权值:  ";
						cin >> h >> t >> c;
						if (cin.fail() || c <= 0 || graph.getVertexPos(h) == -1 || graph.getVertexPos(t) == -1)//输入的权值非法或者两个顶点不存在
						{
							cout << "! 无效边 !" << endl;
							cin.clear();
							getline(cin, bin);
							continue;
						}
						else
						{
							long long pos = vertex_list.FindEdge(h, t);//先找找用户要添加的边是不是已经存在了，如果存在，那就更新边的权值
							if (pos != -1)
							{
								cout << "边 " << vertex_list.FindNum(pos)->get_head() << "-("
									<< vertex_list.FindNum(pos)->get_cost() << ")-"
									<< vertex_list.FindNum(pos)->get_tail() << "的权值已更新为: " << c << endl;
								vertex_list.FindNum(pos)->set_cost(c);
								i--;
							}
							else//添加新边
								vertex_list.Insert(h, t, c, vertex_list.FindBigger(c));//通过FindBigger函数找到链表中第一个比要插入的边权值大的边的位置
							getline(cin, bin);                                         //把新边插入到该节点之前，这样在建立完链表之后，边已经按照权值
							break;                                                     //从小到大的顺序排好了序，便于后续执行Kruskal算法
						}
					}
				}

				while (1)
				{
					cout << endl << "请选择操作:  ";
					cin >> choice;
					getline(cin, bin);
					if (choice != "A" && choice != "C" && choice != "E")
					{
						if (choice == "B")
						{
							cout << "! 电网的边已添加完成 !" << endl;
							continue;
						}
						else if (choice == "D")//执行D之前必须先执行C
						{
							cout << "! 请先通过C操作建立最小生成树 !" << endl;
							continue;
						}
						else
						{
							cout << "! 无效选项 !" << endl;
							continue;
						}
					}
					else
						break;
				}
				if (choice == "A")//重设节点
					continue;
				else if (choice == "E")//退出程序
					break;
				else if (choice == "C")//建立最小生成树
				{
					graph.Kruskal(vertex_list);//利用Kruskal算法生成最小生成树
					cout << "Kruskal最小生成树生成成功!" << endl << endl;

					while (1)
					{
						cout << endl << "请选择操作:  ";
						cin >> choice;
						getline(cin, bin);

						if (choice != "A" && choice != "D" && choice != "E")
						{
							if (choice == "B")
							{
								cout << "! 电网的边已添加完成 !" << endl;
								continue;
							}
							else if (choice == "C")
							{
								cout << "! 最小生成树已建立完成 !" << endl;
								continue;
							}
							else
							{
								cout << "! 无效选项 !" << endl;
								continue;
							}
						}
						else
							break;
					}
					if (choice == "A")//重设顶点
						continue;
					else if (choice == "E")//退出程序
						break;
					else if (choice == "D")//输出最小生成树
					{
						graph.outputGraph();
						cout << endl;
						
						while (1)
						{
							cout << endl << "请选择操作:  ";
							cin >> choice;
							getline(cin, bin);
							if (choice == "A"|| choice == "E")
								break;
							else if (choice == "B")
							{
								cout << endl << "! 电网边已添加完成 !" << endl;
								continue;
							}
							else if (choice == "C")
							{
								cout << endl << "! Kruskal最小生成树已创建完成 !" << endl;
								continue;
							}
							else if (choice == "D")//在输出一次最小生成树
							{
								graph.outputGraph();
								cout << endl;
								continue;
							}
							else
							{
								cout << endl << "! 无效选项 !" << endl;
								break;
							}
						}
						if (choice == "A")
							continue;
						else if (choice == "E")
							break;
					}
				}
			}
		}
		else if (choice == "B"|| choice == "C" || choice == "D")//在执行B、C、D之前必须先执行A
		{
			cout << endl << "请先完成"<<choice<<"操作之前的步骤!" << endl;
			choice = "\0";
			continue;
		}
		else if (choice == "E")//退出程序
			break;
		else
		{
			cout << endl << "! 无效选项 !" << endl;
			choice = "\0";
			continue;
		}
	}
	//******************************************************************************************************
	//******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************
	return 0;
}