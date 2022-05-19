cmake_minimum_required(VERSION 3.22)
project(simpleformat)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(simpleformat
        SimpleFormat.cpp
        main.cpp
        SimpleFormat.hpp)
