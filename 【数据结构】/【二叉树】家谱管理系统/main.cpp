#include <iostream>
#include <string>
#include "class.h"
using namespace std;

int main()
{
	string temp,trash;//temp用来暂存一些数据，trash专门用来吃掉多余的非法输入，功能类似于垃圾桶，故命此名
	cout << "**            家谱管理系统                **" << endl;
	cout << "============================================" << endl;
	cout << "**         请选择要执行的操作             **" << endl;
	cout << "**            A --- 完善家谱              **" << endl;
	cout << "**            B --- 添加家庭成员          **" << endl;
	cout << "**            C --- 解散局部家庭          **" << endl;
	cout << "**            D --- 更改家庭成员姓名      **" << endl;
	cout << "**            E --- 查询兄弟              **" << endl;
	cout << "**            F --- 查询子女              **" << endl;
	cout << "**            G --- 查询父母              **" << endl;
	cout << "**            H --- 查询祖先              **" << endl;
	cout << "**            I --- 退出程序              **" << endl;
	cout << "============================================" << endl;//开头提示信息的输出，相比于demo又新增了几项功能
	cout << "请首先建立一个家谱!" << endl;
	cout << "请输入祖先的姓名: ";
	cin >> temp;
	getline(cin, trash);//吃掉可能存在的多余无效输入

	//先建立家谱的祖先
	BinaryTree<string> Family("ORIGINAL_ANCESTOR");
	TreeNode<string>* node = Family.GetRoot();
	Family.Insert(temp, KID, node);
	cout << "此家谱的祖先是: " << node->GetLeft()->GetData() << endl<<endl;

	while (1)
	{
		string choice;
		cout << "请选择要执行的操作: ";
		cin >> choice;
		getline(cin, trash);
		if (choice == "I")//退出程序
		{
			break;
		}
		else if(choice=="A"|| choice == "B" || choice == "C" || choice == "D" || choice == "E" || choice == "F" || choice == "G" || choice == "H" )
		{
			if (Family.GetRoot()->GetLeft() == NULL)//每次操作前先检查祖先是否还在，若不在必须先重新建立祖先，否则是一颗空树，代码执行会出问题
			{
				string new_ancestor;
				cout << "原家谱的祖先已被上次操作抹除，请重新输入家谱祖先:   ";
				cin >> new_ancestor;
				getline(cin, trash);
				Family.RsetRoot(new_ancestor);//因为本程序的数是额外带一个根节点的，所以再重新插入一个祖先就好
				cout << "新家谱的祖先是: " << Family.GetRoot()->GetLeft()->GetData() << endl << endl;
			}
			if (choice == "A")//完善家庭
			{
				string name,bin;
				cout << "请输入要建立家庭的人的姓名:   ";
				cin >> name;
				getline(cin, trash);
				node = Family.FindTarget(name, Family.GetRoot()->GetLeft());//在当前的家谱树中先找一找是否存在该人物
				if (node == NULL)//家谱中查无此人
				{
					cout << "现存家谱中并不存在此人，家庭建立失败!" << endl << endl;
					continue;
				}
				else
				{
					if (node->GetLeft() != NULL)//考察该人是否已经有子女，若有，则给出提示信息
						cout << name << "已存在直系第一代子女，当前操作中输入的子女名字将视为" << name << "新增子女" << endl;
					long long num;//添加的子女人数
					string bin;
					cout << "请输入" << name << "此次添加的子女人数   ";
					while (1)
					{
						cin >> num;
						if (cin.fail() || num < 0||num>10)//此处设置一个上限，仅仅是为了防止程序在遭遇大数恶意攻击时宕掉
						{
							cout << "输入非法或数字超过10(国家现在，2021年最多是三胎政策哦)，请重新输入:   ";
							cin.clear();
							getline(cin, bin);
							continue;
						}
						else
						{
							getline(cin, trash);
							break;
						}
					}
					if (num == 0)//无新增子女，自然不需要执行后续操作
					{
						cout << name << "新增子女人数为0" << endl << endl;
						continue;
					}
					else
					{
						cout << "请依次输入" << name << "的子女的姓名:   ";
						for (long long i = 0; i < num; i++)
						{
							cin >> bin;
							Family.Insert(bin, KID, Family.FindTarget(name, Family.GetRoot()->GetLeft()));//先通过FindTarget函数定位要插入子女的人在树中的位置，然后再插入
						}
						getline(cin, trash);
						cout << name << "的第一代子孙为: ";
						Family.Kid(Family.FindTarget(name, Family.GetRoot()->GetLeft()));//输出该人的第一代子孙
						cout << endl << endl;
						continue;
					}
				}
			}
			else if (choice == "B")//添加家庭成员
			{
				string name, bin;
				cout << "请输入要添加子女的人的姓名:   ";
				cin >> name;
				getline(cin, trash);

				node = Family.FindTarget(name, Family.GetRoot()->GetLeft());//先判断当前树中有没有该人物
				if (node == NULL)
				{
					cout << "现存家谱中并不存在此人，子女添加失败!" << endl << endl;
					continue;
				}
				else
				{
					cout << "请输入" << name << "新添加的子女的姓名:   ";//输入添加的子女姓名
					cin >> bin;
					getline(cin, trash);

					Family.Insert(bin, KID, Family.FindTarget(name, Family.GetRoot()->GetLeft()));//先通过FindTarget函数定位要插入子女的人在树中的位置，然后再插入
					cout << name << "的第一代子孙为: ";
					Family.Kid(Family.FindTarget(name, Family.GetRoot()->GetLeft()));//输出该人的第一代子孙
					cout << endl << endl;
					continue;
				}
			}
			else if (choice == "C")//解散局部家庭
			{
				string name, bin;
				cout << "请注意，一旦某人被解散家庭，其本人及其所有的子孙都将被从家谱中抹去，请慎重选择此操作!" << endl;
				cout << "输入 Y 继续操作，输入 N 取消此操作:   ";
				while (1)
				{
					cin >> bin;
					getline(cin, trash);
					if (bin != "Y" && bin != "N")//由于一旦执行删除操作，原来数据无法恢复，因此这里为用户提供了取消当前删除操作的选项，降低用户误触的概率
					{
						cout << "非法输入，请重新输入:   ";
						continue;
					}
					else
						break;
				}
				if (bin == "N")//取消当前删除操作
				{
					cout << endl << endl;
					continue;
				}
				cout << "请输入被解散家庭的人的姓名:   ";
				cin >> name;
				getline(cin, trash);

				node = Family.FindTarget(name, Family.GetRoot()->GetLeft());//判断当前树中有无该人物
				if (node == NULL)//家谱中不存在该人
				{
					cout << "现存家谱中并不存在此人，操作失败!" << endl << endl;
					continue;
				}
				else
				{
					cout << "被解散家庭的人是: " << name << endl;
					cout << name << "的第一代子孙是:   ";
					Family.Kid(node);
					cout << endl;

					if (node->GetRight() != NULL && node != Family.GetRoot()->GetLeft())//在删除某个节点之前先将以该节点为根的子树
					{                                                                   //从原树中脱离下来成为一棵单独的树再进行删除
						if (node->GetParent()->GetLeft() == node)//当要删除的节点是父节点的伦理子女
							node->GetParent()->SetLeft(node->GetRight());
						else   //当要删除的节点是其父节点的伦理兄弟
							node->GetParent()->SetRight(node->GetRight());
						node->GetRight()->SetParent(node->GetParent());
					}
					//此时，已经从原树中分离除了完整的待删除子树，且该子树的根节点是没有右子节点的
					//删除该子树
					if (node->GetLeft() != NULL)
						Family.destroy(node->GetLeft());
					Family.kill(node);
					cout << endl << endl;
					continue;
				}
			}
			
			else if (choice == "D")//更改家庭成员姓名
			{
				string name,bin;
				TreeNode<string>* temp;
				cout << "请输入要更改姓名的人的目前名字:   ";
				while (1)
				{
					cin >> name;
					getline(cin, trash);
					temp=Family.FindTarget(name, Family.GetRoot()->GetLeft());//先找一找当前树中是否有该操作对象
					if (temp == NULL)
					{
						cout << "查无此人，请重新输入要更改姓名的人目前的名字:   ";
						continue;
					}
					else
					{
						cout << "请输入更改后的姓名:   ";
						cin >> bin;
						getline(cin, trash);
						break;
					}
				}
				temp->SetData(bin);//重新设定该节点姓名即可
				cout << name << "已经更名为:  " << temp->GetData() << endl << endl;
				continue;
			}
			else if (choice == "E")//查询节点伦理兄弟
			{
				string name;
				TreeNode<string>* temp,*not_show;
				cout << "请输入要查询兄弟的人的名字:   ";
				cin >> name;
				getline(cin, trash);
				temp = Family.FindTarget(name, Family.GetRoot()->GetLeft());//先找一找当前树中是否有该操作对象
				if (temp == NULL)
					cout << name << "不在当前家谱中!";
				else
				{
					not_show = temp;
					temp = Family.FindParent(temp);//要想得到全部伦理兄弟，需要先找到伦理父母，然后输出其子女即可
					cout << name << "的兄弟为: ";
					Family.Kid(temp,not_show);
				}
				cout << endl << endl;
				continue;
			}
			else if (choice == "F")//查询节点伦理子女
			{
				string name;
				TreeNode<string>* temp;
				cout << "请输入要查询子女的人的名字:   ";
				cin >> name;
				getline(cin, trash);
				temp = Family.FindTarget(name, Family.GetRoot()->GetLeft());//先找一找当前树中是否有该操作对象
				if (temp == NULL)
					cout << name << "不在当前家谱中!";
				else
				{
					cout << name << "的子女为: ";
					Family.Kid(temp);//直接调用Kid函数即可
				}
				cout << endl << endl;
				continue;
			}
			else if (choice == "G")//查询节点伦理父母
			{
				string name;
				TreeNode<string>* temp;
				cout << "请输入要查询父母的人的名字:   ";
				cin >> name;
				getline(cin, trash);
				temp = Family.FindTarget(name, Family.GetRoot()->GetLeft());//先找一找当前树中是否有该操作对象
				if (temp == NULL)
					cout << name << "不在当前家谱中!";
				else if (temp == Family.GetRoot()->GetLeft())//当前家谱的原始祖先是不清楚其伦理父母的
					cout << name << "为家谱记载的原始祖先，父母未知";
				else
				{
					cout << name << "的父母为: ";
					temp=Family.FindParent(temp);//直接调用FindParent函数即可
					cout << temp->GetData();
				}
				cout << endl << endl;
				continue;
			}
			else if (choice == "H")//查询家谱原始祖先
			{
				cout << "当前家谱的原始祖先是:  " << Family.GetRoot()->GetLeft()->GetData() << endl << endl;
				continue;
			}
		}
		else//用户输入了其他的无效选项
		{
			cout << "输入无效选项!" << endl << endl;
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