#include <iostream>
#include <fstream>//此头文件仅仅用于读写文件，与数据结构无关
#include <string>
#include "class.h"
using namespace std;

string stopwords="\\/:*?\"<>| ";//用于文件名命名规范判断，依据windows命名规范，若出现stopwords中的字符，则命名不规范

void Wash(string& Key_Wash)//对输入的关键词进行规范化清洗
{
	int len = Key_Wash.length();
	for (int i = 0; i < len; i++)
	{
		if (Key_Wash[i] >= 'A' && Key_Wash[i] <= 'Z')//大写字母转小写字母，因为程序设定为大小写不敏感
			Key_Wash[i] = static_cast<char>(Key_Wash[i]+32);
	}
	for (unsigned int i = 0; i < Key_Wash.length();)//把关键词中的非英文字符去掉，与原来对照，就可以判断原先输入的关键词是否是规范的
	{
		if (Key_Wash[i] < 'a' || Key_Wash[i] >'z')
		{
			Key_Wash.erase(i, 1);//去掉非法关键词
			continue;
		}
		else
		{
			i++;
			continue;
		}
	}
}

int main()
{
	string file_name,tail,text,key,copy_key,word;//tail表示用户输入的文件后缀,text用于暂存控制台中的字符写到文件里，word的作用与text相反
	int length=stopwords.length();

	DataType root_type('*');
	Tire word_tree(root_type);//以符号 * 初始化字典树的根节点

	cout << "欢迎进入star的关键字检索系统!" << endl;
	cout << "请输入要创建的txt文件名("<<"根据Windows命名规则,您的文件名请勿出现空格以及"<<stopwords<<"这些字符) ";

	while (1)//对用户输入的文件名是否规范进行判断
	{
		cin >> file_name;
		for (int i=0; i < length; i++)//如果用户输入的文件名中出现了stopwords中的字符，则命名不符合windows命名规范，需要重新输入
		{
			if (file_name.find(stopwords[i]) >= 0 && file_name.find(stopwords[i]) < file_name.length())
			{
				file_name ="*";
				break;
			}
		}
		if ((file_name != "*")&&(file_name.find('.') > 0 && file_name.find('.') < file_name.length()))
		{
			tail = file_name.substr(file_name.find('.'));//获取用户输入的文件名的后缀，判断是否是 .txt
			if(tail==".txt")
			    break;
			else
			{
				cout << endl << "文件命名非法，请重新输入: ";//后缀名不是 .txt,未按照要求输入文件命名
				continue;
			}
		}
		else
		{
			cout << endl << "文件命名非法，请重新输入: ";//用户输入的文件名称不规范，重新输入
			continue;
		}
	}

	cout << endl << "请输入一段英文(输入回车表示换行，输入*表示结束输入)" << endl;
	cout << "! 请严格按照日常英文写作标点格式规范输入：标点之后空一格再输入其他单词 !" << endl;

	ofstream write(file_name);//建立文件
	if (write.fail())
	{
		cout << "文件创建失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else
	{
		int _=getchar();//吃掉上一个回车，否则会影响本程序的后续运行
		while (1)
		{
			getline(cin,text);//一行一行读入
			//*********************************************************************************************************************
			//*********************************************************************************************************************
			//这里需要说明：若测试时讲word文档里的文章直接copy到控制台中，
			//word文档中的空行实际上是 \n ，此时getline读入的text字符串长度为0，需要进行忽略处理，否则会影响后续字典树的建立
			//此处的处理不会造成其他影响
			//*********************************************************************************************************************
			//*********************************************************************************************************************
			if (text.length() == 0)//如果读入了空行，那么就继续读下一行
				continue;
			if (text[text.length() - 1] == '*')//根据字符 * 判断是不是最后一行
			{
				text.erase(text.length() - 1,1);//存入文件的内容中不应该有最后的字符 *
				write << text;//将text写入文档
				write.close();//及时关闭文档
				break;
			}
			else
			    write << text<<' ';//将text写入文档，特别注意此处空格的作用：同理，这里是对于将word文档中的段落直接复制到控制台测试代码时
			                       //word文档中的分段在控制台中会直接将后一个单词与前面一个段落的标点符号连在一起，造成字典树误判，因此需要额外
			                       //加一个空格隔开。这一操作不会产生其他影响。
		}
	}
	cout << endl << "本段文字已保存在文本文件 " << file_name << " 中。" << endl;
	cout << endl << "请输入要检索的不带空格的关键字(检索不区分大小写) " << endl;
	while (1)
	{
		cin >> key;
		copy_key = key;
		Wash(copy_key);//对输入的关键词进行规范化清洗，使之与字典树中的字符格式一致
		if (copy_key.length() != key.length())//如果规范化清洗之后的关键词与原来输入的关键词不一样，那么说明原先输入的关键词不规范，需要重输
		{
			string bin;
			getline(cin, bin);//吃掉非法输入后面可能跟着的其他非法输入
			cout << endl << "只可以输入不带空格的纯英文字符串，请重新输入：";
			continue;
		}
		else
			break;
	}
	cout << endl << "显示原文件 " << file_name << endl;

	ifstream read(file_name);//读文件
	if (read.fail())
	{
		cout << "文件打开失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else
	{
		while (1)
		{
			read >> word;//读文件
			cout << word << ' ';//输出内容到控制台

			Wash(word);//清洗word以使其符合建立字典树的格式规范
			word_tree.InsertWord(word);//在字典树中插入该单词

			if (read.eof())//判断是否读到文档末尾
			{
				read.close();//及时关闭文件
				break;
			}
		}
		cout << endl;
	}
	//输出结果
	cout << endl << "在源文件中共检索到(大小写不敏感时的完全匹配): " << word_tree.Find(copy_key) << " 个关键字 " << key<< endl;

	//******************************************************************************************************
    //******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	_ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************

	return 0;
}