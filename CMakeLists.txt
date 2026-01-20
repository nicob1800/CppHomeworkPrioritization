cmake_minimum_required(VERSION 4.1)
project(HomeworkPrioritizer)

set(CMAKE_CXX_STANDARD 20)

add_executable(HomeworkPrioritizer main.cpp
        task.h
        TaskManager.h
        sqlite3.c
        sqlite3.h)
find_package(Threads REQUIRED)
target_link_libraries(HomeworkPrioritizer Threads::Threads)
