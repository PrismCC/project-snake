#ifndef SNAKE_HEAD_H
#define SNAKE_HEAD_H

#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;
namespace fs = std::filesystem;

enum class Direction { null = 0, up = 1, right = 2, down = 3, left = 4 };

#define SEPARATE_LINE puts("__________________________________________________")
//#define DEBUG
#define SPEED 300

inline const int colorTable[13]={0,1,2,3,4,5,6,9,10,11,12,13,14};

#endif //SNAKE_HEAD_H
