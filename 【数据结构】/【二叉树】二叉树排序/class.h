#pragma once
#include <iostream>
using namespace std;
#define KID 0 //宏定义左子树方向为0
#define BROTHER 1 //宏定义右子树方向为1

template <class T>
class TreeNode//树节点类
{    //对实例化类模板时的数据类型作出限定，主要是为了防止建立string或者char类型的二叉树节点类
	static_assert(sizeof(T) == sizeof(int) || sizeof(T) == sizeof(double),"Your T type is not the allowed DataType whose size is 4 or 8 bytes.");
private:
	T data;
	TreeNode<T>* LeftNode, * RightNode, * Parent;
public:
	TreeNode(T item = NULL, TreeNode<T>* parent = NULL, TreeNode<T>* left = NULL, TreeNode<T>* right = NULL) :data(item), Parent(parent), LeftNode(left), RightNode(right) {}
	T GetData() { return data; }//返回节点本身的数据信息
	TreeNode<T>* GetParent() { return Parent; }//获取当前节点的父节点
	TreeNode<T>* GetLeft() { return LeftNode; }//获取当前节点的左子节点
	TreeNode<T>* GetRight() { return RightNode; }//获取当前节点的右子节点
	void SetData(T item) { data = item; }//设定当前节点的数据
	void SetParent(TreeNode<T>* parent) { Parent = parent; }//设定当前节点的父节点
	void SetLeft(TreeNode<T>* left = NULL) { LeftNode = left; }//设定当前节点的左子节点
	void SetRight(TreeNode<T>* right = NULL) { RightNode = right; }//设定当前节点的右子节点
};
template <class T>
class BinaryTree//二叉树类
{    //对实例化类模板时的数据类型作出限定，主要是为了防止建立string或者char类型的二叉树类
	static_assert(sizeof(T) == sizeof(int) || sizeof(T) == sizeof(double), "Your T type is not the allowed DataType whose size is 4 or 8 bytes.");
private:
	TreeNode<T>* root, * current;//根节点、当前节点
public:
	BinaryTree(T value = NULL)//构造函数
	{
		root = current = new TreeNode<T>(value);
	}
	~BinaryTree() { destroy(root); }//析构函数
	TreeNode<T>* GetRoot()//获取根节点
	{
		if (root == NULL)
			return NULL;
		return root;
	}
	TreeNode<T>* MaxNode()//获取二叉排序树中最大的节点，位置在二叉排序树的最右分支的最后一个节点，这也是中序遍历最后一个访问的节点
	{
		TreeNode<T>* max = root->GetLeft();//注意不要忽略根节点，本程序中的二叉排序树是带根节点的，根节点下面吊着的子树才是纯的二叉排序树
		while (max->GetRight() != NULL)//只要没到最右分支的最后一个节点，就一直循环
		{
			max = max->GetRight();
		}
		return max;//返回最大值节点的地址
	}
	TreeNode<T>* BSTInsert(T item, TreeNode<T>* location);//构建二叉排序树的插入方式，小数插入在左节点，大数插入在右节点
	TreeNode<T>* FindTarget(T item, TreeNode<T>* start);//从start位置开始寻找值为item的第一个节点
	void MiddleTravel(TreeNode<T>* start);//中序遍历二叉树
	void destroy(TreeNode<T>* location);//用来删除location节点的所有后代
	void kill(TreeNode<T>* location)//用来删除特定的叶节点或者孤立节点
	{
		if (location != root)
		{
			if (location->GetParent()->GetLeft() == location)
				location->GetParent()->SetLeft();//SetLeft()无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL,下同
			else
				location->GetParent()->SetRight();
		}
		delete location;
	}
};
template <class T>
void BinaryTree<T>::destroy(TreeNode<T>* location)//删除以location为根节点的树
{
	if (location != NULL)//相当于用后续遍历的流程释放各节点所占用的内存空间
	{
		destroy(location->GetLeft());
		destroy(location->GetRight());
		if (location != root)
		{
			if (location->GetParent()->GetLeft() == location)
				location->GetParent()->SetLeft();//SetLeft()无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL,下同
			else
				location->GetParent()->SetRight();
		}
		delete location;
	}
}
template <class T>
void BinaryTree<T>::MiddleTravel(TreeNode<T>* start)//中序遍历
{
	if (start != NULL)//递归思路
	{
		T show = start->GetData();
		MiddleTravel(start->GetLeft());
		cout << show;
		if (show == MaxNode()->GetData())
			cout << endl;
		else
			cout << " -> ";
		MiddleTravel(start->GetRight());
	}
}
template <class T>
TreeNode<T>* BinaryTree<T>::BSTInsert(T item,TreeNode<T>* location)//构建二叉排序树的插入方式，小数插入在左节点，大数插入在右节点
{
	if (root->GetLeft() == NULL)//对于只有一个根节点的空树
	{
		location=new TreeNode<T>(item);//开辟内存新建节点
		location->SetParent(root);//设定父子关系
		root->SetLeft(location);
		return root->GetLeft();//返回插入位置
	}
	else
	{
		if (item < location->GetData())//小数插入左子节点
		{
			if (location->GetLeft() == NULL)//相当于递归终止条件
			{
				location->SetLeft(new TreeNode<T>(item));//设定父子关系
				location->GetLeft()->SetParent(location);
				return location->GetLeft();
			}
			else
				return BSTInsert(item,location->GetLeft());//若没找到插入位置，则继续往左分支方向考察
		}
		else if (item > location->GetData())//大数插入右子节点
		{
			if (location->GetRight() == NULL)//相当于递归终止条件
			{
				location->SetRight(new TreeNode<T>(item));//设定父子关系
				location->GetRight()->SetParent(location);
				return location->GetRight();
			}
			else
				return BSTInsert(item,location->GetRight());//若没找到插入位置，则继续往右分支方向考察
		}
		else
		{
			return location;
		}
	}
}
template <class T>
TreeNode<T>* BinaryTree<T>::FindTarget(T target, TreeNode<T>* start)//从start位置开始寻找数中第一个值为target的节点，并返回其位置
{
	TreeNode<T>* temp = start;
	if (temp != NULL)
	{
		while (temp != NULL)//由于排序二叉树中，数字的大小关系体现在树节点的左右分支关系上，因此，可以通过大小比较确定下一步搜索的方向，直到最终找到目标
		{
			if (temp->GetData() == target)
				return temp;
			else if (temp->GetData() < target)
				temp = temp->GetRight();
			else
				temp = temp->GetLeft();
		}
	}
	return NULL;//如果没有执行while循环或者while循环没找到的话，表明不存在目标，返回NULL
}