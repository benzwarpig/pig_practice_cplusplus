#pragma once

#include <spdlog/spdlog.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

static bool is_opend = false;
static FILE* collect_fd = nullptr;
static const std::string collect_path =
    "/home/freedom/wxc/practice_cplusplus/lib/FILE/collect_data.txt";
static std::string write_string = "Sensor Data : ";

namespace CollectSensorData {

template <class T1>
void AddSensorDataToFile(T1 p) {
    write_string += std::to_string(p) + "\r\n";

    fwrite(write_string.c_str(), sizeof(char), write_string.size(), collect_fd);
    spdlog::info("write_string:{}", write_string);

    std::string tmp = "Sensor Data : ";
    write_string.swap(tmp);
}

template <class T1, class... T2>
void AddSensorDataToFile(T1 p, T2... arg) {
    if (!is_opend) {
        is_opend = true;
        collect_fd = fopen(collect_path.c_str(), "w+");
        if (collect_fd == nullptr) std::abort();
    }
    write_string += std::to_string(p) + "   ";
    AddSensorDataToFile(arg...);
}

template <class... T1>
void AddSensorDataToFile(T1... arg) {
    spdlog::info("T1... arg");
    AddSensorDataToFile(arg...);
}

// bool AddSensorDataToFile( int32_t left_encode, int32_t right_encode )
// {
//     if ( !is_opend )
//     {
//         is_opend   = true;
//         collect_fd = fopen( collect_path.c_str(), "w+" );
//         if ( collect_fd == nullptr ) std::abort();
//     }

//     std::string tmp = "Sensor Data : ";
//     tmp += std::to_string( left_encode ) + "   ";
//     tmp += std::to_string( right_encode ) + "   ";

//     tmp += "\r\n";
//     spdlog::info( "tmp_size:{}", tmp );
//     fwrite( tmp.c_str(), sizeof( char ), tmp.size(), collect_fd );
// }

}  // namespace CollectSensorData
