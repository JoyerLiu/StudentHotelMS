
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;
class Student //储存student信息的类
{
public:
    string S_name;
    int S_num;
    int SH_num;
    Student() :S_num(-1), SH_num(-1) {}
    Student(int Snum, string name, int SHnum) :S_name(name), S_num(Snum), SH_num(SHnum) {}
    void OutCMD()
    {
        cout << S_num << "  " << S_name << "  " << SH_num << endl;
    }
};
vector<Student*> studentList;
map<string, vector<Student*>> studentTable_name;
map<int, vector<Student*>> studentTable_SHnum;

vector<Student*>::iterator Listfind(int num) //二分查找学号
{
    if (studentList.size() <= 0)
        return studentList.end();
    sort(studentList.begin(), studentList.end(), [](auto& a, auto& b)
        {
            return a->S_num < b->S_num;
        });
    int min_ = 0, mid = 0, max_ = studentList.size()-1;
    while (min_ <= max_ )
    {
        mid = (min_ + max_) / 2;
        if (studentList[mid]->S_num > num)
            max_ = mid - 1;
        else if (studentList[mid]->S_num < num)
            min_ = mid + 1;
        else
            break;
    }
    if (studentList.size() > 0 && studentList[mid]->S_num == num)
        return studentList.begin() + mid;
    return studentList.end();
}
int Tablefind(int Student_num)//在studnetTable中查询宿舍号
{
    if (studentTable_SHnum.find(Student_num) != studentTable_SHnum.end())
        return 1;
    return 0;
}
int Tablefind(string name) //查找姓名
{
    if (studentTable_name.find(name) != studentTable_name.end())
        return 1;
    return 0;
}
int DATAinput(string input)//导入Table与List
{
    if (input == "")
    {
        cout << "SYSTEM >> WARRING：发现空输入，已跳过" << endl;
        return 2;
    }
    int s1 = 0;
    int s2 = 0;
    if (input == "end")
        return 3;
    s1 = input.find(' ');//使用空格的位置对input进行分割
    s2 = input.find(' ', s1 + 1);
    if (s1 >= 11 || input.size() - s2 >= 11)
    {
        cout << "SYSTEM >> WARRING：超出长度，已跳过" << endl;
        return 4;
    }
    auto i = Listfind(stoi(input.substr(0, s1)));
    if (studentList.size() > 0 && i != studentList.end())
    {
        cout << "SYSTEM >> WARRING：发现学号"<< input.substr(0, s1) <<"已录入，将更新数据" << endl;
        (*i)->S_name = input.substr(s1 + 1, s2 - s1 - 1);
        (*i)->SH_num = stoi(input.substr(s2 + 1, input.size()));
        return 4;
    }
    studentList.push_back(new Student(stoi(input.substr(0, s1)), input.substr(s1 + 1, s2 - s1 - 1), stoi(input.substr(s2 + 1, input.size()))));//添加学生信息
    studentTable_name[studentList.back()->S_name].push_back(studentList.back());//添加到名字表中
    studentTable_SHnum[studentList.back()->SH_num].push_back(studentList.back());//添加到宿舍表中
    return 1;
}
void input_toCMD()//命令框输入
{
    string input;
    while (getline(cin, input))
    {
        int i = DATAinput(input);
        if (i == 0)
        {
            cout << "SYSTEM >> ERROR：在输入过程中发现了错误,请检查错误输出" << endl;
            break;
        }
        if (i == 3)
            break;
    }
}
void input_toData(string filePath)//从其他文件导入
{
    ifstream ifs;
    ifs.open(filePath);
    string data;
    int count_ = 0;
    while (true)
    {
        getline(ifs, data);
        if (DATAinput(data) == 3)
        {
            cout << "读取结束,共读取 " << count_ << " 个元素" << endl;
            return;
        }
        count_++;
    }
    ifs.close();
}
void input_toData()//从文件导入
{
    input_toData("Data");
}
void save_toData()//保存数据
{
    ofstream ofs;
    ofs.open("DATA", ios::trunc);
    sort(studentList.begin(), studentList.end(), [](Student*& a, Student*& b) {
        if (a->S_num > b->S_num)
            return true;
        return false;
        });
    int count_ = 0;
    for (auto i : studentList)
    {
        ofs << i->S_num << " " << i->S_name << " " << i->SH_num << endl;
        count_++;
    }
    ofs << "end";
    ofs.close();
}


void Funstion_FIND()
{
    while (true)
    {
        int a = 0;
        cout << "主页->搜索子页" << endl;
        cout << "请输入序列号选择功能：" << endl;
        cout << "1、以学生学号查找\n" << "2、以学生姓名查找\n" << "3、以宿舍号查找\n" << "4、列出全部信息\n" << "0、返回上一层" << endl;
        cout << "__________________________________________________" << endl;
        cin >> a;
        switch (a)
        {
        case 1://以学号查询
        {
            cout << "请输入学生学号" << endl;
            int num = 0;
            cin >> num;
            auto i = Listfind(num);
            cout << endl;
            if (i != studentList.end())
                (*i)->OutCMD();
            else
                cout << "未找到学生学号" << num << "的信息" << endl;
            break;
        };
        case 2://以姓名查询
        {
            cout << "请输入学生姓名" << endl;
            string name;
            cin >> name;
            cout << endl;
            if (Tablefind(name))
                for (auto i : studentTable_name[name])
                    i->OutCMD();
            else
                cout << "未找到学生姓名" << name << "的信息" << endl;
            break;
        }
        case 3://以宿舍号查询
        {
            cout << "请输入宿舍号" << endl;
            int num;
            cin >> num;
            cout << endl;
            if (Tablefind(num))
                for (auto i : studentTable_SHnum[num])
                    i->OutCMD();
            else
                cout << "未找到宿舍号" << num << "的信息" << endl;
            break;
        }
        case 4://列出全部信息
        {
            for (auto i : studentList)
                i->OutCMD();
            break;
        }
        case 0: return;
        default:
            break;
        }
        cout << "\n__________________________________________________" << endl;
    }
}
void Funstion_LOAD()
{
    cout << "正在加载数据" << endl;
    input_toData();
    cout << "完毕" << endl;
}
void Funstion_SAVE()
{
    cout << "正在储存数据" << endl;
    save_toData();
    cout << "完毕" << endl;
}
void Funstion_ADD()
{
    cout << "请输入要插入的数据，请严格按照\'学号 姓名 宿舍号\'的方式输入，并以end结束输入" << endl;
    input_toCMD();
    cout << "输入完成，正在返回上一层" << endl;
}
void Funstion_CHANGE()
{
    cout << "请输入要修改的数据，请严格按照\'学号 姓名 宿舍号\'的方式输入" << endl;
    cout << "请注意，学号不可变动" << endl;
    string input;
    getline(cin, input);
    int s1=input.find(' ');
    if (Listfind(stoi(input.substr(0, s1))) != studentList.end())
        DATAinput(input);
    else
        cout << "未找到学号"<< input.substr(0, s1) <<"，请检查输入是否有误并重新输入" << endl;

}
void Funstion_DEL()
{
    cout << "请输入要删除的数据的学号" << endl;
    string str;
    cin >> str;
    cin.ignore();
    auto i = Listfind(stoi(str));
    if ( i != studentList.end())
    {
        for (auto j = studentTable_name[(*i)->S_name].begin();j!= studentTable_name[(*i)->S_name].end();j++)//从名字表删除
            if ((*j)->S_num == (*i)->S_num)
            {
                studentTable_name[(*i)->S_name].erase(j);
                break;
            }
        for (auto j = studentTable_SHnum[(*i)->SH_num].begin(); j != studentTable_SHnum[(*i)->SH_num].end(); j++)//从宿舍表删除
            if ((*j)->SH_num == (*i)->SH_num)
            {
                studentTable_SHnum[(*i)->SH_num].erase(j);
                break;
            }   
        studentList.erase(i);
    }
    else
    {
        cout << "未找到相关学号" << str << "的信息，请检查输入" << endl;
    }
}


int main()
{
    input_toData();
    while (true)
    {
        cout << "****** 欢迎使用宿舍管理系统  ******\n\n已录入" << to_string(studentList.size()) << "条信息\n" << endl;
        string UImassage[] = { "导入数据","保存数据","查找信息","添加信息","修改信息","删除信息","退出系统" };
        cout << "请输入序列号选择功能：" << endl;
        int number = 1;
        for (auto i : UImassage)
            cout << number << "、" << i << endl, number++;
        cout << "____________________________________________" << endl;
        int n = 0;
        cin >> n;
        cin.ignore(1000,'\n');
        switch (n)
        {
        case 1: Funstion_LOAD(); break;
        case 2: Funstion_SAVE(); break;
        case 3: Funstion_FIND(); break;
        case 4: Funstion_ADD(); break;
        case 5: Funstion_CHANGE(); break;
        case 6: Funstion_DEL(); break;
        case 7:
        {
            cout << "请确认输入！(Y/N)" << endl;
            string a;
            cin >> a;
            cin.ignore(1000, '\n');
            if (a == "Y")
                return 0;
        }
        default:
            break;
        }
    }
}

