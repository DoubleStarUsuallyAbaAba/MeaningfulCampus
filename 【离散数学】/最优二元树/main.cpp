#include <iostream>
#include <string>
using namespace std;
#define N 1000
int num=0;
struct tree
{
	int num;
	struct tree* Lnode;
	struct tree* Rnode;
}*fp[N];     //保存结点 
char s[N] = { '\0' };  //放前缀码

void init_node(int f[], int n)  //生成叶子结点
{
	int i;
	struct tree* pt;
	for (i = 0; i < n; i++)
	{
		pt = new tree;  //生成叶子结点
		pt->num = f[i];
		pt->Lnode = NULL;
		pt->Rnode = NULL;
		fp[i] = pt;
	}
}

void sort(struct tree* array[], int n)  //将第N-n个点插入到已排好序的序列中
{
	int i;
	struct tree* temp;
	for (i = n; i < num - 1; i++)
		if (array[i]->num > array[i + 1]->num)
		{
			temp = array[i + 1];
			array[i + 1] = array[i];
			array[i] = temp;
		}
}

struct tree* construct_tree(int f[], int n)  //建立树 
{
	int i;
	struct tree* pt;
	for (i = 1; i < n; i++)
	{
		pt = new tree;  //生成非叶子结点
		pt->num = fp[i - 1]->num + fp[i]->num;
		pt->Lnode = fp[i - 1];
		pt->Rnode = fp[i];
		fp[i] = pt;  //wl+w2
		sort(fp,i);
	}
	return fp[n - 1];
}

void preorder(struct tree* p, int k, char c)//前序遍历树
{
	int j;
	if (p != NULL)
	{
		if (c == 'L')
			s[k] = '0';
		else 
			s[k] = '1';
		if (p->Lnode == NULL)
		{   //P 指向叶子 
			cout << p->num<<" : ";
			for (j = 0; j <= k; j++)
				cout<<s[j];
			cout<<endl;
		}
		preorder(p->Lnode, k + 1, 'L');
		preorder(p->Rnode, k + 1, 'R');
	}
}
void free_tree(struct tree* p)//释放树的存储空间
{
	if (p->Lnode == NULL&&p->Rnode==NULL)
		delete p;
	else
	{
		free_tree(p->Lnode);
		free_tree(p->Rnode);
		delete p;
	}
}
void quicksort(int a[], int left, int right)//快速排序算法，将权值从小到大排序，便于建树
{
	int i, j, t, temp;
	if (left > right)
		return;

	temp = a[left];
	i = left;
	j = right;
	while (i != j) {
		while (a[j] >= temp && j > i)
			j--;
		while (a[i] <= temp && j > i)
			i++;
		if (i < j) {
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
	}
	a[left] = a[i];
	a[i] = temp;

	quicksort(a,left, i - 1);
	quicksort(a,i + 1, right);
}
int main()
{
	string bin;
	cout << "请输入节点个数(必须是不超过"<<N<<"的正整数):  ";
	while (1)//输入节点数目，若输入非数字字符或者数字大小超范围，则重新输入
	{
		cin >> num;
		if (cin.fail() || num<1 || num>N)
		{
			cout << "节点数目非法，请重新输入:  ";
			cin.clear();
			getline(cin, bin);
			continue;
		}
		else
			break;
	}
	int f[N];
	for (int i = 0; i < N; i++)//初始化数组
		f[i]=-1;
	cout << "请输入节点(以空格分隔):  ";
	for (int i = 0; i < num; i++)//输入各节点权值
		cin >> f[i];
	quicksort(f, 0, num - 1);//将频率权值从小到大排序
	struct tree* head;
	init_node(f, num);  //初始化结点 
	head = construct_tree(f, num);  //生成最优树 
	preorder(head, 0, 'L');  //遍历树 
	free_tree(head);//删除树
	system("pause");
	return 0;
}
//2 3 5 7 11 13 17 19 23 29 31 37 41
//3 5 7 11 2 13 17 19 41 23 29 31 37