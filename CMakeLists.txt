

cmake_minimum_required(VERSION 3.10)

project(raii LANGUAGES CXX)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ./include)


