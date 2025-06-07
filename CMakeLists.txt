
cmake_minimum_required(VERSION 3.31)
project(biblioteka_numeryczna)

set(CMAKE_CXX_STANDARD 20)

add_executable(biblioteka_numeryczna main.cpp
        include/interpolacja.h
        include/ukladyRownanLiniowych.h
        include/aproksymacja.h
        include/calkowanieNumeryczne.h
        include/rownaniaRozniczkowe.h
        include/rownaniaNieliniowe.h
        include/funkcje.h
        src/ukladyRownanLiniowych.cpp
        src/interpolacja.cpp
        src/aproksymacja.cpp
        src/calkowanieNumeryczne.cpp
        src/rownaniaRozniczkowe.cpp
        src/rownaniaNieliniowe.cpp
        src/funkcje.cpp)