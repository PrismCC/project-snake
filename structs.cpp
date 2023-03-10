#include "structs.h"

extern vector<vector<int>> snakeMap;
extern vector<Snake> snakes;

void Snake::changeDi(int d)
{
    switch (d)
    {
        case 1:
            di = Direction::up;
            break;
        case 2:
            di = Direction::right;
            break;
        case 3:
            di = Direction::down;
            break;
        case 4:
            di = Direction::left;
            break;
        default:
            break;
    }
    snakeMap[head->y][head->x] = 200 + color * 4 + (int) di - 1;
}

void Snake::stepIn(int gy, int gx)
{
    snakeMap[head->y][head->x] = 100 + color;
    snakeMap[gy][gx] = 200 + color * 4 + (int) di - 1;

    node *newNode = new node;
    newNode->y = gy;
    newNode->x = gx;
    newNode->next = nullptr;
    head->next = newNode;
    head = newNode;
}

void Snake::move()
{
    int gx = head->x, gy = head->y;
    int height = (int) snakeMap.size();
    int width = (int) snakeMap[0].size();
    if (di == Direction::up)
        gy = (gy + 1) % height;
    else if (di == Direction::down)
        gy = (gy - 1 + height) % height;
    else if (di == Direction::right)
        gx = (gx + 1) % width;
    else if (di == Direction::left)
        gx = (gx - 1 + width) % width;
    int block = snakeMap[gy][gx];

    if(block == 2)
        ++store;

    if (store > 0)
    {
        --store;
        ++length;
    }
    else
    {
        node *p = tail->next;
        snakeMap[tail->y][tail->x] = 0;
        free(tail);
        tail = p;
    }


    switch (block)
    {
        case 2:
        case 0:
            stepIn(gy, gx);
            break;
        case 1:
            die();
            return;
        default:
            if (block > 100 && block <= 200)
                die();
            else
            {
                for (auto& x: snakes)
                {
                    if(!x.isAlive||x.color==color)
                        continue;
                    if (x.head->y == gy && x.head->x == gx)
                    {
                        x.die();
                        break;
                    }
                }
                die();
            }
            return;
    }
    ori_di=di;
}

void Snake::die()
{
    isAlive = false;
    node *p = tail->next;
    while (tail != head)
    {
        int r = rand() % 4;
        if (r == 0)
            snakeMap[tail->y][tail->x] = 2;
        else
            snakeMap[tail->y][tail->x] = 0;
        free(tail);
        tail = p;
        p = p->next;
    }
    snakeMap[tail->y][tail->x] = 2;
    free(tail);
    tail = head = nullptr;
    length = 0;
    store = 0;
}


