#pragma once
#include <iostream>
using namespace std;
#define KID 0 //宏定义左子树方向为0
#define BROTHER 1 //宏定义右子树方向为1
template <class T>
class TreeNode//树节点类
{
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
{
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
	void RsetRoot(T value)//重设祖先
	{
		current = new TreeNode<T>(value);//新建节点
		current->SetParent(root);//为新建节点设定父节点为根节点
		root->SetLeft(current);//为原来的根节点设定子节点
	}
	TreeNode<T>* Insert(T item, int direction, TreeNode<T>* location);//在当前节点的左或右方向插入值为item的新子节点
	TreeNode<T>* FindTarget(T item, TreeNode<T>* start);//从start位置开始寻找值为item的第一个节点
	TreeNode<T>* FindParent(TreeNode<T>* node)//寻找node节点的伦理意义上的父母，注意与父节点的区别
	{
		if (node == NULL && current != NULL)//设定可能默认的参数
			node = current;
		if ((node == NULL && current == NULL) || node == root)//不存在父节点的情况
			return NULL;
		else
		{
			while (node->GetParent()->GetLeft() != node)//只要没找到就一直向上考察
			{
				if (node->GetParent() == NULL)//不存在父节点的情况
					return NULL;
				else
					node = node->GetParent();//没找到就继续向上寻找
			}
			current = node->GetParent();//注意循环结束条件
			return current;//返回父节点地址
		}
	}
	void ForwardTravel(TreeNode<T>* start,T NOT_SHOW_1=NULL, T NOT_SHOW_2 = NULL);//前序遍历二叉树
	void MiddleTravel(TreeNode<T>* start, T NOT_SHOW_1 = NULL, T NOT_SHOW_2 = NULL);//中序遍历二叉树
	void BackwardTravel(TreeNode<T>* start, T NOT_SHOW_1 = NULL, T NOT_SHOW_2 = NULL);//后续遍历二叉树
	TreeNode<T>* GetParent(TreeNode<T>* location)
	{
		if (location == NULL || location == root || location->Parent == NULL)//父节点不存在的情况
			return NULL;
		return location->Parent;//返回父节点
	}
	int Brother(TreeNode<T>* location, TreeNode<T>* NOT_SHOW = NULL);//输出location节点的所有伦理兄弟，此处的NOT_SHOW用于调控输出时不输出某些字符
	int Kid(TreeNode<T>* location, TreeNode<T>* NOT_SHOW = NULL)//输出location节点的所有伦理子女
	{
		if (location == NULL || location->GetLeft() == NULL)//无子女的情况
		{
			cout << "无子女" << endl;
			return 0;
		}
		else
		{
			if (location->GetLeft() != NOT_SHOW)//先指向左子节点输出第一个子女
				cout << location->GetLeft()->GetData() << ' ';
			if ((NOT_SHOW == NULL && location->GetLeft()->GetRight() != NULL) || (NOT_SHOW != NULL))//第一个子女节点的右方向兄弟节点才是其他子女所在的位置
				Brother(location->GetLeft(), NOT_SHOW);
			return 1;
		}
	}
	void destroy(TreeNode<T>* location);//用来删除location的所有后代
	void kill(TreeNode<T>* location)//用来删除特定的叶节点或者孤立节点
	{
		if (location != root)
		{
			if (location->GetParent()->GetLeft() == location)
				location->GetParent()->SetLeft();//SetLeft无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL
			else
				location->GetParent()->SetRight();
		}
		delete location;//释放该叶节点所占的空间
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
				location->GetParent()->SetLeft();//SetLeft无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL，下同
			else
				location->GetParent()->SetRight();
		}
		delete location;
	}
}
template <class T>
void BinaryTree<T>::MiddleTravel(TreeNode<T>* start, T NOT_SHOW_1, T NOT_SHOW_2)//中序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		T show = start->GetData();
		MiddleTravel(start->GetLeft(), NOT_SHOW_1, NOT_SHOW_2);
		if (show != NOT_SHOW_1 && show != NOT_SHOW_2)
			cout << show<<' ';
		MiddleTravel(start->GetRight(), NOT_SHOW_1, NOT_SHOW_2);
	}
}
template <class T>
void BinaryTree<T>::ForwardTravel(TreeNode<T>* start, T NOT_SHOW_1, T NOT_SHOW_2)//前序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		T show = start->GetData();
		if(show!= NOT_SHOW_1&& show != NOT_SHOW_2)
		    cout << show<<' ';
		ForwardTravel(start->GetLeft(), NOT_SHOW_1, NOT_SHOW_2);
		ForwardTravel(start->GetRight(), NOT_SHOW_1, NOT_SHOW_2);
	}
}
template <class T>
void BinaryTree<T>::BackwardTravel(TreeNode<T>* start, T NOT_SHOW_1, T NOT_SHOW_2)//后序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		T show = start->GetData();
		BackwardTravel(start->GetLeft(), NOT_SHOW_1, NOT_SHOW_2);
		BackwardTravel(start->GetRight(), NOT_SHOW_1, NOT_SHOW_2);
		if (show != NOT_SHOW_1 && show != NOT_SHOW_2)
			cout << show<<' ';
	}
}
template <class T>
int BinaryTree<T>::Brother(TreeNode<T>* location, TreeNode<T>* NOT_SHOW)//输出location节点的所有伦理兄弟，即输出其所在的右子树一枝上的除该节点外所有的节点
{
	if (location == NULL || location->GetRight() == NULL)//无兄弟姐妹情况
	{
		cout << "无兄弟姐妹" << endl;
		return 0;
	}
	else
	{
		while (location->GetRight() != NULL)//遍历该枝右子树
		{
			if (location->GetRight() != NOT_SHOW)
				cout << location->GetRight()->GetData() << ' ';
			location = location->GetRight();
			continue;
		}
		return 1;
	}
}
template <class T>
TreeNode<T>* BinaryTree<T>::Insert(T item, int direction, TreeNode<T>* location)//在当前节点的左或右方向插入值为item的新子节点
{
	if (location == NULL)//非法插入位置
	{
		cout << "插入位置非法，插入失败" << endl;
		return NULL;
	}
	current = new TreeNode<T>(item);
	if (direction == KID)           //插入子节点
	{
		TreeNode<T>* temp = location->GetLeft();
		if (temp == NULL)//以前没有子节点则直接设定left指针
		{
			location->SetLeft(current);
			current->SetParent(location);
		}
		else//以前有子节点则将新子节点插入原来子女的其他兄弟之后
		{
			while (temp->GetRight() != NULL)
				temp = temp->GetRight();
			temp->SetRight(current);
			current->SetParent(temp);
		}
		current = temp;
		return current;
	}
	else if (direction == BROTHER)//插入兄弟节点
	{
		TreeNode<T>* temp = location->GetRight();
		if (temp == NULL)//以前没有兄弟节点则直接设定right指针
		{
			location->SetRight(current);
			current->SetParent(location);
		}
		else//以前有兄弟节点则将新节点插入到其他兄弟之后
		{
			while (temp->GetRight() != NULL)
				temp = temp->GetRight();
			temp->SetRight(current);
			current->SetParent(temp);
		}
		return current;
	}
	else//处理错误的非法参数，如已经宏定义插入子节点传入参数0，兄弟节点传入参数1，但传入了其他的数
	{
		cout << "未定义的插入节点属性" << endl;
		return NULL;
	}
}
template <class T>
TreeNode<T>* BinaryTree<T>::FindTarget(T target, TreeNode<T>* start)//从start位置开始寻找数中第一个值为target的节点，并返回其位置
{
	if (start == NULL)//无效的搜索起始位置
		return NULL;
	else if (start->GetData() == target)//递归结束条件
		return start;
	else     //递归思路：左子树没找到就继续到右子树找
	{
		TreeNode<T>* temp = FindTarget(target, start->GetLeft());
		if (temp == NULL && start != NULL && (start->GetRight() != NULL))
			temp = FindTarget(target, start->GetRight());
		return temp;
	}
}