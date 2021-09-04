

cmake_minimum_required(VERSION 3.10)

set(RAII_LIBRARY_NAME raii)

find_package(fmt REQUIRED)

project(${RAII_LIBRARY_NAME} LANGUAGES CXX)

add_library(${RAII_LIBRARY_NAME} INTERFACE)

target_include_directories(${RAII_LIBRARY_NAME} INTERFACE ./include)

target_link_libraries(${RAII_LIBRARY_NAME} INTERFACE fmt::fmt)

