#ifndef SNAKE_OTHERS_H
#define SNAKE_OTHERS_H

#include "head.h"

vector<int> random_select(vector<int>& nums, int m);
void setRandomSeed();
int randomDirection(int way[3]);
void textColor(int color=7, int back = 0);
void clearScreen();
void loading();
void emptyInput();

#endif //SNAKE_OTHERS_H
