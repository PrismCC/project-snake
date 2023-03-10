cmake_minimum_required(VERSION 3.23)
project(snake)

set(CMAKE_CXX_STANDARD 20)

add_executable(snake main.cpp head.h structs.cpp structs.h others.cpp others.h play.cpp play.h)
