#include <iostream>
#include <string>
#include "class.h"
using namespace std;

int wash(string num)//判断输入的数字集合中是否存在不合规范的情况
{
	unsigned int i=0;
	for (i = 0; i < num.length(); i++)//遍历数字集合
	{
		if ((num[i] >= '0' && num[i] <= '9') || num[i] == ' ')//数字与空格是合规的
		{
			i++;//遇到纯数字或者空格，计数
			continue;
		}
		else if (i == 0 && (num[i] == '+' || num[i] == '-') && num[i + 1] >= '0' && num[i + 1] <= '9')//对于有正负标号的数字也是合规的，注意防止越界
		{
			i++;//遇到表示数字正负的符号，计数
			continue;
		}
		else
		{//对于有正负标号的数字也是合规的，注意防止越界
			if ((i < num.length() - 1) && (num[i] == '+' || num[i] == '-') && num[i - 1] == ' ' && num[i + 1] >= '0' && num[i + 1] <= '9')
			{
				i++;//遇到表示数字正负的符号，计数
				continue;
			}
			else
				break;
		}
	}
	if (i >= num.length())//如果合规的符号不小于字符串长度，那么整个字符串都是符合输入规范的
		return 1;
	else
		return 0;//否则，字符串中一定含有不合规的字符
}

int main()
{
	string choice, bin, num_set;//num_set用来暂存用户输入的所有数字
	BinaryTree<int> tree(0);
	cout << "==============================================" << endl;
	cout << "***           整数二叉树排序               ***" << endl;
	cout << "==============================================" << endl;
	cout << "***         1.---建立或重建二叉排序树      ***" << endl;
	cout << "***         2.---插入元素                  ***" << endl;
	cout << "***         3.---查询元素                  ***" << endl;
	cout << "***         4.---退出程序                  ***" << endl;
	cout << "==============================================" << endl << endl;

	while (1)
	{
		cout << endl << "Please select: ";
		cin >> choice;
		getline(cin, bin);//吃掉可能存在的多余无效输入，bin的功能类似于垃圾桶，故命此名

		if (choice == "1")//建立或重建二叉排序树
		{
			if (tree.GetRoot()->GetLeft() != NULL)//在重建二叉树之前先删除之前的二叉树
				tree.destroy(tree.GetRoot()->GetLeft());
			cout << "Please input keys to build or rebuild a binary sorting tree: " << endl;
			while (1)
			{
				getline(cin, num_set);//读入用户输入的数字
				if (num_set.length())
					break;
			}
			if (!wash(num_set))//对非法输入的判断与处理
			{
				cout << "! illegal input !" << endl;//非法输入，重新选择操作
				continue;
			}
			else
			{
				num_set += ' ';
				for (unsigned int i = 0; i < num_set.length() - 1;)
				{
					unsigned int j = i;
					while (num_set[i] != ' ')
					{
						i++;//过滤出数字，i-j得出该数字占据着几位字符，以便于转换成数字存入二叉排序树
					}
					if (j == i)//忽略掉空格
					{
						i++;
						continue;
					}
					else
					{
						int key = stoi(num_set.substr(j, i - j));//从字符串中截取并转换成出一个数字，可正可负，带不带正负号均可
						TreeNode<int>* exist = tree.FindTarget(key, tree.GetRoot()->GetLeft());//判断二叉树中是否已经有相同数字，二叉排序树不允许有相同数字
						if (exist != NULL)
						{
							cout << "Your input key<" << key << "> has existed !" << endl;
							continue;
						}
						else//若无相同的数字，则按照建立二叉排序树的方式将新数字建立在二叉排序树中
						{
							tree.BSTInsert(key, tree.GetRoot()->GetLeft());
							continue;
						}
					}
				}
			}
			cout << "Sorted numbers :" << endl;
			tree.MiddleTravel(tree.GetRoot()->GetLeft());//给出排序结果
			continue;
		}
		else if (choice == "2")//插入元素
		{
			int key;
			cout << "Please input a key to insert and sort:  ";
			while (1)
			{
				cin >> key;
				if (cin.fail())//非法输入，重新输入
				{
					cout << "当前输入非法，请重新输入一个整数: ";
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
			TreeNode<int>* exist = tree.FindTarget(key, tree.GetRoot()->GetLeft());//判断是否树中已经有相同数字
			if (exist != NULL)
				cout << "Your input key<" << key << "> has existed !" << endl;
			else
				tree.BSTInsert(key, tree.GetRoot()->GetLeft());//无相同数字则插入
			cout << "Sorted numbers :" << endl;
			tree.MiddleTravel(tree.GetRoot()->GetLeft());//给出新的排序序列
			continue;
		}
		else if (choice == "3")//查询元素
		{
			int key;
			cout << "Please input a key to search:  ";
			while (1)
			{
				cin >> key;
				if (cin.fail())//非法输入，重新输入
				{
					cout << "当前输入非法，请重新输入一个整数: ";
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
			TreeNode<int>* exist = tree.FindTarget(key, tree.GetRoot()->GetLeft());//判断是否树中已经有相同数字
			if (exist != NULL)
				cout << "search successfully !" << endl;
			else
				cout << key << " doesn't existed !" << endl;
			continue;
		}
		else if (choice == "4")//退出程序
			break;
		else//无效选项
		{
			cout << endl << "! Illegal Choice !" << endl;
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
// 测试案例： 12 34 67 48 19 44 21 30 19 7 4 24 9 88 100 100 0