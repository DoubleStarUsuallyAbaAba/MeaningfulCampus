#include <iostream>
#include <string>
#include <ctime>
#include "sort.h"
using namespace std;

clock_t start, finish;//用于计时

//产生n个随机整数
int* RandomArray(int n, int rangL=0, int rangR=10000)//n为产生随机数的数量，rangL和rangR为随机数的范围，返回值类型为指针

{
	int* arr = new int[n];
	srand(static_cast<unsigned int>(time(NULL)));//使用当前时间进行随机数发生器的初始化，使每次生成的随机序列不一样
	for (int i = 0; i < n; i++)
	{
		arr[i] = rand() % (rangR - rangL + 1) + rangL;//使随机数范围在[rangL,rangR]之内
	}

	return arr;//return 的也是指针
}

int main()
{
	int num;
	int* numArray = NULL;
	string choice,bin;//bin专门用来吃掉多余的非法输入，功能类似于垃圾桶，故命此名
	cout << "**            排序算法比较                **" << endl;
	cout << "============================================" << endl;
	cout << "**            1 --- 冒泡排序              **" << endl;
	cout << "**            2 --- 选择排序              **" << endl;
	cout << "**            3 --- 直接插入排序          **" << endl;
	cout << "**            4 --- 希尔排序              **" << endl;
	cout << "**            5 --- 快速排序              **" << endl;
	cout << "**            6 --- 堆排序                **" << endl;
	cout << "**            7 --- 归并排序              **" << endl;
	cout << "**            8 --- 基数排序              **" << endl;
	cout << "**            9 --- 退出程序              **" << endl;
	cout << "============================================" << endl;//开头提示信息的输出，相比于demo又新增了几项功能
	
	while (1)//输入要产生的随机数的个数，并判断是否存在非法输入
	{
		cout << "请输入要产生的随机数的个数: ";
		cin >> num;
		if (cin.fail() || num < 2||num>50000)
		{
			cin.clear();
			getline(cin, bin);//吃掉可能存在的多余无效输入
			cout << "! 输入非数字字符、输入的数字小于下限2或超出上限50000，无效输入 !" << endl;
			continue;
		}
		else
		{
			getline(cin, bin);//吃掉可能存在的多余无效输入
			numArray = RandomArray(num, 0, num);//随机序列生成
			cout << endl;
			break;
		}
	}

	while (1)
	{
		cout <<endl<< "请选择排序算法:  ";
		cin >> choice;
		if (choice == "9")//退出程序
			break;
		else if (choice == "1")//冒泡排序
		{
			start = clock();//取起始时间
			int ex_num=BubbleSort<int>(numArray,num);//调用冒泡排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;//转换成以秒为单位的时间差
			cout << "冒泡排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "冒泡排序交换次数:   " << ex_num << " 次" << endl;
			continue;
		}
		else if (choice == "2")//选择排序
		{
			start = clock();//取起始时间
			int ex_num = SelectSort<int>(numArray, num);//调用选择排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "选择排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "选择排序交换次数:   " << ex_num << " 次" << endl;
			continue;
		}
		else if (choice == "3")
		{
			start = clock();//取起始时间
			int ex_num = InsertSort<int>(numArray, num);//调用直接插入排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "直接插入排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "直接插入排序交换次数:   " << ex_num << " 次" << endl;
			continue;
		}
		else if (choice == "4")
		{
			start = clock();//取起始时间
			int ex_num = ShellSort<int>(numArray, num);//调用希尔排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "希尔排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "希尔排序交换次数:   " << ex_num << " 次" << endl;
			continue;
		}
		else if (choice == "5")
		{
			int* array_copy = new int[num];
			int ex_num = 0;
			for (int i = 0; i < num; i++)
				array_copy[i] = numArray[i];

			start = clock();//取起始时间
			QuickSort<int>(array_copy,0, num,&ex_num);//调用快速排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "快速排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "快速排序交换次数:   " << ex_num << " 次" << endl;
			delete[] array_copy;
			continue;
		}
		else if (choice == "6")
		{
			start = clock();//取起始时间
			int ex_num = HeapSort<int>(numArray, num);//调用堆排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "堆排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "堆排序交换次数:   " << ex_num << " 次" << endl;
			continue;
		}
		else if (choice == "7")
		{
			start = clock();//取起始时间
			int comp=MergeSort<int>(numArray, num);//调用归并排序函数，参数：待排序数组、随机数个数
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "二路归并排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "二路归并排序比较次数:   "<<comp<<" 次" << endl;
			continue;
		}
		else if (choice == "8")
		{
			start = clock();//取起始时间
			RadixSort<int>(numArray, num,to_string(num).length());//调用基数排序函数，参数：待排序数组、随机数个数、随机数最大位数（本程序将随机数大小上限设定为随机数个数）
			finish = clock();//取终止时间
			double time_cost = (static_cast<double>(finish) - start) / CLOCKS_PER_SEC;
			cout << "基数排序所用时间:   " << time_cost << " 秒" << endl;
			cout << "基数排序交换次数:   0 次" << endl;//基数排序中不存在交换与比较
			continue;
		}
		else
		{
			getline(cin, bin);
			cout << "! 无效选项 !" << endl;
			continue;
		}
	}


	delete[]numArray;
	cout << endl;
	system("pause");
	return 0;
}