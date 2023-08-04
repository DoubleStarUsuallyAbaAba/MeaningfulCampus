#include <iostream>
#include <string>
using namespace std;

int main()
{
    int n_dim = 0;
    string bin;
    while (1)//输入关系矩阵维数并判断是否非法输入或者输入的数字超范围，若输入无效，则重新输入
    {
        cout << "Input number of dimensions [1,100]:";
        cin >> n_dim;
        if (cin.fail() || n_dim < 1 || n_dim>100)
        {
            cin.clear();
            getline(cin, bin);
            cout << "! Illegal Input !" << endl;
            continue;
        }
        else
        {
            getline(cin, bin);
            break;
        }
    }
    char** array = new char* [n_dim];   //以下是动态创建一个 n_dim*n_dim 数组
    char ch;
    for (int k = 0; k < n_dim; k++)//初始化指针数组，防止产生野指针
        array[k] =NULL;


    array[0] = new char[n_dim];          //这句也是动态创建一个数组，配合上面开辟的数组，最终会形成n_dim*n_dim的二维数组
    for (int k = 0; k < n_dim; k++)
        array[0][k] = '\0';

    for (int i = 0; i < n_dim;)
    {
        cout << "请输入第" << i+1 << "列的" << n_dim << "个元素，每个数字之间间以一个空格隔开" << endl;
        for (int j = 0; j < n_dim; j++)
        {
            cin >> ch;
            if (cin.fail() || (ch != '1' && ch != '0'))
            {
                cin.clear();
                getline(cin, bin);
                cout << "! 存在除0与1之外的其他非法数字 !" << endl;//防止非法输入，关系矩阵中只有0、1两种数字
                break;
            }
            else
            {
                array[i][j] = ch;
                if (j == n_dim - 1)
                {
                    getline(cin, bin);//吃掉可能存在的多余的无效输入
                    i++;
                    if (i < n_dim)
                    {
                        array[i] = new char[n_dim];          //这句也是动态创建一个数组，配合上面开辟的数组，最终会形成n_dim*n_dim的二维数组
                        for (int k = 0; k < n_dim; k++)//初始化指针，防止产生野指针
                            array[i][k] = '\0';
                    }
                }
                continue;
            }
        }
    }
    //******************************************************************************
    //WarShell算法部分******************************************************************************
    for (int i = 0; i < n_dim; i++)
        for (int j = 0; j < n_dim; j++)
            for (int k = 0; k < n_dim; k++)
                if (array[i][j] == '1' && array[k][i] == '1')
                    array[k][j] = '1';
    //******************************************************************************
    //******************************************************************************

    cout << "The trasitive closure of a relation R represented:" << endl;//输出结果
    for (int i = 0; i < n_dim; i++)
    {
        for (int j = 0; j < n_dim; j++)
            cout << array[i][j];
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < n_dim; i++)//删除开辟的用于存放数组的内存
        delete array[i];

    system("pause");
    return 0;
}