#ifndef SNAKE_STRUCTS_H
#define SNAKE_STRUCTS_H

#include "head.h"

typedef struct node_t
{
    int x;
    int y;
    node_t *next;
} node, *list;

struct Snake
{
    int color;
    int length;
    Direction di;
    Direction ori_di;
    bool isAlive = true;
    bool isAI;
    list tail = nullptr;
    node *head = nullptr;
    int store;

    Snake(int _color, bool _isAI) : color(_color), length(1), di(Direction::null), ori_di(Direction::null), isAI(_isAI), store(2) {}

    void changeDi(int d);

    void stepIn(int gy,int gx);

    void move();

    void die();
};

struct Score{
    int score;
    int color;
    bool isAI;
    int no;
};

#endif //SNAKE_STRUCTS_H
