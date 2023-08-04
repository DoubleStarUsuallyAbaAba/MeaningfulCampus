#pragma once
#include <iostream>
using namespace std;

template<typename T>//冒泡排序
int BubbleSort(T* array,int len)
{
	int pass = 1, exchange = 1,ex_num=0;
	T* array_copy = new T[len];//生成备份数组，不能直接改动原来的数组，后续的排序操作要用
	for (int i = 0; i < len; i++)
		array_copy[i] = array[i];
	while (pass < len && exchange)//若中途出现不发生交换的情况，那么说明数组排序已经完成
	{
	    exchange = 0;
		int j = len - 1;
		for (; j >= pass; j--)//从后往前比较
		{
			if (array_copy[j - 1] > array_copy[j])//交换并统计交换次数
			{
				T temp = array_copy[j - 1];//交换
				array_copy[j - 1] = array_copy[j];
				array_copy[j] = temp;
				ex_num++;//交换次数计数
				exchange = 1;
			}
		}
		pass++;
	}
	delete []array_copy;//排序完成，删除备份数组
	return ex_num;
}
template<typename T>//选择排序
int SelectSort(T* array, int len)
{
	T* array_copy = new T[len];
	int ex_num = 0;
	for (int i = 0; i < len; i++)//生成备份数组，不能直接改动原来的数组，后续的排序操作要用
		array_copy[i] = array[i];
	for (int i = 0; i < len - 1; i++)
	{
		int min = i;//设定当前待排序序列的初始最小值位置
		for (int j = i + 1; j < len; j++)
		{
			if (array_copy[j] < array_copy[min])//遍历得到真实的当前待排序序列的最小值位置
				min = j;
		}
		if (min != i)//交换并计数次数
		{
			T temp = array_copy[i];
			array_copy[i] = array_copy[min];
			array_copy[min] = temp;
			ex_num++;
		}
	}
	delete []array_copy;//排序完成，删除备份数组
	return ex_num;
}
template<typename T>
int InsertSort(T* array, int len)//直接插入排序
{
	T* array_copy = new T[len];
	int ex_num = 0;
	for (int i = 0; i < len; i++)//生成备份数组，不能直接改动原来的数组，后续的排序操作要用
		array_copy[i] = array[i];
	for (int i = 1; i < len; i++)
	{
		T temp = array_copy[i];//暂存待插入元素
		int j = i;
		while (j > 0 && temp < array_copy[j - 1])//定位插入位置并交换、计数
		{
			array_copy[j] = array_copy[j - 1];
			j--;
			ex_num++;
		}
		if (j != i)//最后将暂存的元素插入相应位置
			array_copy[j] = temp;
	}
	delete []array_copy;//排序完成，删除备份数组
	return ex_num;
}
template <typename T>
int ShellSort(T* array,int len)//希尔排序
{
	T* array_copy = new T[len];
	int ex_num = 0, gap = len / 2;
	for (int i = 0; i < len; i++)//生成备份数组，不能直接改动原来的数组，后续的排序操作要用
		array_copy[i] = array[i];
	while (gap)//gap不为0则一直循环，进行排序操作
	{
		for (int i = gap; i < len; i++)//在通过gap划分的不同数字段中执行直接插入排序
		{
			T temp = array_copy[i];//暂存待插入元素
			int j = i;
			while (j >= gap && temp < array_copy[j - gap])//定位插入位置并交换、计数
			{
				array_copy[j] = array_copy[j - gap];
				j -= gap;
				ex_num++;
			}
			if (j != i)
				array_copy[j] = temp;//最后将暂存的元素插入相应位置
		}
		gap = gap/2;
	}
	delete []array_copy;
	return ex_num;
}
template <typename T>
void QuickSort(T* array_copy, int left,int right,int* EX_NUM)//快速排序
{
	if (left < right)
	{
		int pivotpos = left;//基准位置
		T pivot = array_copy[left];
		for (int i = left + 1; i < right; i++)
		{
			if (array_copy[i] < pivot && ++pivotpos != i)//把比基准数小的数字调到左边位置，相应的，较大的数字调到右边位置
			{
				T temp = array_copy[i];//交换
				array_copy[i] = array_copy[pivotpos];
				array_copy[pivotpos] = temp;
				(*EX_NUM)++;//交换次数计数
			}
		}
		T temp = array_copy[left];//交换
		array_copy[left] = array_copy[pivotpos];
		array_copy[pivotpos] = temp;
		(*EX_NUM)++;//交换次数计数
		QuickSort(array_copy, left, pivotpos - 1, EX_NUM);//快速排序基准左边的序列
		QuickSort(array_copy, pivotpos + 1, right, EX_NUM);//快速排序基准右边的序列
	}
}
template<typename T>
void FilterDown(T* array_copy, int current, int len,int *ex_num)//堆的向下调整，用于建立初始最大堆
{
	int child = 2 * current + 1;
	T temp = array_copy[current];
	while (child <= len)
	{
		if (child + 1 <= len && array_copy[child] < array_copy[child + 1])//定位两个子节点中较大节点的位置
			child += 1;
		if (temp >= array_copy[child])
			break;
		else//若父节点小于最大子节点，二者需要互换位置
		{
			array_copy[current] = array_copy[child];
			current = child;
			child = 2 * child + 1;
			(*ex_num)++;
		}
	}
	array_copy[current] = temp;
}
template <typename T>
int HeapSort(T* array, int len)//堆排序
{
	T* array_copy = new T[len];
	int ex_num = 0;
	for (int i = 0; i < len; i++)//生成备份数组，不能直接改动原来的数组，后续的排序操作要用
		array_copy[i] = array[i];
	for (int i = (len - 2) / 2; i >= 0; i--)
		FilterDown(array_copy, i, len-1, &ex_num);//生成初始最大堆
	for (int i = len-1; i >= 1; i--)
	{
		T temp = array_copy[0];//最大元素移到末尾，对除末尾段最大元素之外的元素重新生成最大堆
		array_copy[0] = array_copy[i];//交换并计数
		array_copy[i] = temp;
		ex_num++;
		FilterDown(array_copy, 0, i-1, &ex_num);//堆调整
	}
	delete[]array_copy;
	return ex_num;
}
template <typename T>
void Merge(T* initArray, T* mergedArray,int L, int M, int N,int* comp)//调整两个len长度序列中数字大小顺序
{
	int i = L, j = M + 1, k = L;
	while (i <= M && j <= N)//小的数字排在前面
	{
		if (initArray[i] <= initArray[j])
		{
			mergedArray[k] = initArray[i];
			i++;
			k++;
			(*comp)++;
		}
		else
		{
			mergedArray[k] = initArray[j];
			j++;
			k++;
			(*comp)++;
		}
	}
	if (i <= M)//将剩余的数字插入结果序列
	{
		for (int n1 = k, n2 = i; n1 <= N && n2 <= M; n1++, n2++)
			mergedArray[n1] = initArray[n2];
	}
	else
	{
		for (int n1 = k, n2 = j; n1 <= N && n2 <= N; n1++, n2++)
			mergedArray[n1] = initArray[n2];
	}
}
template <typename T>
void MergePass(T* initArray, T* mergedArray, int len,int num,int* comp)//对整个序列的所有数字序列执行一次归并操作
{
	int i = 0;
	while (i + 2 * len <= num)//调整所有的len长度序列中数字大小顺序
	{
		Merge(initArray, mergedArray,i, i+len-1, i+2*len - 1,comp);
		i += 2 * len;
	}
	if(i+len<=num-1)//调整剩余的长度序列中数字大小顺序
		Merge(initArray, mergedArray,i, i + len - 1, num - 1,comp);
	else
	{
		for (int j = i; j <= num - 1; j++)
			mergedArray[j] = initArray[j];
	}
}
template <typename T>
int MergeSort(T* array, int num)//归并排序
{
	int comp = 0;
	T* temp_1 = new T[num],*temp_2=new T [num];//建立两个备份数组用于排序，不可直接改动原数组，其他排序要用
	for (int i = 0; i < num; i++)
	{
		temp_1[i] = array[i];
		temp_2[i] = array[i];
	}
	int len = 1;
	while (len < num)//划分的子序列长度不断变长的过程
	{
		MergePass(temp_1, temp_2, len, num,&comp);
		len *= 2;
		MergePass(temp_2, temp_1, len, num,&comp);//最终的排序序列在temp_1中
		len *= 2;
	}
	cout << endl;
	delete[]temp_1;
	delete[]temp_2;//删除备份数组
	return comp;
}
template <typename T>
void RadixSort(T* array, const int len,int size)//基数排序
{
	T* array_copy = new T[len],*tmp=new T[len];
	for (int i = 0; i < len; i++)
		array_copy[i] = array[i];

	int count[10];
	int i, j, k;
	int radix = 1;
	for (i = 1; i <= size; i++) //进行size次排序
	{
		for (j = 0; j < 10; j++)
			count[j] = 0; //每次分配前清空计数器
		for (j = 0; j < len; j++)
		{
			k = (array_copy[j] / radix) % 10; //统计每个桶中的记录数
			count[k]++;
		}
		for (j = 1; j < 10; j++)
			count[j] = count[j - 1] + count[j]; //将tmp中的位置依次分配给每个桶
		for (j = len - 1; j >= 0; j--) //将所有桶中记录依次收集到tmp中
		{
			k = (array_copy[j] / radix) % 10;
			tmp[count[k] - 1] = array_copy[j];
			count[k]--;
		}
		for (j = 0; j < len; j++) //将临时数组的内容复制到data中
			array_copy[j] = tmp[j];
		radix = radix * 10;
	}

	delete[]array_copy;
	delete[]tmp;
}


