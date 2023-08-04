#pragma once
#include <iostream>
#include <string>
#include <direct.h>
#include <io.h>
using namespace std;
#define KID 0 //宏定义左子树方向为0
#define BROTHER 1 //宏定义右子树方向为1

struct FCB {
	string fileName;
	int index;
	int length;
	string address;

	FCB(string a, int b = -1, int c = 0,string d = "C:\\2052134") {
		fileName = a;
		index = b;
		length = c;
		address = d;
	}
};


class TreeNode//树节点类
{
private:
	FCB data;
	TreeNode* LeftNode, * RightNode, * Parent;
public:
	TreeNode(FCB item, TreeNode* parent = NULL, TreeNode* left = NULL, TreeNode* right = NULL) :data(item), Parent(parent), LeftNode(left), RightNode(right) {}
	string GetFCBFileName() { return data.fileName; }//返回节点本身的数据信息
	string GetFCBAddress() { return data.address; }//返回节点本身的数据信息
	int GetFCBIndex() { return data.index; }
	int GetFCBLength() { return data.length; }
	TreeNode* GetParent() { return Parent; }//获取当前节点的父节点
	TreeNode* GetLeft() { return LeftNode; }//获取当前节点的左子节点
	TreeNode* GetRight() { return RightNode; }//获取当前节点的右子节点
	void SetData(string a, int b, int c, string d) { data.fileName = a; data.index = b; data.length = c; data.address = d; }//设定当前节点的数据
	void SetDataIndex(int a) { data.index = a; }
	void SetDataLength(int a) { data.length = a; }
	void SetDataFileName(string a) { data.fileName = a; }
	void SetParent(TreeNode* parent) { Parent = parent; }//设定当前节点的父节点
	void SetLeft(TreeNode* left = NULL) { LeftNode = left; }//设定当前节点的左子节点
	void SetRight(TreeNode* right = NULL) { RightNode = right; }//设定当前节点的右子节点
};

class BinaryTree//二叉树类
{
private:
	TreeNode* root, * current;//根节点、当前节点
public:
	BinaryTree(FCB value)//构造函数
	{
		root = current = new TreeNode(value);
	}
	~BinaryTree() { destroy(root); }//析构函数
	TreeNode* GetRoot()//获取根节点
	{
		if (root == NULL)
			return NULL;
		return root;
	}
	TreeNode* Insert(FCB item, int direction, TreeNode* location);//在当前节点的左或右方向插入值为item的新子节点
	TreeNode* FindTarget(string name, TreeNode* start);//从start位置开始寻找值为item的第一个节点
	TreeNode* FindParent(TreeNode* node)//寻找node节点的伦理意义上的父母，注意与父节点的区别
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
	void ForwardTravel(TreeNode* start);//前序遍历二叉树
	void MiddleTravel(TreeNode* start);//中序遍历二叉树
	void BackwardTravel(TreeNode* start);//后续遍历二叉树
	TreeNode* GetParent(TreeNode* location)
	{
		if (location == NULL || location == root || location->GetParent() == NULL)//父节点不存在的情况
			return NULL;
		return location->GetParent();//返回父节点
	}
	int Brother(TreeNode* location);//输出location节点的所有伦理兄弟，此处的NOT_SHOW用于调控输出时不输出某些字符
	int Kid(TreeNode* location)//输出location节点的所有伦理子女
	{
		int count = 0;
		if (location == NULL || location->GetLeft() == NULL)//无子女的情况
		{
			//cout << "无子女" << endl;
			return 0;
		}
		else
		{
			/*cout << location->GetLeft()->GetFCBFileName()<< ' '
				<< location->GetLeft()->GetFCBIndex() << ' '
				<< location->GetLeft()->GetFCBLength() << ' '
				<< location->GetLeft()->GetFCBAddress() << endl;*/
			count = 1;
			if (location->GetLeft()->GetRight() != NULL) {//第一个子女节点的右方向兄弟节点才是其他子女所在的位置
				count += Brother(location->GetLeft());
			}
			return count;
		}
	}
	TreeNode* GetTargetSon(TreeNode* location, int num) {
		int count = 0;
		if (location == NULL || location->GetLeft() == NULL) {//无子女的情况
			return NULL;
		}
		else{
			count = 1;
			if (num == count) {
				return location->GetLeft();
			}
			else if (location->GetLeft()->GetRight() != NULL) {//第一个子女节点的右方向兄弟节点才是其他子女所在的位置
				location = location->GetLeft();
				while (location->GetRight() != NULL)//遍历该枝右子树
				{
					location = location->GetRight();
					++count;
					if (count == num) {
						return location;
					}
					continue;
				}
			}
			return NULL;
		}
	}
	void destroy(TreeNode* location);//用来删除location的所有后代
	void kill(TreeNode* location)//用来删除特定的叶节点或者孤立节点
	{
		if (location != root)
		{
			if (location->GetParent()->GetLeft() == location) {
				location->GetParent()->SetLeft(location->GetRight());//SetLeft无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL
				if(location->GetRight()!= NULL)
				    location->GetRight()->SetParent(location->GetParent());
			}
			else {
				location->GetParent()->SetRight(location->GetRight());
				if (location->GetRight() != NULL)
				    location->GetRight()->SetParent(location->GetParent());
			}
		}
		delete location;//释放该叶节点所占的空间
	}
};
void BinaryTree::destroy(TreeNode* location)//删除以location为根节点的树
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
		//if (location->GetParent()->GetLeft() == location) {
		//	location->GetParent()->SetLeft(location->GetRight());//SetLeft无参数即使用默认参数NULL，表明将其父节点的左子节点指向NULL
		//	location->GetRight()->SetParent(location->GetParent());
		//}
		//else {
		//	location->GetParent()->SetRight(location->GetRight());
		//	location->GetRight()->SetParent(location->GetParent());
		//}
		delete location;
	}
}
void BinaryTree::MiddleTravel(TreeNode* start)//中序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		MiddleTravel(start->GetLeft());
		cout << start->GetFCBFileName() << ' ' << start->GetFCBIndex() << ' ' << start->GetFCBLength() << ' ' << start->GetFCBAddress() << endl;
		MiddleTravel(start->GetRight());
	}
}
void BinaryTree::ForwardTravel(TreeNode* start)//前序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		cout << start->GetFCBFileName() << ' ' << start->GetFCBIndex() << ' ' << start->GetFCBLength() << ' ' << start->GetFCBAddress() << endl;
		ForwardTravel(start->GetLeft());
		ForwardTravel(start->GetRight());
	}
}
void BinaryTree::BackwardTravel(TreeNode* start)//后序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		BackwardTravel(start->GetLeft());
		BackwardTravel(start->GetRight());
		cout << start->GetFCBFileName() << ' ' << start->GetFCBIndex() << ' ' << start->GetFCBLength() << ' ' << start->GetFCBAddress() << endl;
	}
}
int BinaryTree::Brother(TreeNode* location)//输出location节点的所有伦理兄弟，即输出其所在的右子树一枝上的除该节点外所有的节点
{
	if (location == NULL || location->GetRight() == NULL)//无兄弟姐妹情况
	{
		return 0;
	}
	else
	{
		int count = 0;
		while (location->GetRight() != NULL)//遍历该枝右子树
		{
			location = location->GetRight();
			++count;
			continue;
		}
		return count;
	}
}
TreeNode* BinaryTree::Insert(FCB item, int direction, TreeNode* location)//在当前节点的左或右方向插入值为item的新子节点
{
	if (location == NULL)//非法插入位置
	{
		cout << "插入位置非法，插入失败" << endl;
		return NULL;
	}
	current = new TreeNode(item);
	if (direction == KID)           //插入子节点
	{
		TreeNode* temp = location->GetLeft();
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
		//current = temp;
		return current;
	}
	else if (direction == BROTHER)//插入兄弟节点
	{
		TreeNode* temp = location->GetRight();
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
TreeNode* BinaryTree::FindTarget(string target, TreeNode* start)//从start位置开始寻找数中第一个值为target的节点，并返回其位置
{
	if (start == NULL)//无效的搜索起始位置
		return NULL;
	else if (start->GetFCBAddress() == target)//递归结束条件
		return start;
	else     //递归思路：左子树没找到就继续到右子树找
	{
		TreeNode* temp = FindTarget(target, start->GetLeft());
		if (temp == NULL && start != NULL && (start->GetRight() != NULL))
			temp = FindTarget(target, start->GetRight());
		return temp;
	}
}