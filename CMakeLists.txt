cmake_minimum_required(VERSION 3.4)
project(SUN_NEW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    SUN_NEW/main.cpp
    SUN_NEW/model.cpp
    SUN_NEW/model.h
    SUN_NEW/multiNet.cpp
    SUN_NEW/multiNet.h
    SUN_NEW/network.cpp
    SUN_NEW/network.h
    SUN_NEW/node.cpp
    SUN_NEW/node.h
    SUN_NEW/other.cpp
    SUN_NEW/other.h
    SUN_NEW/resource.h
    SUN_NEW/stdafx.cpp
    SUN_NEW/stdafx.h
    SUN_NEW/targetver.h)

add_executable(SUN_NEW ${SOURCE_FILES})