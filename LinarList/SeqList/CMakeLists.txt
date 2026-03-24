cmake_minimum_required(VERSION 3.10)
project(SeqList)

set(CMAKE_CXX_STANDARD 17)

add_executable(SeqList
    main.cpp
    SeqList.cpp
)

target_include_directories(SeqList PRIVATE .)