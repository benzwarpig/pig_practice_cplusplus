cmake_minimum_required(VERSION 3.12)

project(cmake_add_definitions)

add_compile_options(-std=c++14)

add_definitions(-DPIG_TESTER)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED
    practise.cpp
)

target_link_libraries(${PROJECT_NAME})
