cmake_minimum_required(VERSION 3.10)
project(Subway)

set(CMAKE_CXX_STANDARD 20)

add_executable(Subway
        main.cpp
        metro.h
        metro.cpp)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
add_library(subway_lib metro.cpp)
target_include_directories(subway_lib PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(Subway PRIVATE subway_lib)
