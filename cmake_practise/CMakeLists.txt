cmake_minimum_required(VERSION 3.12)

project(cmake_practise)

add_executable(${PROJECT_NAME}
    cmake_practise.cpp
)

add_subdirectory(add_definitions)

target_link_libraries(${PROJECT_NAME}
    cmake_add_definitions
    m
)
