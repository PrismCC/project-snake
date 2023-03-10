#include "play.h"
#include "others.h"

int main()
{
    system("chcp 65001 > nul");
    int option1[5] = {-1, 1, 2, 3, 0};
    int option2[4] = {-1, 4, 8, 12};

    puts("贪吃蛇");

    STATE_1:

    string mapName = "empty";

#ifndef DEBUG
    puts("请输入加载的地图文件名(不用输入\".txt\"), 输入quit退出:  ");
    cin >> mapName;
    cin.get();
    if (mapName == "quit")
        return 0;
#endif

    fs::path mapPath{"map/" + mapName + ".txt"};
    ifstream mapIn{mapPath};

    while (mapIn.fail())
    {
        puts("无法打开该文件, 请检查是否输入正确及文件是否存在");

        puts("请输入加载的地图文件名(不用输入\".txt\"), 输入quit退出:  ");
        cin >> mapName;
        cin.get();
        if (mapName == "quit")
            return 0;

        mapPath = {"map/" + mapName + ".txt"};
        mapIn.open(mapPath);
    }

    string checkLine;
    getline(mapIn, checkLine);
    if (checkLine != "MAP_FOR_SNAKE")
    {
        puts("该文件格式错误, 请检查是否是地图文件");
        goto STATE_1;
    }

    puts("打开文件成功");
    printMap(mapName);

    STATE_2:

    int playerNum = 1, totalNum = 3;

#ifndef DEBUG
    puts("请输入游玩人数: [1] 单人  [2] 双人  [3] 三人  [4] 旁观  [0] 返回");
    cin >> playerNum;
#endif

    while (playerNum < 0 || playerNum > 4)
    {
        puts("输入格式错误, 请重新输入");
        puts("请输入游玩人数: [1] 单人  [2] 双人  [3] 三人  [4] 旁观  [0] 返回");
        cin >> playerNum;
    }

    if (playerNum == 0)
        goto STATE_1;

#ifndef DEBUG
    puts("请输入总人数: [1] 4  [2] 8  [3] 12  [0] 返回");
    cin >> totalNum;
#endif

    while (totalNum < 0 || totalNum > 3)
    {
        puts("输入格式错误, 请重新输入");
        puts("请输入总人数: [1] 4  [2] 8  [3] 12  [0] 返回");
        cin >> totalNum;
    }

    if (totalNum == 0)
        goto STATE_2;

    string name;
    int height, width;
    mapIn >> name >> height >> width;

    prepare(mapName, option1[playerNum], option2[totalNum]);
    play(option1[playerNum], option2[totalNum]);

    clearScreen();
    puts("游戏结束");
    printScore(true);
    cin.get();
    cin.get();
    return 0;
}
