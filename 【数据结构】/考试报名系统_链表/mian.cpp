#include <iostream>
#include <iomanip>//此头文件仅仅用于简化输出格式控制，与数据结构无关
#include <string>
#include "class.h"
using namespace std;

//此函数对输入的数字进行处理，对非法的无效输入进行处理(即提示用户重新输入)
long long input_num()
{
	long long number;
	cout << START_WORD_1 << endl << START_WORD_2;
	while (1)
	{
		cin >> number;
		if (cin.fail() || number <= 0 || number > BIG_NUM)//输入非法非数字字符或输入负数或数字超上限，则重新输入
		{
			cin.clear();
			cin.ignore();
			if (number > BIG_NUM)
				cout << START_WORD_4;
			else
				cout << START_WORD_3;
			continue;
		}
		else
			break;
	}
	return number;
}

//此函数对不同选项下输入的信息进行判断是否为非法输入并处理
Data<string, string> input_data(long long& space_num,long long &space_name,string choice)
{

	string onum="\0", oname = "\0", osex = "\0", oage = "\0", omajor = "\0",bin;
	Data<string, string> stu;
	int hash[256] = { 0 };
	while (1)
	{
		int count_num = 0, count_chinese = 0, count_english = 0;
		for (int j = 0; j < 256; j++)
			hash[j] = 0;
		if (choice == "6")//执行6操作时需要输入的信息
		{
			cin >> onum >> oname >> osex >> oage >> omajor;
			getline(cin, bin);//此语句用于忽略用可能在输完应该输入的全部信息后又意外多输入了其他数据，以免多余的输入影响后续操作，下同
		}
		else if (choice == "1")//执行1操作时需要输入考号
		{
			cin >> onum;
			getline(cin, bin);//此语句用于忽略用可能在输完应该输入的全部信息后又意外多输入了其他数据，以免多余的输入影响后续操作，下同
		}
		else if (choice == "2")//执行2操作时需要输入姓名
		{
			cin >> oname;
			getline(cin, bin);//此语句用于忽略用可能在输完应该输入的全部信息后又意外多输入了其他数据，以免多余的输入影响后续操作，下同
		}
		else if (choice == "3")//执行3操作时需要输入性别
		{
			cin >> osex;
			getline(cin, bin);
		}
		else if (choice == "4")//执行4操作时需要输入年龄
		{
			cin >> oage;
			getline(cin, bin);
		}
		else if (choice == "5")//执行5操作时需要输入报考类别专业
		{
			cin >> omajor;
			getline(cin, bin);
		}
		string copyname = oname, copymajor = omajor;
		if (choice == "6" || choice == "1")
		{
			if (onum == "0")
			{
				cout << ERROR_NUM_ZERO << endl;//防止将考号设置为0，与0表示的取消操作产生二义性
				continue;
			}
			for (unsigned int j = 0; j < onum.length(); j++)
				hash[static_cast<unsigned char>(onum[j])]++;
			for (int j = 48; j <= 57; j++)
				count_num += hash[j];//利用输入字符的ASCII码统计数字的个数，从而判断考号中是否出现了除数字以外的非法无效字符
			if (count_num != onum.length())
			{
				cout << ERROR_NUM << endl;
				continue;//若输入的考号不合规范则重新输入
			}
			else
			{
				for (unsigned int j = 0; j < 256; j++)
					hash[j] = 0;
				count_num = 0;
				stu.set_num(onum);//判断考号合规以后才设定考生考号
				if (choice == "1")
					break;
			}
		}
		if (choice == "6" || choice == "2")
		{
			for (unsigned int j = 0; j < copyname.length(); j++)//此处对输入的姓名中汉字的数量进行统计，用于判断是否存在非法字符
			{
				hash[static_cast<unsigned char>(copyname[j])]++;
				if (j >= 1)
				{//一个汉字相当于连续的两个char字符，且ASCII码占位在129-255位
					if (static_cast<unsigned char>(copyname[j]) > 128 && static_cast<unsigned char>(copyname[j - 1]) > 128)
					{
						count_chinese++;
						copyname[j] = '\0';
						copyname[j - 1] = '\0';
					}
				}
			}
			for (int j = 48; j <= 57; j++)
				count_num += hash[j];
			for (int j = 65; j <= 90; j++)
				count_english += hash[j];
			for (int j = 97; j <= 122; j++)
				count_english += hash[j];//三个for循环同理，利用ASCII码值统计输入的姓名是否存在无效的非法输入
			if (count_english + count_chinese == 0 || count_num + count_english + count_chinese * 2 != copyname.length() || count_english * count_chinese != 0 || count_num * count_chinese != 0)
			{
				cout << ERROR_NAME_1 << endl;
				continue;//若输入的姓名不合规范则重新输入
			}
			else if (count_english + count_num == 0 && count_chinese == 1)
			{
				cout << ERROR_NAME_2 << endl;
				continue;//若输入的姓名不合规范则重新输入
			}
			else
			{
				for (int j = 0; j < 256; j++)
					hash[j] = 0;
				count_num = 0;
				count_chinese = 0;
				count_english = 0;
				stu.set_name(oname);//输入规范的姓名才会设定考生姓名
				if (choice == "2")
					break;
			}
		}
		if (choice == "6" || choice == "3")
		{
			if (osex != "男" && osex != "女")
			{
				cout << ERROR_SEX << endl;//性别只有男与女，否则输入了非法的无效性别，重新输入
				continue;
			}
			else
			{
				stu.set_sex(osex);//输入合规的性别才会对考生的性别进行设定
				if (choice == "3")
					break;
			}
		}
		if (choice == "6" || choice == "4")
		{
			for (unsigned int j = 0; j < oage.length(); j++)
				hash[static_cast<unsigned char>(oage[j])]++;
			for (int j = 48; j <= 57; j++)//同理，对输入的年龄数据中是否存在除数字以外的非法无效数字进行判断
				count_num += hash[j];
			if (count_num != oage.length())
			{
				cout << ERROR_AGE_1 << endl;
				continue;//若输入的年龄非法则重新输入，下同
			}
			else if(oage.length() != 2)
			{
				cout << ERROR_AGE_2 << endl;
				continue;
			}
			else
			{
				for (int j = 0; j < 256; j++)
					hash[j] = 0;
				count_num = 0;
				stu.set_age(oage);//只有输入的年龄合规，才会对考生年龄进行设定
				if (choice == "4")
					break;
			}
		}
		if (choice == "6" || choice == "5")
		{
			for (unsigned int j = 0; j < copymajor.length(); j++)//同理，对考生输入的报考类别进行判断是否存在非法字符
			{
				hash[static_cast<unsigned char>(copymajor[j])]++;//此处是对输入的报考类别中汉字数量的统计，本程序只允许报考类别中完全采用中文表示
				if (j >= 1)
				{
					if (static_cast<unsigned char>(copymajor[j]) > 128 && static_cast<unsigned char>(copymajor[j - 1]) > 128)
					{
						count_chinese++;
						copymajor[j] = '\0';
						copymajor[j - 1] = '\0';
					}
				}
			}
			if (2 * count_chinese != copymajor.length())
			{
				cout << ERROR_MAJOR << endl;//若输入的报考类别不合规范，则重新输入
				continue;
			}
			else
			{
				for (int j = 0; j < 256; j++)
					hash[j] = 0;
				count_chinese = 0;
				stu.set_major(omajor);//只有当输入的报考类别符合规范时，才会对考生的报考类别进行设定
				break;
			}
		}
	}
	if (choice == "6" || choice == "1")//根据最长的考号位数与姓名位数动态调整输出之间的空格数量，以保证输出信息完整美观
	    space_num= (((onum.length()+static_cast<long long>(3)) > space_num) ? (onum.length()+ static_cast<long long>(3)) : space_num);
	if (choice == "6" || choice == "2")
	    space_name = (((oname.length()+ static_cast<long long>(3)) > space_name) ? (oname.length()+ static_cast<long long>(3)) : space_name);
	return stu;
}


int main()
{
	long long number,konge_num = 7,konge_name=7;
	Data<string, string> zero,copy;//zero表示头节点
	zero.set_num("\0");
	zero.set_name("\0");
	zero.set_sex("\0");
	zero.set_age("\0");
	zero.set_major("\0");
	zero.set_flag_age("\0");
	zero.set_flag_major("\0");
	number=input_num();
	if (number)
	{
		cout << START_WORD_5 << endl;//程序开始提示语
		//用头节点与输入的第一个考生信息实例化链表类
		List<Data<string, string>, string, string> list(zero, input_data(konge_num,konge_name,"6"));
		for (int i = 1; i < number;)//建立number个数的考生节点
		{
			zero = input_data(konge_num, konge_name,"6");
			if (list.Same(zero.get_num()) >= 1)
			{
				cout << ERROR_SAMENUM << endl;//如果考号重复，则重新输入，考号作为标识考生唯一性的键码，不允许重复
				continue;
			}
			else
			{
				list.Insert(zero, i);//插入考生节点
				i++;
			}
		}
		//输出建立的考生信息，其中各项信息间的空格数量依靠变量值动态控制，以使输出信息完整美观
		cout << endl << setiosflags(ios::left) <<setw(to_string(list.Length()).length() + static_cast<long long>(5))<<"位置" 
			<< setw(konge_num) << "考号"<< setw(konge_name) << "姓名" << setw(7) << "性别"<< setw(7) << "年龄" << "报考类别" << endl;
	    number = list.Length();
		for (int i = 0; i < number; i++)
		{
			cout << setw(to_string(list.Length()).length() + static_cast<long long>(5)) << i + 1 << setw(konge_num) << list.Get(i)->get_num()
				<< setw(konge_name)<< list.Get(i)->get_name() << setw(7) << list.Get(i)->get_sex()
				<< setw(7) << list.Get(i)->get_age() << list.Get(i)->get_major() << endl;
		}
		string choice="\0";
		int success = 1;
		//初次建立完考生信息后继续执行相应操作
		while (1)
		{
			cout <<endl<< ACTION_WORD ;
			cin >> choice;
			if (choice == "0")
				continue;
			else if (choice == "1")//插入功能
			{
				if (list.Length() >= BIG_NUM)//达到链表长度上限（21亿多）则不支持插入功能，以确保程序稳定运行，实际情况一般不会触及此上限
				{
					cout << FULL_LIST << endl;
					continue;
				}
				int location = 0;
				cout << endl << INSERT_WORD_1;
				while (1)
				{
					cin >> location;//输入要插入的考生位置
					if (location == 0)
						break;
					else if (location<0 || location>number + 1)
					{
						cout << ERROR_INPUT;//位置不合规则重新输入
						continue;
					}
					else
					{
						cout << endl << INSERT_WORD_2 << endl;
						while (1)
						{
							zero = input_data(konge_num, konge_name, "6");
							if (list.Same(zero.get_num()) >= 1)
							{
								cout << endl << ERROR_SAMENUM;//考号重复则重新输入
								continue;
							}
							else
								break;
						}
						success = list.Insert(zero, location - 1);//成功插入考生信息
						if (!success)
						{
							cout << "Fail insert because of something unexpected." << endl;
							break;
						}
						else
						{//输出当前考生列表
							cout << endl << setw(to_string(list.Length()).length() + static_cast<long long>(5)) <<"位置" 
								<< setw(konge_num) << "考号"<< setw(konge_name) << "姓名" << setw(7) << "性别"
								<< setw(7) << "年龄" << "报考类别" << endl;
							number = list.Length();
							for (int i = 0; i < number; i++)
							{
								cout << setw(to_string(list.Length()).length()+ static_cast<long long>(5)) << i + 1 
									<< setw(konge_num) << list.Get(i)->get_num()
									<< setw(konge_name) << list.Get(i)->get_name() << setw(7) << list.Get(i)->get_sex()
									<< setw(7) << list.Get(i)->get_age() << list.Get(i)->get_major() << endl;
							}
							break;
						}
					}
				}
				if (location == 0)
					continue;//此时的0其实在表示取消当前插入操作
			}
			else if (choice == "2")//删除功能
			{
				string kaohao;
				int location;
				cout << endl << DELETE_WORD_1;
				while (1)
				{
					cin >> kaohao;//输入要删除的考生考号
					if (kaohao == "0")//此时的0其实表示取消当前删除操作
						break;
					location=list.Find(kaohao);//先查列表，是否存在输入的考号与相应考生
					if (location == -1)
					{
						cout << endl << ERROR_FIND;//列表中不存在该考号考生，无法执行删除操作
						break;
					}
					else
					{//输出要删除的考生信息
						cout << endl << DELETE_WORD_2<< list.Get(location)->get_num()<<"   " << list.Get(location)->get_name() << "   "
							<< list.Get(location)->get_sex() << "   " << list.Get(location)->get_age() << "   "
							<< list.Get(location)->get_major() << endl;
						success=list.Remove(location);//删除该考生节点
						if (!success)
						{
							cout << "Fail remove because of something unexpected." << endl;
							break;
						}
						else
						{//输出删除后的考生列表
							cout << endl << setw(to_string(list.Length()).length() + static_cast<long long>(5)) << "位置" 
								<< setw(konge_num) << "考号"
								<< setw(konge_name) << "姓名" << setw(7) << "性别"<< setw(7) << "年龄" << "报考类别" << endl;
							number = list.Length();
							for (int i = 0; i < number; i++)
							{
								cout << setw(to_string(list.Length()).length() + static_cast<long long>(5)) << i + 1 << setw(konge_num) 
									<< list.Get(i)->get_num()<< setw(konge_name) << list.Get(i)->get_name() 
									<< setw(7) << list.Get(i)->get_sex()
									<< setw(7) << list.Get(i)->get_age() << list.Get(i)->get_major() << endl;
							}
							break;
						}
					}
				}
				if (kaohao == "0"||location==-1)
					continue;//此处表示，若输入0或者输入了不存在的考生考号，则结束删除操作，重新开始一轮外层大循环选择下一步操作
			}
			else if (choice == "3"|| choice == "4")//由于查找和修改操作的流程较为相似，因此写在一个分支里
			{
				string kaohao,order="*";
				int location;
				cout << endl << FIND_WORD;
				while (1)
				{
					cin >> kaohao;
					if (kaohao == "0")
						break;//此时表示取消当前操作
					location = list.Find(kaohao);//查询当前列表中是否存在输入的考生考号
					if (location == -1)
					{
						cout << endl << ERROR_FIND<<endl;
						break;//输入的考生不存在，则退出当前内层循环
					}
					else
					{//输出当前考号相应的考生信息
						cout << endl<< setw(konge_num) << "考号" 
							<< setw(konge_name) << "姓名" << setw(7) << "性别"<< setw(7) << "年龄" << "报考类别" << endl;
						cout << setw(konge_num) << list.Get(location)->get_num() << setw(konge_name) << list.Get(location)->get_name() << setw(7) << list.Get(location)->get_sex()
							<< setw(7) << list.Get(location)->get_age() << list.Get(location)->get_major() << endl;
						//如果是4操作，还需要执行下面的程序对考生信息进行修改，若是3操作，则查询已完成，不会执行下面的代码
						if (choice == "4")
						{
							cout << REWRITE_CHOICE;//要修改的信息对应的选项
							cin >> order;
							while (1)
							{
								if (order == "0")
									break;
								if (order == "1")//修改考号
								{
									cout << endl << REWRITE_NUM;
									break;
								}
								else if (order == "2")//修改姓名
								{
									cout << endl << REWRITE_NAME;
									break;
								}
								else if (order == "3")//修改性别
								{
									cout << endl << REWRITE_SEX;
									break;
								}
								else if (order == "4")//修改年龄
								{
									cout << endl << REWRITE_AGE;
									break;
								}
								else if (order == "5")//修改报考专业
								{
									cout << endl << REWRITE_MAJOR;
									break;
								}
								else if (order == "6")//修改该考号考生的全部信息
								{
									cout << endl << START_WORD_5;
									break;
								}
								else//无效的非法选项，重新输入
								{
									cout << endl << ERROR_REWRITE;
									continue;
								}
							}
							while (order!="0")
							{
								zero = input_data(konge_num, konge_name, order);//输入相应此操作需要输入的数据
								if (list.Same(zero.get_num()) >= 1)
								{
									cout << endl << ERROR_NUM_R;//如果考号重复则重新输入相应修改信息
									continue;
								}
								else
								{
									if (order == "6" || order == "1")
										list.Get(location)->set_num(zero.get_num());//修改姓名
									if (order == "6" || order == "2")
									{
										if (order == "2"&&list.Get(location)->get_name() == zero.get_name())
										{
											cout << endl << ERROR_NAME_R;//不支持修改后的姓名与原姓名相同，重新输入
											continue;
										}
										list.Get(location)->set_name(zero.get_name());//修改姓名
									}
									if (order == "6" || order == "3")
									{
										//不支持修改后的性别与原性别相同，重新输入
										if (order == "3"&&list.Get(location)->get_sex() == zero.get_sex())
										{
											cout << endl << ERROR_SEX_R;
											continue;
										}
										list.Get(location)->set_sex(zero.get_sex());//修改性别
									}
									if (order == "6" || order == "4")
									{
										//不支持修改后的年龄与原年龄相同，重新输入
										if (order == "4"&&list.Get(location)->get_age() == zero.get_age())
										{
											cout << endl << ERROR_AGE_R;
											continue;
										}
										list.Get(location)->set_age(zero.get_age());//修改年龄
									}
									if (order == "6" || order == "5")
									{
										//不支持修改后的报考类别与原来相同，重新输入
										if (order == "5"&&list.Get(location)->get_major() == zero.get_major())
										{
											cout << endl << ERROR_MAJOR_R;
											continue;
										}
										list.Get(location)->set_major(zero.get_major());//修改报考类别
									}
									break;
								}		
							}
							if (order != "0")
							{
								//输出修改后的考生列表信息
								cout << endl << REWRITE_END_1 << endl;
								cout << endl << setw(konge_num) << "考号" << setw(konge_name) << "姓名" << setw(7) << "性别"
									<< setw(7) << "年龄" << "报考类别" << endl;
								cout << setw(konge_num) << list.Get(location)->get_num() << setw(konge_name) << list.Get(location)->get_name() << setw(7) << list.Get(location)->get_sex()
									<< setw(7) << list.Get(location)->get_age() << list.Get(location)->get_major() << endl;
								cout << endl << REWRITE_END_2 << endl;
								number = list.Length();
								cout << endl << setw(to_string(list.Length()).length() + static_cast<long long>(5)) 
									<< "位置" << setw(konge_num) << "考号"
									<< setw(konge_name) << "姓名" << setw(7) << "性别" << setw(7) << "年龄" << "报考类别" << endl;
								for (int i = 0; i < number; i++)
								{
									cout << setw(to_string(list.Length()).length()+ static_cast<long long>(5)) << i + 1 
										<< setw(konge_num) << list.Get(i)->get_num()
										<< setw(konge_name) << list.Get(i)->get_name() << setw(7) << list.Get(i)->get_sex()
										<< setw(7) << list.Get(i)->get_age() << list.Get(i)->get_major() << endl;
								}
							}
						}
						break;
					}
					if (order == "0")
						break;
				}
				if (kaohao == "0"||location==-1||order=="0")//若输入0表示取消当前操作，或者输入了不存在的位置
					continue;
			}
			else if (choice == "5")//统计功能
			{
			    long long count_people = list.Length(), count_man = 0, count_woman = 0,count_out=0;
				cout << endl << STATISTIC<<endl;
				cout << "目前报名共有考生 " << count_people << " 人" << endl;//考生总人数即链表长度
				for (int i = 0; i < count_people; i++)
				{
					int count_age = 1, count_major = 1;
					if (list.Get(i)->get_flag_age() != "0")
					{
						for (int j = i + 1; j < count_people; j++)//此循环用于遍历链表统计同年龄段的人数
						{
							if (list.Get(j)->get_flag_age() != "0" && list.Get(i)->get_age() == list.Get(j)->get_age())
							{
								count_age++;
								list.Get(j)->set_flag_age("0");
							}
						}
						list.Get(i)->set_flag_age(to_string(count_age));
					}
					if (list.Get(i)->get_flag_major() != "0")
					{
						for (int j = i + 1; j < count_people; j++)//此循环用于遍历链表统计同专业的人数
						{
							if (list.Get(j)->get_flag_major() != "0" && list.Get(i)->get_major() == list.Get(j)->get_major())
							{
								count_major++;
								list.Get(j)->set_flag_major("0");
							}
						}
						list.Get(i)->set_flag_major(to_string(count_major));
					}
					if (list.Get(i)->get_sex() == "男")//统计男生人数，女生人数等于总人数减去男生人数
						count_man++;
				}
				if (count_people == 0)
					cout << NOTHING << endl;
				else//输出统计信息
				{
					int max_age = 0, max_major = 0;
					count_woman = count_people - count_man;
					cout << "共有男生 " << count_man << " 人，女生 " << count_woman << " 人" << endl;
					if (count_woman != 0)
						cout << "男女人数比值为: " << static_cast<double>(count_man) / static_cast<double>(count_woman) << endl;
					cout << AGE_LIST << endl;
					for(;count_out != count_people;)//此循环用于从多到低输出年龄段人数分布
					{
						count_out = 0;
						for (int j = 0; j < count_people; j++)
						{
							if (list.Get(j)->get_flag_age() != "0" && list.Get(max_age)->get_flag_age() < list.Get(j)->get_flag_age())
								max_age = j;
							else if (list.Get(j)->get_flag_age() == "0")
								++count_out;
						}
						cout << list.Get(max_age)->get_age() << "岁: " << list.Get(max_age)->get_flag_age() << " 人" << endl;
						list.Get(max_age)->set_flag_age("0");
						++count_out;
					}
					count_out = 0;
					cout << MAJOR_LIST << endl;
					for(;count_out != count_people;)//此循环用于从多到少输出报考类别人数分布
					{
						count_out = 0;
						for (int j = 0; j < count_people; j++)
						{
							if (list.Get(j)->get_flag_major() != "0" && list.Get(max_major)->get_flag_major() < list.Get(j)->get_flag_major())
								max_major = j;
							else if (list.Get(j)->get_flag_major() == "0")
								count_out++;
						}
						cout << list.Get(max_major)->get_major() << " : " << list.Get(max_major)->get_flag_major() << " 人" << endl;
						list.Get(max_major)->set_flag_major("0");
						count_out++;
					}
				}
				for (int i = 0; i < count_people; i++)
				{
					list.Get(i)->set_flag_age("\0");
					list.Get(i)->set_flag_major("\0");
				}
			}
			else if (choice == "finish")//结束重新的标志
			    break;
			else
			{
			    cout <<endl<< ERROR_CHOICE << endl;//输入了无效的非法选项，则重新输入
				continue;
            }
		}
	}
	//******************************************************************************************************
	//******************************************************************************************************
	cout << endl << "程序到此结束，请按回车键继续，此处主要是为了防止程序闪退" << endl;
	int _ = getchar();
	_ = getchar();
	//******************************************************************************************************
	//******************************************************************************************************
	return 0;
}























































// author@2052134 刘治华