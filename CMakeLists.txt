cmake_minimum_required(VERSION 3.31)
project(biblioteka_numeryczna)

set(CMAKE_CXX_STANDARD 20)

# Włącz testy
enable_testing()

# Dodaj GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Dla Windows: force shared libraries
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Utwórz bibliotekę (bez main.cpp)
add_library(biblioteka_numeryczna
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

target_include_directories(biblioteka_numeryczna PUBLIC include)

# Utwórz plik wykonywalny z main.cpp
add_executable(main main.cpp)

# Podłącz bibliotekę do pliku wykonywalnego
target_link_libraries(main biblioteka_numeryczna)

# Dodaj katalog z testami
add_subdirectory(tests)