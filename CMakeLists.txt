
cmake_minimum_required(VERSION 3.31)
project(biblioteka_numeryczna)

set(CMAKE_CXX_STANDARD 20)

add_executable(biblioteka_numeryczna
        main.cpp
        ../biblioteka-numeryczna/include/interpolacja.h
        ../biblioteka-numeryczna/include/ukladyRownanLiniowych.h
        ../biblioteka-numeryczna/include/aproksymacja.h
        ../biblioteka-numeryczna/include/calkowanieNumeryczne.h
        ../biblioteka-numeryczna/include/rownaniaRozniczkowe.h
        ../biblioteka-numeryczna/include/rownaniaNieliniowe.h
        ../biblioteka-numeryczna/src/ukladyRownanLiniowych.cpp
        ../biblioteka-numeryczna/src/interpolacja.cpp
        ../biblioteka-numeryczna/src/aproksymacja.cpp
        ../biblioteka-numeryczna/src/calkowanieNumeryczne.cpp
        ../biblioteka-numeryczna/src/rownaniaRozniczkowe.cpp
        ../biblioteka-numeryczna/src/rownaniaNieliniowe.cpp
        ../biblioteka-numeryczna/src/wczytanieDanych.cpp)

target_include_directories(biblioteka_numeryczna PRIVATE include)
