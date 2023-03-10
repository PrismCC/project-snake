#include "play.h"
#include "others.h"
#include <cstring>



void prepare(string &mapName, int playerNum, int totalNum)
{
    clearScreen();
    loadMap(mapName);

    int height = (int) snakeMap.size(), width = (int) snakeMap[0].size();

    bool colorUsed[13] = {false};
    for (int i = 1; i <= 12; ++i)
    {
        textColor(colorTable[i]);
        cout << i << "  ";
    }
    textColor(7);
    puts("");
    for (int i = 1; i <= playerNum; ++i)
    {
        int color;
        printf("请选择玩家%d的颜色(参见上面序号): ", i);
        cin >> color;
        while (color <= 0 || color > 12 || colorUsed[color])
        {
            puts("输入格式错误或该颜色已被占用, 请重新输入");
            cin >> color;
        }
        colorUsed[color] = true;
        snakes.emplace_back(colorTable[color], false);
    }

    vector<int> colors, results;
    for (int i = 1; i <= 12; ++i)
    {
        if (!colorUsed[i])
            colors.push_back(i);
    }
    results = random_select(colors, totalNum - playerNum);
    for (auto x: results)
        snakes.emplace_back(colorTable[x], true);

    puts("玩家总览:");
    for (int count = 1; auto x: snakes)
    {
        textColor(x.color);
        cout << "序号:\t" << count++ << '\t' << (x.isAI ? "AI" : "Human") << endl;
    }
    textColor(7);
    SEPARATE_LINE;
    puts("操作方法:");
    textColor(4);
    puts("玩家一: w s a d");
    textColor(1);
    puts("玩家二: i k j l");
    textColor(2);
    puts("玩家三: 5 3 2 1");

    vector<int> coordinates;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (snakeMap[i][j] == 0)
                coordinates.push_back(i * width + j);
        }
    }

    vector<int> result = random_select(coordinates, totalNum);

    for (int i = 0; i < totalNum; ++i)
    {
        node *newNode = new node;
        newNode->y = result[i] / width;
        newNode->x = result[i] % width;
        newNode->next = nullptr;

        snakes[i].tail = snakes[i].head = newNode;
    }

    setRandomSeed();
    for (int i = playerNum; i < totalNum; ++i)
    {
        snakes[i].changeDi(rand() % 4 + 1);
        snakes[i].ori_di = snakes[i].di;
    }

#ifdef DEBUG
    loading();
#endif

}

void play(int playerNum, int totalNum)
{

    for (auto x: snakes)
        snakeMap[x.tail->y][x.tail->x] = 100 + x.color;

    inputDirection(playerNum);

    int round = 0;

    while (!isEnd())
    {
        while (_kbhit())
        {
            char key = (char) getch();
            changeDirection(playerNum, key);

            clearScreen();
            printScreen();
        }

        AiDirection(playerNum, totalNum);

        for (auto &x: snakes)
        {
            if (x.isAlive)
                x.move();
        }

        int foodNum = rand() % 2;
        setFood(foodNum);

        clearScreen();
        if (round % 10 == 0)
            system("cls");
        printScreen();
        Sleep(SPEED);

        ++round;
    }
}


void printMap(string &mapName)
{
    fs::path mapPath{"map/" + mapName + ".txt"};
    ifstream mapIn{mapPath};

    int height, width;
    string check, name;
    mapIn >> check >> name;
    mapIn >> height >> width;
    SEPARATE_LINE;
    cout << "地图名: " << mapName << "   大小: " << height << "*" << width << endl;
    for (int i = height - 1; i >= 0; --i)
    {
        for (int j = 0; j < width; ++j)
        {
            char c;
            do
                mapIn >> c;
            while (isspace(c));
            if (c == '0')
                cout << "□";
            else if (c == '1')
                cout << "⊠";
            else if (c == '2')
                cout << "❦";
        }
        puts("");
    }
    SEPARATE_LINE;
}

void loadMap(string &mapName)
{
    fs::path mapPath{"map/" + mapName + ".txt"};
    ifstream mapIn{mapPath};
    int height, width;
    string check, name;
    mapIn >> check >> name;
    mapIn >> height >> width;

    for (int i = 0; i < height; ++i)
    {
        vector<int> newLine;
        for (int j = 0; j < width; ++j)
        {
            char c;
            do
                mapIn >> c;
            while (isspace(c));
            newLine.push_back(c - '0');
        }
        snakeMap.insert(snakeMap.begin(), newLine);
    }
}

bool isEnd()
{
    bool result = true;
    for (auto x: snakes)
    {
        if (x.isAlive)
            result = false;
    }
    return result;
}

void printScreen()
{
    int height = (int) snakeMap.size();
    int width = (int) snakeMap[0].size();

    for (int i = height - 1; i >= 0; --i)
    {
        for (int j = 0; j < width; ++j)
        {
            int v = snakeMap[i][j];
            switch (v)
            {
                case 0:
                    textColor(15);
                    cout << "⊡";
                    break;
                case 1:
                    textColor(8);
                    cout << "⊠";
                    break;
                case 2:
                    textColor(13);
                    cout << "❦";
                    break;
                default:
                    if (v > 100 && v <= 200)
                    {
                        textColor(v - 100);
                        cout << "⧈";
                    }
                    else
                    {
                        int color = (v - 200) / 4, di = (v - 200) % 4;
                        textColor(color);
                        cout << (di <= 1 ? (di == 0 ? "▲" : "▶") : (di == 2 ? "▼" : "◀"));
                    }
            }
        }
        puts("");
    }
    textColor();
    SEPARATE_LINE;
    printScore();
}

void inputDirection(int playerNum)
{
    bool player1Flag = false, player2Flag = false, player3Flag = false;
    if (playerNum < 1)
        player1Flag = true;
    if (playerNum < 2)
        player2Flag = true;
    if (playerNum < 3)
        player3Flag = true;

    while (!(player1Flag && player2Flag && player3Flag))
    {
        clearScreen();
        printScreen();
        SEPARATE_LINE;
        puts("所有玩家按下任意方向键后开始游戏");

        char key = 0;
        if (_kbhit())
            key = (char) getch();

#ifdef DEBUG
        key = 'w';
#endif

        if (key != 0)
        {
            if (strchr("wdsa", key) != nullptr)
            {
                if (key == 'w')
                    snakes[0].ori_di = snakes[0].di = Direction::up;
                else if (key == 'd')
                    snakes[0].ori_di = snakes[0].di = Direction::right;
                else if (key == 's')
                    snakes[0].ori_di = snakes[0].di = Direction::down;
                else if (key == 'a')
                    snakes[0].ori_di = snakes[0].di = Direction::left;
                snakeMap[snakes[0].tail->y][snakes[0].tail->x] = 200 + snakes[0].color * 4 + (int) snakes[0].di - 1;
                player1Flag = true;
            }
            else if (playerNum >= 2 && strchr("ilkj", key) != nullptr)
            {
                if (key == 'i')
                    snakes[1].ori_di = snakes[1].di = Direction::up;
                else if (key == 'l')
                    snakes[1].ori_di = snakes[1].di = Direction::right;
                else if (key == 'k')
                    snakes[1].ori_di = snakes[1].di = Direction::down;
                else if (key == 'j')
                    snakes[1].ori_di = snakes[1].di = Direction::left;
                snakeMap[snakes[1].tail->y][snakes[1].tail->x] = 200 + snakes[1].color * 4 + (int) snakes[1].di - 1;
                player2Flag = true;
            }
            else if (playerNum >= 3 && strchr("5321", key) != nullptr)
            {
                if (key == '5')
                    snakes[2].ori_di = snakes[2].di = Direction::up;
                else if (key == '3')
                    snakes[2].ori_di = snakes[2].di = Direction::right;
                else if (key == '2')
                    snakes[2].ori_di = snakes[2].di = Direction::down;
                else if (key == '1')
                    snakes[2].ori_di = snakes[2].di = Direction::left;
                snakeMap[snakes[2].tail->y][snakes[2].tail->x] = 200 + snakes[2].color * 4 + (int) snakes[2].di - 1;
                player3Flag = true;
            }
        }
        Sleep(1000);
    }

    puts("游戏即将开始...");
    Sleep(1000);
}

void AiDirection(int playerNum, int totalNum)
{
    int height = (int) snakeMap.size(), width = (int) snakeMap[0].size();

    for (int i = playerNum; i < totalNum; ++i)
    {
        if (!snakes[i].isAlive)
            continue;
        int way[3] = {0, 2, 0};
        int headX = snakes[i].head->x, headY = snakes[i].head->y;
        int straightX, straightY, turnLeftX, turnLeftY, turnRightX, turnRightY;
        switch (snakes[i].ori_di)
        {
            case Direction::up:
                straightX = headX;
                straightY = (headY + 1) % height;
                turnLeftX = (headX - 1 + width) % width;
                turnRightX = (headX + 1) % width;
                turnLeftY = turnRightY = headY;
                break;
            case Direction::down:
                straightX = headX;
                straightY = (headY - 1 + height) % height;
                turnRightX = (headX - 1 + width) % width;
                turnLeftX = (headX + 1) % width;
                turnLeftY = turnRightY = headY;
                break;
            case Direction::right:
                straightX = (headX + 1) % width;
                straightY = headY;
                turnLeftY = (headY + 1) % height;
                turnRightY = (headY - 1 + height) % height;
                turnLeftX = turnRightX = headX;
                break;
            case Direction::left:
                straightX = (headX - 1 + width) % width;
                straightY = headY;
                turnRightY = (headY + 1) % height;
                turnLeftY = (headY - 1 + height) % height;
                turnLeftX = turnRightX = headX;
                break;
            default:
                exit(2);
        }
        int block[3] = {snakeMap[turnLeftY][turnLeftX], snakeMap[straightY][straightX],
                        snakeMap[turnRightY][turnRightX]};

        for (int j = 0; j < 3; ++j)
        {
            if (block[j] == 0)
                way[j] += 1;
            else if (block[j] == 2)
                way[j] = 10;
            else
                way[j] = 0;
        }
        int di[4][3] = {
                {4, 1, 2},
                {1, 2, 3},
                {2, 3, 4},
                {3, 4, 1}};
        int choice = randomDirection(way);
        snakes[i].changeDi(di[(int) snakes[i].ori_di - 1][choice]);
    }
}

void changeDirection(int playerNum, char key)
{
    if (snakes[0].isAlive && strchr("wdsa", key) != nullptr)
    {
        if (key == 'w' && snakes[0].ori_di != Direction::down)
            snakes[0].changeDi(1);
        else if (key == 'd' && snakes[0].ori_di != Direction::left)
            snakes[0].changeDi(2);
        else if (key == 's' && snakes[0].ori_di != Direction::up)
            snakes[0].changeDi(3);
        else if (key == 'a' && snakes[0].ori_di != Direction::right)
            snakes[0].changeDi(4);
    }
    else if (snakes[1].isAlive && playerNum >= 2 && strchr("ilkj", key) != nullptr)
    {
        if (key == 'i' && snakes[1].ori_di != Direction::down)
            snakes[1].changeDi(1);
        else if (key == 'l' && snakes[1].ori_di != Direction::left)
            snakes[1].changeDi(2);
        else if (key == 'k' && snakes[1].ori_di != Direction::up)
            snakes[1].changeDi(3);
        else if (key == 'j' && snakes[1].ori_di != Direction::right)
            snakes[1].changeDi(4);
    }
    else if (snakes[2].isAlive && playerNum >= 3 && strchr("5321", key) != nullptr)
    {
        if (key == '5' && snakes[2].ori_di != Direction::down)
            snakes[2].changeDi(1);
        else if (key == '3' && snakes[2].ori_di != Direction::left)
            snakes[2].changeDi(2);
        else if (key == '2' && snakes[2].ori_di != Direction::up)
            snakes[2].changeDi(3);
        else if (key == '1' && snakes[2].ori_di != Direction::right)
            snakes[2].changeDi(4);
    }
}

void setFood(int num)
{
    vector<int> coordinates;
    int height = (int) snakeMap.size(), width = (int) snakeMap[0].size();

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (snakeMap[i][j] == 0)
                coordinates.push_back(i * width + j);
        }
    }

    vector<int> result = random_select(coordinates, num);

    for (auto x: result)
        snakeMap[x / width][x % width] = 2;
}

int compare(const void *a, const void *b)
{
    auto *x = (Score *) a, *y = (Score *) b;
    return (-x->score + y->score);
}

void printScore(bool flag)
{
    Score list[12];
    static Score max = {-1, 0, false, 13};
    int number = 0;
    if (flag)
    {
        textColor(max.color);
        cout << "胜利者: " << max.no << "\t最高分: " << max.score << endl;
        textColor();
        return;
    }
    for (int i = 0; i < snakes.size(); ++i)
    {
        if (snakes[i].isAlive)
        {
            list[number].score = snakes[i].store + snakes[i].length;
            list[number].color = snakes[i].color;
            list[number].isAI = snakes[i].isAI;
            list[number].no = i + 1;

            if (list[number].score > max.score)
                max = list[number];
            ++number;
        }
    }
    qsort(list, number, sizeof(Score), compare);

    textColor(max.color);
    cout << "最高分: " << max.score << "编号: " << max.no << endl;
    textColor();
    cout << "排名\t编号\t分数\t类别" << endl;
    for (int i = 0; i < min(5, number); ++i)
    {
        textColor(list[i].color);
        cout << i + 1 << '\t' << list[i].no << '\t' << list[i].score << '\t' << (list[i].isAI ? "AI" : "Human") << endl;
    }
    textColor();
}