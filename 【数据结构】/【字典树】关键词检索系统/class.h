#pragma once
#include <iostream>
#include <string>
using namespace std;
#define NUM 26 //26个英文字母

class DataType//被DataNode类调用，每个字符包含三个信息：字符本身，该字符出现的次数，该字符作为结尾出现的次数
{
private:
	char letter;//字符本身
	int num,end;//字符出现的次数，该字符作为结尾出现的次数
public:
	DataType() { letter = NULL; num = 0; end = 0; }//构造函数
	DataType(char copy_letter,int copy_num=0,int copy_end=0):letter(copy_letter),num(copy_num),end(copy_end){}//构造函数
	char GetLetter() { return letter; }//返回字符本身
	int GetNum() { return num; }//返回字符出现的次数
	int GetEnd() { return end; }//返回字符作为结尾出现的次数，也就是该词精确出现的次数
	void AddNum() { num++; }//字符每出现一次，num+1
	void AddEnd() { end++; }//字符每作为结尾出现一次，end+1
};
class DataNode//节点类
{
private:
	DataType data;
	DataNode* link[NUM];//每一个字符之后出现的字符有26种情况（不区分大小写）
public:
	DataNode(DataType new_data)//构造函数
	{
		data = new_data;
		for (int i = 0; i < NUM; i++)
			link[i] = NULL;
	}
	DataType* GetData()//返回data的地址
	{
		return &data;
	}
	DataNode* GetNode(int i)//返回Node的地址
	{
		if (i < 0 || i >= NUM)
			return NULL;
		return link[i];
	}
	void InsertNode(DataType type)//插入节点
	{
		link[static_cast<int>(type.GetLetter() - 'a')]
			= new DataNode(type);
	}
	//说明：虽然此类中没有自定义析构函数用于delete新开辟的内存空间，但Tire类中的析构函数会执行这一操作
};
class Tire//字典树类
{
private:
	DataNode* root;//根节点
public:
	Tire(DataType init)//构造函数
	{
		root = new DataNode(init);
	}
	~Tire(){MakeEmpty(root);}//析构函数
	void InsertWord(string word);//在字典树中插入一个单词
	int Find(string key);//在字典树中不区分大小写精确查找一个单词出现的次数
	void MakeEmpty(DataNode* next);//被析构函数调用，用于释放开辟的内存空间
};
void Tire::InsertWord(string word)//在字典树中插入一个单词
{
	DataNode* current = root;                //从字典树的根节点开始
	unsigned int length = word.length();
	for (unsigned int i = 0; i < length; i++)//遍历单词中的每一个字母，若字典树中各层字母已存在，则num++即可，否则新建节点
	{
		DataType data(word[i]);
		if (current->GetNode(static_cast<int>(word[i] - 'a')) == NULL)
			current->InsertNode(data);
		current = current->GetNode(static_cast<int>(word[i] - 'a'));
		current->GetData()->AddNum();
		if (i == length - 1)
			current->GetData()->AddEnd();//若该字符作为结尾出现，则end++，这也是该词出现的次数
	}
}
int Tire::Find(string key)//在字典树中不区分大小写精确查找一个单词出现的次数
{
	DataNode* current = root;                //从字典树的根节点开始
	unsigned int length = key.length();
	for (unsigned int i = 0; i < length; i++)//遍历单词中的每一个字母
	{
		if (current->GetNode(static_cast<int>(key[i] - 'a')) == NULL)//若该单词的两个字母间的联系在字典树中不存在，则该单词在字典树中不存在，返回0即可
			return 0;
		current = current->GetNode(static_cast<int>(key[i] - 'a'));//移动到下一个字母节点
		if (i == length - 1)
			return current->GetData()->GetEnd();//如果到了单词词尾，则返回end值，即为该单词精确出现的次数
	}
	return 0;
}
void Tire::MakeEmpty(DataNode* next)//递归删除
{
	DataNode* current = next;
	for (int i = 0; i < NUM; i++)
	{
		if (current->GetNode(i) == NULL)
			continue;
		else
		{
			MakeEmpty(current->GetNode(i));//依照字典树各层递归检索
			if (i == NUM - 1)
				delete(current);//在所有后续字母都删除完成后删除该字母本身
		}
	}
}