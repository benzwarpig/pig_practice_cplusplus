cmake_minimum_required(VERSION 3.5)

add_compile_options(-std=c++14)

project(PracticeCPlusPlus)

find_package(spdlog)
include_directories(${spdlog_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-g") # 调试信息
set(CMAKE_CXX_FLAGS "-Wall") # 开启所有警告

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

message(STATUS "current_source_dir : ${CMAKE_CURRENT_SOURCE_DIR}")

# find protobuf end
include_directories(./lib/algorithm)
include_directories(./lib/algorithm/graph_search)
include_directories(./lib/DesignPattern/FactoryMode)
include_directories(./lib/DesignPattern/ObserverMode)
include_directories(./lib/CallBack)
include_directories(./lib/ThreadPool)
include_directories(./lib/FILE)
include_directories(./lib/LeetCode)

include_directories(./lib/C++11/function_templete)
include_directories(./lib/C++11/future_asyn)
include_directories(./lib/C++11/CAS)
include_directories(./lib/C++11/lambda)
include_directories(./lib/C++11/variable_parameter)

include_directories(./lib/C++17/Optional)

# include_directories for linux func
include_directories(./linux/message_queue)
include_directories(./linux/semaphore)
include_directories(./linux/share_memory)
include_directories(./linux/signal)

# 遍历项目根目录下所有的 .cpp 文件
file(GLOB_RECURSE files *.cpp)

foreach(file ${files})
    if(file MATCHES practise.cpp)
        message(STATUS "ignore practise.cpp")
    else()
        string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
        add_executable(${exe} ${file})
        message(\ \ \ \ --\ src/${exe}.cpp\ will\ be\ compiled\ to\ bin/${exe})
    endif()
endforeach()

add_subdirectory(cmake_practise)

target_link_libraries(asyn_test spdlog pthread m pthread)
target_link_libraries(copy_and_swap spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(ThreadPool spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(ProtoType spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(Optional spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(variable_parameter spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_signal spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_share_memory spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_semaphore spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_semaphore_test spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_message_queue spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_message_queue_A spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(pig_message_queue_B spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(ObserverMode spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(area_interpolation spdlog pthread ${ZMQ_LIBRARIES} m pthread)
target_link_libraries(graph_search_planner spdlog pthread ${ZMQ_LIBRARIES} m pthread)

