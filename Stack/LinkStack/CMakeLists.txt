cmake_minimum_required(VERSION 3.10)
project(LinkStack)

set(CMAKE_CXX_STANDARD 17)

add_executable(LinkStack
    main.cpp
    LinkStack.cpp
)

target_include_directories(LinkStack PRIVATE .)