#pragma once
#define BIG_NUM 2147483647//即使是动态内存开辟，也是有一定限制的，且十亿量级足够实际使用
#define FULL_LIST "目前列表考生人数已达到程序上限，插入操作无效" //为确保程序稳定，设定了链表长度上限，但一般实际中不会触发此上限

//结合实际情况考量非法的无效操作，并对其进行处理时输出的提示语，宏定义以使main函数简洁美观，便于复用提示语
#define ERROR_NUM "您输入的上一位考生考号中含有非数字字符，不合规范，请重新输入上一位考生的信息: "
#define ERROR_NUM_ZERO "考生考号不可设置为单字符0，请重新输入上一位考生的信息: "
#define ERROR_CHOICE "无效选择！"
#define ERROR_NAME_1 "您输入的上一位考生姓名不符合基本规范，请重新输入上一位考生的信息: "
#define ERROR_NAME_2 "您输入的上一位考生姓名只有一个汉字，存在漏输汉字，请重新输入上一位考生的信息: "
#define ERROR_SEX "您输入的上一位考生性别不符合基本规范，请重新输入上一位考生的信息: "
#define ERROR_AGE_1 "您输入的上一位考生年龄中含有非数字字符，不合规范，请重新输入上一位考生的年龄: "
#define ERROR_AGE_2 "您输入的上一位考生年龄低于10岁或高于99岁，不合常理，请重新输入上一位考生的信息: "
#define ERROR_MAJOR "您输入的上一位考生报考类别含有非中文字符，不符合基本规范，请重新输入上一位考生的信息: "
#define ERROR_INPUT "您输入的插入位置为非正数或超出链表长度加一，为无效插入位置，请重新输入: "
#define ERROR_FIND "查无此人，考生列表中不存在该考号考生!"
#define ERROR_SAMENUM "考号重复！重复的考号无法有效区分学生,请重新输入该考生的全部信息: "
#define ERROR_REWRITE "请重新选择正确的修改选择: "
#define ERROR_NUM_R "修改后的考号不得与原考号相同或与已有考号重复,请重新输入: "
#define ERROR_NAME_R "修改后的姓名不得与原姓名相同,请重新输入修改后的姓名: "
#define ERROR_SEX_R "修改后的性别不得与原性别相同,请重新输入修改后的性别: "
#define ERROR_AGE_R "修改后的年龄不得与原年龄相同,请重新输入修改后的年龄: "
#define ERROR_MAJOR_R "修改后的报考类别不得与原报考类别相同,请重新输入修改后的报考类别: "

//输出的提示语，利用宏定义以使main函数简洁美观，便于复用提示语
#define START_WORD_1 "首先请建立考生信息系统!"
#define START_WORD_2 "请输入考生人数: "
#define START_WORD_3 "输入的人数无效，请重新输入: "
#define START_WORD_4 "输入的人数太多，超出程序上限，请重新输入: "
#define START_WORD_5 "请依次输入考生的考号，姓名，性别，年龄及报考类别: "
#define START_WORD_6 "若全名太长则请使用不超过6个字的缩写名即可，如：艾合买提"
#define ACTION_WORD "请选择您要进行的操作(1为插入,2为删除,3为查找,4为修改,5为统计,0为取消当前操作,finish为退出程序): "
#define INSERT_WORD_1 "请输入您要插入的考生的位置(合法范围为1~当前考生总人数+1)，输入0则表示取消当前插入操作: "
#define INSERT_WORD_2 "请依次输入要插入的考生的考号，姓名，性别，年龄及报考类别: "
#define DELETE_WORD_1 "请输入要删除的考生的考号，输入0则表示取消当前删除操作: "
#define DELETE_WORD_2 "您删除的考生信息是: "
#define FIND_WORD "请输入要操作的考生的考号: "
#define REWRITE_CHOICE "请选择要修改的内容(1为考号,2为姓名,3为性别,4为年龄,5为报考科目,6为重新全部修改该考号考生信息,0为取消当前修改操作): "
#define REWRITE_NUM "请输入修改后的考号: "
#define REWRITE_NAME "请输入修改后的姓名: "
#define REWRITE_SEX "请输入修改后的性别: "
#define REWRITE_AGE "请输入修改后的年龄: "
#define REWRITE_MAJOR "请输入修改后的报考科目: "
#define REWRITE_END_1 "修改后的该考生信息为: "
#define REWRITE_END_2 "修改后的考生信息列表为: "
#define STATISTIC "统计得到的考生信息是: "
#define AGE_LIST "各年龄人数分布（按人数从多到少排序）："
#define MAJOR_LIST "各报考科目人数分布（按人数从多到少排序）："
#define NOTHING " 空表 "


template <class T1, class T2>
class Data
{
private:
	T1 num,age,flag_age,flag_major;
	T2 name, sex, major;
public:
	void set_num(T1 a)//设置考生考号
	{
		num = a;
	} 
	void set_age(T1 a)//设置考生年龄
	{
		age = a;
	}
	void set_flag_age(T1 a)//标记考生年龄,便于统计
	{
		flag_age = a;
	}
	void set_flag_major(T1 a)//标记考生专业,便于统计
	{
		flag_major = a;
	}
	void set_name(T2 a)//设置考生姓名
	{
		name = a;
	}
	void set_sex(T2 a)//设置考生性别
	{
		sex = a;
	}
	void set_major(T2 a)//设置考生专业
	{
		major = a;
	}
	T1 get_num()//获取考生考号
	{
		return num;
	}
	T2 get_name()//获取考生姓名
	{
		return name;
	}
	T2 get_sex()//获取考生性别
	{
		return sex;
	}
	T1 get_age()//获取考生年龄
	{
		return age;
	}
	T2 get_major()//获取考生专业
	{
		return major;
	}
	T1 get_flag_age()//获取考生年龄标记
	{
		return flag_age;
	}
	T1 get_flag_major()//获取考生专业标记
	{
		return flag_major;
	}
};
template <class T, class T1, class T2> 
class List;
template <class T, class T1, class T2> 
class ListNode
{
private:
	Data<T1, T2> data;
	ListNode<T,T1,T2>* link;
public:
	ListNode();
	ListNode(const T &item);
	void InsertAfter(ListNode<T, T1, T2> *p);//在当前节点后插入新节点
	ListNode<T, T1, T2>* RemoveAfter();//移除当前节点后面一个节点并返回移除节点的地址
	friend class List<T, T1, T2>;
};
template <class T, class T1, class T2> 
class List
{
private:
	ListNode<T, T1, T2>* first, * last;
public:
	List(const T& value, const T &next_value) //构造函数，实例化类时建立头结点并讲头结点与后面第一个节点连接
	{ 
		last = first = new ListNode<T, T1, T2>(value); 
		last=first->link = new ListNode<T, T1, T2>(next_value);//动态开辟内存存储新节点，在实例化类时即将头结点与第一个节点连接
	}
	~List();
	void MakeEmpty();//删除链表时使用，被析构函数调用
	long long Length() const;//统计链表长度
	int Find(T1 value); //返回值为value的链表节点的位置
	ListNode<T, T1, T2>* Find(int i); //返回链表第i个位置的节点的地址
	ListNode<T, T1, T2>* GetNode(const T& item, ListNode<T, T1, T2>* next = NULL);//建立新节点
	int Insert(T value, int i);//在位置i插入值为value的链表节点
	int Same(T1 value);  //寻找链表中是否有value值一样的节点，如：防止考生考号重复或统计同名者等功能
	int Remove(int i); //移除链表中位置为i的节点
	T* Get(int i);//获取链表i位置的节点值
};

template <class T, class T1, class T2>
ListNode<T, T1, T2>::ListNode() :link(NULL) {}
template <class T, class T1, class T2>
ListNode<T, T1, T2>::ListNode(const T& item) : data(item), link(NULL) {}
template <class T, class T1, class T2>
void ListNode<T, T1, T2>::InsertAfter(ListNode<T, T1, T2>* p)//在当前节点后插入新节点
{
	p->link = link;
	link = p;
}
template <class T, class T1, class T2>
ListNode<T, T1, T2>*  List<T, T1, T2>::GetNode(const T& item, ListNode<T, T1, T2>* next)//建立新节点
{
	ListNode<T, T1, T2>* newnode = new ListNode<T, T1, T2>(item);
	newnode->link = next;   //建立新节点并将新节点指针指向下一个节点
	return newnode;
}
template <class T, class T1, class T2>
ListNode<T, T1, T2>* ListNode<T, T1, T2>::RemoveAfter()//建立新节点
{
	ListNode<T, T1, T2>* temp = link;
	if (link == NULL)
		return NULL;
	link = temp->link;
	return temp;
}
template <class T, class T1, class T2>
List<T, T1, T2>::~List()
{
	MakeEmpty();
	delete first;//由于MakeEmpty函数并不能删除头结点，因此需要额外执行删除头结点的操作
}
template <class T, class T1, class T2>
void List<T, T1, T2>::MakeEmpty()
{
	ListNode<T, T1, T2>* q;
	while (first->link != NULL)//只要链表中存在除头结点以外的其他节点，就一直循环执行删除操作
	{
		q = first->link;//指向头结点的下一个节点，并用q暂存其地址
		first->link = q->link;//令头结点指向下下个节点，与紧挨头结点的下一个节点脱钩，以便于对其执行删除操作
		delete q;//删除q指向的节点
	}
	last = first;//循环执行完毕后，链表中只余下头结点，将last指针与first指针对齐，以便于删除
}
template <class T, class T1, class T2>
long long List<T, T1, T2>::Length() const //统计链表长度
{
	ListNode<T, T1, T2>* p = first->link;//从头结点指向的下一个节点开始计数
	long long count = 0;
	while (p != NULL)//只要节点不空，即未到链表末尾，则一直循环计数，并将指针指向当前节点的下一个节点
	{
		p = p->link;
		count++;
	}
	return count;//返回节点数量，即链表长度
}
template <class T, class T1, class T2>
int List<T, T1, T2>::Same(T1 value) //寻找链表中是否有value值一样的节点，如：防止考生考号重复或统计同名者等功能
{
	ListNode<T, T1, T2>* p = first->link;
	int count = 0;
	while (p != NULL)//在链表范围内进行查找同值节点
	{
		if(p->data.get_num() == value)
		    count++;//若有同值节点，则计数
		p = p->link;
	}
	return count;//返回同值节点数量
}
template <class T, class T1, class T2>
int List<T, T1, T2>::Find(T1 value)//返回值为value的链表节点的位置
{
	ListNode<T, T1, T2>* p = first->link;//从头结点指向的下一个节点开始
	int count = 0;
	while (p != NULL && p->data.get_num() != value)//在链表长度范围内查找，如果找到即可以跳出循环，返回计数值
	{
		p = p->link;
		count++;
	}
	if (p == NULL)
		count = -1;//如果p==NULL,那说明在链表中没找到拥有该值的节点，返回-1，表示未找到
	return count;
}
template <class T, class T1, class T2>
ListNode<T, T1, T2>* List<T, T1, T2>::Find(int i)//返回链表第i个位置的节点的地址
{
	if (i < -1)//非法的无效输入处理
		return NULL;
	if (i == -1)//头结点在链表中的位置用-1标识
		return first;
	ListNode<T, T1, T2>* p = first->link;
	int j = 0;
	while (p != NULL && j < i)//在链表长度范围内一直循环第i个位置或最后一个节点
	{
		p = p->link;
		j = j++;
	}
	return p;//如果第i个位置在链表长度范围内，返回的是节点地址，如i已经超出了链表长度，此时p的值恰好是NULL，返回NULL表示该位置不存在有效节点
}
template <class T, class T1, class T2>
int List<T, T1, T2>::Insert(T value, int i)//在位置i插入值为value的链表节点
{
	ListNode<T, T1, T2>* p = Find(i - 1);//在第i个位置插入节点当然需要定位i-1位置的节点，以使其指针指向插入的新节点
	if (p == NULL)//对非法无效位置的处理
		return 0;//返回0表示插入失败
	ListNode<T, T1, T2>* newnode=GetNode(value, p->link);//先动态开辟内存存储新建立的节点，并让新建立的节点指向原来在i位置的节点
	if (p->link == NULL)
		last = newnode;//如果p已经是尾结点的话，那么新插入的节点成为新的尾结点
	p->link = newnode;//原来i-1位置的节点指针指向新建节点，此时，新建节点成为第i位置的节点，原来i位置及后面的节点位置随着这一步整体自然后移1位
	return 1;//返回1表示插入成功
}
template <class T, class T1, class T2>
int List<T, T1, T2>::Remove(int i)//移除链表中位置为i的节点
{
	ListNode<T, T1, T2>* q, * p = Find(i - 1);//在第i个位置插入节点当然需要定位i-1位置的节点，以更改其指针指向
	if (p == NULL || p->link == NULL)
		return 0;//如果第i位置本身不存在节点，自然就不需要额外的删除操作
	q = p->link;
	p->link = q->link;//i-1位置的指针指向下下个节点，跳过要删除的i位置节点
	if (q == last)
		last = p;//如果要删除的节点是尾节点的话，尾结点指向应前移一位
	delete q;//删除i位置节点
	return 1;//返回1表示删除成功
}
template <class T, class T1, class T2>
T* List<T, T1, T2>::Get(int i)//获取链表i位置的节点值
{
	ListNode<T, T1, T2>* p = Find(i);//先找到i位置的节点
	if (p == NULL || p == first)//对于非法位置的节点的处理
		return NULL;
	else
		return &p->data;//返回i位置节点的值
}

























































// author@2052134 刘治华