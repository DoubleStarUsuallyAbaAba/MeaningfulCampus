#include <iostream>
#include <string>
using namespace std;

int input(int value,char value_name)//输入函数
{
	string bin;
	cout << endl << "请输入" << value_name << "的值（0或1）, 以回车结束: ";
	while (1)
	{
		cin >> value;
		if ((value != 0 && value != 1) || cin.fail())//输入非数字字符或者输入0、1之外的数字字符为无效输入，需要重新输入
		{
			cin.clear();
			getline(cin, bin);//吃掉可能存在的多余无效输入
			cout << endl << value_name<<"的值输入有误, 请重新输入: ";
			continue;
		}
		else
			break;
	}
	return value;
}

int main()
{
	int p=-1, q=-1, result[4];
	char choice;
	string bin;
	while (1)
	{
		cout << "***************************************" << endl;
		cout << "**                                   **" << endl;
		cout << "**        欢迎进入逻辑运算程序       **" << endl;
		cout << "**                                   **" << endl;
		cout << "***************************************" << endl;
		p = input(p, 'P');//输入
		q = input(q, 'Q');
		result[0] = p && q;  //与运算
		result[1] = p || q;  //或运算
		result[2] = (!p) || q;  //蕴含运算，将其转化为与或非形式
		result[3] = ((!p) || q) && ((!q) || p);  //等值运算，将其转化为与或非形式
		cout << endl;
		cout << "合取运算:" << endl << "P∧Q = " << result[0] << endl;
		cout << "析取运算:" << endl << "P∨Q = " << result[1] << endl;
		cout << "蕴含运算:" << endl << "P→Q = " << result[2] << endl;
		cout << "等值运算:" << endl << "P <-> Q = " << result[3] << endl;

		cout << endl << "是否继续运算?（y/n）: ";
		while (1)//处理非法输入
		{
			cin >> choice;
			if (choice != 'y' && choice != 'n')
			{
				getline(cin, bin);
				cout << endl << "输入错误,请重新输入y或n : ";
				continue;
			}
			else
				break;
		}
		if (choice == 'y')//继续程序
		{
			system("cls");  //清屏
			continue;
		}
		else if (choice == 'n')//结束程序
		{
			cout << "欢迎下次再次使用!" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}