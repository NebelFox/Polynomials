cmake_minimum_required(VERSION 3.19)
project(3_polynomials)

set(CMAKE_CXX_STANDARD 20)

#set (CMAKE_EXE_LINKER_FLAGS "-static")

#SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "E:/programs/boost/boost_1_75_0")
#SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "E:/programs/boost/boost_1_75_0")
#
#find_package(Boost)
#                IF (Boost_FOUND)
#                include_directories(${Boost_INCLUDE_DIR})
#                endif()

add_executable(3_polynomials main.cpp Polynom.cpp Polynom.h)