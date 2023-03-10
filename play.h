#ifndef SNAKE_PLAY_H
#define SNAKE_PLAY_H

#include "head.h"
#include "structs.h"

void prepare(string &mapName, int playerNum, int totalNum);

void play(int playerNum, int totalNum);

void printMap(string &mapName);

void loadMap(string &mapName);

bool isEnd();

void printScreen();

void inputDirection(int playerNum);

void AiDirection(int playerNum, int totalNum);

void changeDirection(int playerNum, char key);

void setFood(int num);

int compare(const void *a, const void *b);

void printScore(bool flag = false);

inline vector<Snake> snakes;
inline vector<vector<int>> snakeMap;


#endif //SNAKE_PLAY_H
