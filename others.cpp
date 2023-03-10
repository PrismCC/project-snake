#include "others.h"
#include <random>

// 从n个数中随机选取m个数
vector<int> random_select(vector<int> &nums, int m)
{
    vector<int> result; // 存储结果
    int n = (int) nums.size(); // 获取n的值
    if (m > n) return result; // 如果m大于n，返回空结果
    static default_random_engine engine(random_device{}()); // 创建默认的随机数引擎
    static uniform_int_distribution<int> dist(0, 1000000); // 创建均匀分布
    for (int i = 0; i < m; i++)
    {
        int index = dist(engine) % n; // 随机生成一个索引
        result.push_back(nums[index]); // 将对应的数放入结果中
        swap(nums[index], nums[n - 1]); // 将选中的数与最后一个数交换
        n--; // 将n减一，避免重复选择
    }
    return result;
}

//设置随机数种子
void setRandomSeed()
{
    clockid_t a = 0;
    struct timespec p = {0, 0};
    clock_gettime(a, &p);
    srand((unsigned)p.tv_nsec);
}

int randomDirection(int way[3])
{
    int sum=0;
    for(int i=0;i<3;++i)
        sum+=way[i];
    if(sum<1)
        return rand()%3;
    for(int i=0;i<3;++i)
    {
        if(i>0)
            way[i]+=way[i-1];
    }
    double r=rand()%sum;
    for(int i=0;i<3;++i)
        if(r<way[i])
            return i;
    return 1;
}

inline void textColor(int color, int back) //0黑 1深蓝 2绿 3青 4红 5紫 6黄 7白 8灰 9蓝 10嫩绿 11浅青 12橙 13粉红 14亮黄 15黄白
{
    static int ori_color=7;
    static HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if(color!=ori_color)
    {
        SetConsoleTextAttribute(output, color | (back % 16) * 16);
        ori_color=color;
    }
}


void clearScreen()// 清屏
{
    printf("\033[2J"); // 清屏
    printf("\033[0;0H"); // 光标移动到左上角
}

void loading()
{
    textColor(13);
    for (int i = 0; i < 3; ++i)
    {
        cout << "少女祈祷中";
        Sleep(300);
        cout << ".";
        Sleep(300);
        cout << ".";
        Sleep(300);
        cout << ".";
        Sleep(300);
        cout << "\r";
        cout << "               ";
        cout << "\r";
    }
    textColor(7);
    clearScreen();
}

void emptyInput()
{
    while (_kbhit())
        getch();
}