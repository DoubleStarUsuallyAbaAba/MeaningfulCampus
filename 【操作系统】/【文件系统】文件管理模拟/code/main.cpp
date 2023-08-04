#undef UNICODE
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include "function.h"
#include "tree.h"
#include <direct.h>
#include <io.h>
#include <stdlib.h>
using namespace std;

string PATH = "C:\\2052134";
string stopwords = ".\\/:*?\"<>| ";//用于文件名命名规范判断，依据windows命名规范，若出现stopwords中的字符，则命名不规范
int page = 1;
int chosen = -1;

int disk[100][4];
int bitmap[100];

void showPath() {
	cct_gotoxy(32, 1);
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	for (int i = 0; i < 10; ++i) {
		cout << "          ";//10个空格
	}
	cct_gotoxy(32, 1);
	cout << PATH;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void inputNewFileName() {
	drawTextField();
	cct_gotoxy(0, 31);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "请输入符合windows命名规范的文件名，模拟程序默认建立txt文件，请勿额外输入文件后缀名,输入完成后请敲击回车键" << endl;
	cout << "文件名: ";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void inputNewDictName() {
	drawTextField();
	cct_gotoxy(0, 31);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "请输入符合windows命名规范的文件夹名，输入完成后请敲击回车键" << endl;
	cout << "文件夹名: ";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void ReName() {
	drawTextField();
	cct_gotoxy(0, 31);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "请输入和旧名称不同的新名称，输入完成后请敲击回车键" << endl;
	cout << "新名称: ";
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}
void BackwardDelete(TreeNode* start)//后序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		BackwardDelete(start->GetLeft());
		BackwardDelete(start->GetRight());
		string fileName = start->GetFCBFileName();
		if (fileName.find('.') >= 0 && fileName.find('.') < fileName.length()) {
			//string deleteCommand = "del " + selectedFile->GetFCBAddress();
			remove((start->GetFCBAddress()).c_str());
			//system(deleteCommand.c_str());
			bitmap[start->GetFCBIndex()] = 0;
			for (int i = 0; i < start->GetFCBLength(); ++i) {
				for (int j = 0; j < 4; ++j) {
					int k = disk[start->GetFCBIndex()][i];
					if(k>=0 && k<100)
					    disk[k][j] = -1;
				}
				disk[start->GetFCBIndex()][i] = -1;
			}
		}
		else {
			RemoveDirectory((start->GetFCBAddress()).c_str());
		}
	}
}
int control(BinaryTree &FILE)
{
	int state = 0, xMouse = -1, yMouse = -1, action = -1, key_1 = -1, key_2 = -1;
	while (1){
		state = my_read_keyboard_and_mouse(xMouse, yMouse, action, key_1, key_2);//读鼠标、键盘行为，并作出响应 
		if (state == -1)//表示：如果按ESC键，则退出
			break;
		//cct_gotoxy(0, 40);
		//cout << "                       ";
		//cct_gotoxy(0, 40);
		//cout << xMouse << ' ' << yMouse;
		//((((0 <= state && state <= 9) || (21 <= state && state <= 24)) && state % 3 == 0) || (140 <= state && state <= 158 && (state - 140) % 3 == 0))
		if (state==-2) {
			int state = clickLocation(xMouse,yMouse);
			if (state != -3 && state<200) {
				especiallyShow(state);
			}
			if ((state > 200 && state <2147483647)||(state == 140)) {
				if (state > 200 && state < 2147483647) {
					TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
					padding();
					int count = FILE.Kid(tmp);
					for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
						drawMiddle(i);
					}
					for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
						cct_gotoxy(23, 3 * (i + 1) + 1);
						cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
						if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
							cct_gotoxy(135, 3 * (i + 1) + 1);
							cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
						}
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					}
					chosen = state - 200 + 8 * (page - 1);
					if (chosen <= count) {
						selectFile(chosen % 8 == 0 ? 8 : chosen % 8);
					}
					else {
						chosen = -1;
					}
					
				}
				else if (state == 140 && chosen != -1) {
					TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
					TreeNode* selectedFile = FILE.GetTargetSon(tmp, chosen);
					string fileName = selectedFile->GetFCBFileName();
					if (fileName.find('.') >= 0 && fileName.find('.') < fileName.length()) {
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cct_gotoxy(32, 1);
						for (int i = 0; i < 10; ++i) {
							cout << "          ";//10个空格
						}
						cct_gotoxy(32, 1);
						cout << PATH + "\\" + fileName;
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);

						string word;
						string filePath = selectedFile->GetFCBAddress();
						ifstream read(filePath);//读文件
						if (read.fail()){
							cout << "文件打开失败,请确保代码运行环境正常" << endl;
							exit(0);
						}
						else{
							cct_gotoxy(0, 31);
							while (read >> word){
								cout << word << ' ';//输出内容到控制台
								if (read.eof()){//判断是否读到文档末尾
									read.close();//及时关闭文件
									break;
								}
								//read >> word;//读文件
								//cout << word << ' ';//输出内容到控制台
							}
							cout << endl;
						}
						ofstream write(filePath,ifstream::out | ifstream::app);//建立文件
						if (write.fail()){
							cout << "文件操作失败,请确保代码运行环境正常" << endl;
							exit(0);
						}
						else{
							char first = getchar();//吃掉上一个回车，否则会影响本程序的后续运行
							word = "\0";
							if (first != '\n' && first != '\r') {
								getline(cin, word);//一行一行读入
								word = first + word;
							}
							write << word << endl;
							write.close();
						}
						chosen = -2;
					}
					else {
						page = 1;
						PATH = PATH + "\\" + fileName;
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
						cct_gotoxy(32, 1);
						for (int i = 0; i < 10; ++i) {
							cout << "          ";//10个空格
						}
						cct_gotoxy(32, 1);
						cout << PATH;
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
						padding();
						int count = FILE.Kid(tmp);
						for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
							drawMiddle(i);
						}
						for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
							cct_gotoxy(23, 3 * (i + 1) + 1);
							cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
							cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
							cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						}
						chosen = -1;
					}
				}
			}
			else if (state == 143) {
				if (chosen == -2) {
					cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
					cct_gotoxy(32, 1);
					for (int i = 0; i < 10; ++i) {
						cout << "          ";//10个空格
					}
					cct_gotoxy(32, 1);
					cout << PATH;
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);

					TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
					padding();
					int count = FILE.Kid(tmp);
					for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
						drawMiddle(i);
					}
					for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
						cct_gotoxy(23, 3 * (i + 1) + 1);
						cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
						if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
							cct_gotoxy(135, 3 * (i + 1) + 1);
							cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
						}
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					}
					drawTextField();
					chosen = -1;
				}
			}
			else if (state == 146 && chosen != -1) {
			    TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
			    TreeNode* selectedFile = FILE.GetTargetSon(tmp, chosen);
				string fileName = selectedFile->GetFCBFileName();
				if (fileName.find('.') >= 0 && fileName.find('.') < fileName.length()) {
					//string deleteCommand = "del " + selectedFile->GetFCBAddress();
					remove((selectedFile->GetFCBAddress()).c_str());
					//system(deleteCommand.c_str());
					bitmap[selectedFile->GetFCBIndex()] = 0;
					for (int i = 0; i < selectedFile->GetFCBLength(); ++i) {
						for (int j = 0; j < 4; ++j) {
							disk[disk[selectedFile->GetFCBIndex()][i]][j] = -1;
						}
						disk[selectedFile->GetFCBIndex()][i] = -1;
					}
					FILE.kill(selectedFile);
				}
				else {

					TreeNode* removeLocation = NULL;
					if (selectedFile->GetLeft() != NULL) {
						removeLocation = selectedFile->GetLeft();
						string filePath = removeLocation->GetFCBAddress();
						if (_access(filePath.c_str(), 0) == 0) {
							BackwardDelete(removeLocation);
						}
					}
					//if (selectedFile->GetParent()->GetRight() == selectedFile) {
					//	selectedFile->GetParent()->SetRight(selectedFile->GetRight());
					//	if (selectedFile->GetRight() != NULL)
					//	    selectedFile->GetRight()->SetParent(selectedFile->GetParent());
					//} 
					//else if (selectedFile->GetParent()->GetLeft() == selectedFile) {
					//	selectedFile->GetParent()->SetLeft(selectedFile->GetRight());
					//	if (selectedFile->GetRight() != NULL)
					//		selectedFile->GetRight()->SetParent(selectedFile->GetParent());
					//}
					RemoveDirectory((selectedFile->GetFCBAddress()).c_str());
					FILE.destroy(removeLocation);
					FILE.kill(selectedFile);
				}
				padding();
				int count = FILE.Kid(tmp);
				if (count % 8 == 0 && page>=2) {
					page--;
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
				drawAttribute();
				chosen = -1;
			}
			else if (state == 0) {
			    chosen = -1;
			    page = 1;
			    if (PATH != "C:\\2052134" && PATH != "C:\\2052134\\X" && PATH != "C:\\2052134\\Y" && PATH != "C:\\2052134\\Z") {
					int tail = PATH.length() - 1;
					for (; PATH[tail] == '\\'; --tail);
					tail = tail - 1;
					PATH.erase(tail, PATH.length() - tail);
				}
				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				cct_gotoxy(32, 1);
				cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
				for (int i = 0; i < 10; ++i) {
					cout << "          ";
				}
				cct_gotoxy(32, 1);
				cout << PATH;
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				int count = FILE.Kid(tmp);
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
            }
			else if (state == 3) {
				chosen = -1;
				page = 1;
				PATH = "C:\\2052134\\X";
				showPath();

				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				int count = FILE.Kid(tmp);
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
			}
			else if (state == 6) {
				chosen = -1;
				page = 1;
				PATH = "C:\\2052134\\Y";
				showPath();

				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				int count = FILE.Kid(tmp);
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
				//cct_gotoxy(10, 40);
				//cout << count;
				//for (int i = 0; i < count; ++i) {
				//	drawMiddle(i);
				//}
			}
			else if (state == 9) {
				chosen = -1;
				page = 1;
				PATH = "C:\\2052134\\Z";
				showPath();

				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				int count = FILE.Kid(tmp);
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
			}
			else if (state == 21) {
				chosen = -1;
				if (page >= 2) {
					page--;
				}
				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				int count = FILE.Kid(tmp);
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
				drawTextField();
			}
			else if (state == 24) {
				chosen = -1;
				TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
				padding();
				int count = FILE.Kid(tmp);
				if (8 * page < count) {
					page++;
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					drawMiddle(i);
				}
				for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
					cct_gotoxy(23, 3 * (i + 1) + 1);
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
					if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
						cct_gotoxy(135, 3 * (i + 1) + 1);
						cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
					}
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				}
				drawTextField();
			}
			else if (state == 149) {
			    chosen = -1;
				string newName = "\0";
				while (1){//对用户输入的文件名是否规范进行判断
					inputNewFileName();
					cin >> newName;
					int length = stopwords.length();
					for (int i = 0; i < length; i++){//如果用户输入的文件名中出现了stopwords中的字符，则命名不符合windows命名规范，需要重新输入
						if (newName.find(stopwords[i]) >= 0 && newName.find(stopwords[i]) < newName.length()){
							newName = "*";
							break;
						}
					}
					if (newName.length() > 0) {
						if (FILE.FindTarget((PATH + "\\" + newName + ".txt"), FILE.FindTarget(PATH, FILE.GetRoot())) != NULL) {
							newName = "*";
						}
					}
					if (newName != "*"){
						newName += ".txt";

						srand((unsigned int)time(0));
						int randomNum = (rand() % 4) + 1;//产生1-4的随机数
						int capacity = 0;
						for (int i = 0; i < 100; ++i) {
							if(bitmap[i] == 0)
							    capacity += 1;
						}
						if (capacity < randomNum+1) {
							cct_gotoxy(72, 28);
							cct_setcolor(COLOR_HCYAN, COLOR_HRED);
							cout << "!!!磁盘空间不足!!!";
							cct_setcolor(COLOR_BLACK, COLOR_WHITE);
							drawTextField();
							break;
						}

						ofstream create(PATH +"\\" +  newName);//建立文件
						if (create.fail()){
							cout << "文件创建失败,请确保代码运行环境正常" << endl;
							exit(0);
						}
						create.close();

						TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
						TreeNode*  fileLocation = FILE.Insert(FCB(newName, -1, 0, PATH + "\\" + newName), KID, tmp);

						for (int i = 0; i < 100; ++i) {
							if (bitmap[i] == 0) {
								fileLocation->SetDataIndex(i);
								fileLocation->SetDataLength(randomNum);
								bitmap[i] = 1;
								break;
							}
						}
						int num = 0;
						for (int i = 0; i < 100 && num < randomNum; ++i) {
							if (bitmap[i] == 0) {
								bitmap[i] = 1;
								disk[fileLocation->GetFCBIndex()][num] = i;
								for (int j = 0; j < 4; ++j) {
									disk[i][j] = -2;
								}
								num++;
							}
						}

						padding();
						int count = FILE.Kid(tmp);
						if ((count - 1) % 8 == 0 && count>1) {
							page++;
						}
						for (int i = 0; i < (count < 8 * page ? count%8 : 8); ++i) {
							drawMiddle(i);
						}
						for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
							cct_gotoxy(23, 3 * (i + 1)+1);
							cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
							cout << FILE.GetTargetSon(tmp, (page-1)*8+i + 1)->GetFCBFileName();
							if (FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength() > 0) {
								cct_gotoxy(135, 3 * (i + 1) + 1);
								cout << FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBLength();
							}
							cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						}
						drawAttribute();
						drawTextField();
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);

						break;
					}
					else{
						cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
						cout << "文件命名非法，请重新输入!!!";//用户输入的文件名称不规范，重新输入
						Sleep(400);
						cct_setcolor(COLOR_BLACK, COLOR_WHITE);
						continue;
					}
				}
			}
			else if (state == 152) {
			    chosen = -1;
				string newName = "\0";
				while (1) {//对用户输入的文件名是否规范进行判断
					inputNewDictName();
					cin >> newName;
					int length = stopwords.length();
					for (int i = 0; i < length; i++) {//如果用户输入的文件名中出现了stopwords中的字符，则命名不符合windows命名规范，需要重新输入
						if (newName.find(stopwords[i]) >= 0 && newName.find(stopwords[i]) < newName.length()) {
							newName = "*";
							break;
						}
					}
					if ((newName != "*") && length > 0) {
						//system(("md " + PATH + "\\" + newName).c_str());
						if (_access((PATH + "\\" + newName).c_str(), 0) == -1) { //判断该文件夹是否存在
							int flag = _mkdir((PATH + "\\" + newName).c_str());  //Windows创建文件夹
							if (flag != 0)  { //创建失败
								exit(0);
							}
							TreeNode* tmp = FILE.FindTarget(PATH, FILE.GetRoot());
							FILE.Insert(FCB(newName, -1, 0, PATH + "\\" + newName), KID, tmp);
							padding();
							int count = FILE.Kid(tmp);
							if ((count - 1) % 8 == 0 && count>1) {
								page++;
							}
							for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
								drawMiddle(i);
							}
							for (int i = 0; i < (count < 8 * page ? count % 8 : 8); ++i) {
								cct_gotoxy(23, 3 * (i + 1)+1);
								cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
								cout<<FILE.GetTargetSon(tmp, (page - 1) * 8 + i + 1)->GetFCBFileName();
								cct_setcolor(COLOR_BLACK, COLOR_WHITE);
							}
						}
						drawTextField();
						break;
					}
					else {
						cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
						cout << "文件命名非法，请重新输入!!!";//用户输入的文件名称不规范，重新输入
						Sleep(400);
						continue;
					}
				}
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
			}
		}
	}
	return 0;
}

void ForwardTravelWrite(TreeNode* start, TreeNode* root)//前序遍历，NOT_SHOW用于控制输出时不输出某些在树内存在的内容
{
	if (start != NULL)//递归思路
	{
		ofstream write("C:\\2052134\\目录文件.txt", ifstream::out | ifstream::app);//建立文件
		if (write.fail()) {
			cout << "文件操作失败,请确保代码运行环境正常" << endl;
			exit(0);
		}
		else {
			write << start->GetFCBFileName() << ' '
				<< start->GetFCBIndex() << ' '
				<< start->GetFCBLength() << ' '
				<< start->GetFCBAddress() << ' '
				<< (start == root ? "NULL" : start->GetParent()->GetFCBAddress()) << ' ';
			if (start == root) {
				write << NULL;
			}
			else {
				if (start->GetParent()->GetLeft() == start) {
					write << 0;
				}
				else {
					write << 1;
				}
			}
			write << endl;
			write.close();
			//cout << start->GetFCBFileName() << ' ' << start->GetFCBIndex() << ' ' << start->GetFCBLength() << ' ' << start->GetFCBAddress() << endl;
			ForwardTravelWrite(start->GetLeft(), root);
			ForwardTravelWrite(start->GetRight(), root);
		}
	}
}

void buildTree(BinaryTree& BUILDFILE) {
	ofstream write("C:\\2052134\\目录文件.txt", ifstream::out | ifstream::app);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		//write << "START"<<' ';
		write.close();
	}
	ifstream read("C:\\2052134\\目录文件.txt");//读文件
	if (read.fail()) {
		cout << "文件打开失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		TreeNode* node = BUILDFILE.GetRoot();
		string word, filename,fileAddress,fileParentAddress;
		int index = -1,length = 0,num=0,kid=0;
		while (read >> word) {
			//cout << word;
			//system("pause");
			//cct_cls();
			if (num % 6 == 0) {
				filename = word;
			}
			else if (num % 6 == 1) {
				index = stoi(word);
			}
			else if (num % 6 == 2) {
				length = stoi(word);
			}
			else if (num % 6 == 3) {
				fileAddress = word;
			}
			else if (num % 6 == 4) {
				fileParentAddress = word;
			}
			else if (num % 6 == 5) {
				kid = stoi(word);
			}
			num++;
			if (num == 6) {
				num = 0;
				if (fileAddress != "C:\\2052134" && fileAddress != "C:\\2052134\\X" && fileAddress != "C:\\2052134\\Y" && fileAddress != "C:\\2052134\\Z") {
					FCB file(filename, index, length, fileAddress);
					BUILDFILE.Insert(file, kid, BUILDFILE.FindTarget(fileParentAddress, node));
				}
			}
			if (read.eof()) {//判断是否读到文档末尾
				read.close();//及时关闭文件
				break;
			}
		}
	}
}

void DiskWrite()
{
	ofstream write("C:\\2052134\\磁盘文件.txt", ifstream::out);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 4; ++j) {
				write << disk[i][j];
				if (j < 3) {
					write << ' ';
				}
			}
			write << endl;
		}
		write.close();
	}
}
void BitMapWrite()
{
	ofstream write("C:\\2052134\\位图文件.txt", ifstream::out);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		for (int i = 0; i < 100; ++i) {
			write << bitmap[i] << endl;
		}
		write.close();
	}
}
void DiskRead()
{
	ofstream write("C:\\2052134\\磁盘文件.txt", ifstream::out | ifstream::app);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		//write << "START"<<' ';
		write.close();
	}
	ifstream read("C:\\2052134\\磁盘文件.txt", ifstream::in);//建立文件
	if (read.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		string word;
		int i = 0, j = 0;
		while (read >> word) {
			disk[i][j] = stoi(word);
			++j;
			if (j == 4) {
				j = 0;
				++i;
				if (i == 100) {
					read.close();//及时关闭文件
					break;
				}
			}
			if (read.eof()) {//判断是否读到文档末尾
				read.close();//及时关闭文件
				break;
			}
		}
	}
}
void BitMapRead()
{
	ofstream write("C:\\2052134\\位图文件.txt", ifstream::out | ifstream::app);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		//write << "START"<<' ';
		write.close();
	}
	ifstream read("C:\\2052134\\位图文件.txt", ifstream::in);//建立文件
	if (read.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	else {
		string word;
		int i = 0;
		while (read >> word) {
			bitmap[i] = stoi(word);
			++i;
			if (i == 100) {
				read.close();//及时关闭文件
				break;
			}
			if (read.eof()) {//判断是否读到文档末尾
				read.close();//及时关闭文件
				break;
			}
		}
	}
}

int main() {
	//创建名为output的文件夹用于输出
	string createCommand = "md C:\\2052134",deleteCommand = "rd C:\\2052134"; //mkdir
	system(createCommand.c_str());
	system((createCommand+"\\X").c_str());
	system((createCommand + "\\Y").c_str());
	system((createCommand + "\\Z").c_str());
	cout << "可能需要等待一段时间，但正常情况下会在30秒之内……" << endl;
	FCB root("2052134"), XPan("X"), YPan("Y"), ZPan("Z");
	BinaryTree MyFile(root);
	TreeNode* node = MyFile.GetRoot();
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 4; ++j) {
			disk[i][j] = -1;
		}
		bitmap[i] = 0;
	}
	XPan.address = XPan.address + "\\" + XPan.fileName;
	YPan.address = YPan.address + "\\" + YPan.fileName;
	ZPan.address = ZPan.address + "\\" + ZPan.fileName;
	MyFile.Insert(XPan, KID, node);
	MyFile.Insert(YPan, KID, node);
	MyFile.Insert(ZPan, KID, node);
	buildTree(MyFile);
	DiskRead();
	BitMapRead();
	int xViewSize = 0, yViewSize = 0, xBufferSize = 0, yBufferSize = 0;
	//cct_enable_mouse();//允许使用鼠标
	cct_getconsoleborder(xViewSize, yViewSize, xBufferSize, yBufferSize);//获取用户初始时cmd窗口的大小设置，以便改变后为用户恢复原状
	cct_setconsoleborder(X_SET, Y_SET+1, XX_SET, YY_SET+1);//改变cmd窗口的大小及缓冲区的大小，以使窗口大小适应游戏界面显示
	
    //cout << xViewSize << ' ' << yViewSize << ' ' << xBufferSize << ' ' << yBufferSize << ' ' << endl;  // 120 30 120 9001

	padding();
	drawHeadline();
	drawLeft();

	drawRight();
	drawAttribute();
	drawTextField();

	cct_gotoxy(32, 1);
	cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
	cout << PATH;
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);

	control(MyFile);

	
	
	ofstream write("C:\\2052134\\目录文件.txt", ifstream::out);//建立文件
	if (write.fail()) {
		cout << "文件操作失败,请确保代码运行环境正常" << endl;
		exit(0);
	}
	ForwardTravelWrite(MyFile.GetRoot(), MyFile.GetRoot());
	DiskWrite();
	BitMapWrite();

	cct_cls();
	system("pause");
	return 0;
}