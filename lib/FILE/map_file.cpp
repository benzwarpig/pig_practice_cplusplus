//
// create by wxc on 2022/6/22
// for calibrate edge line laser
//

#ifndef LEDONG_CALIBRATION_H
#define LEDONG_CALIBRATION_H

#include <spdlog/spdlog.h>

#include <iostream>
#include <map>
#include <string>

// file
#include <fcntl.h>
#include <sys/stat.h>

#include "collect_sensor_data.h"

inline void GetCurrentFilePath(std::string& tmp, const std::string& cut_path) {
    // get current path and save in string
    char file_path[1024];
    auto res = readlink("/proc/self/exe", file_path, 1024);
    if (res == -1) {
        spdlog::error("can not get current path !");
        std::abort();
    }
    std::string path_tmp(file_path);

    // find the real project path and earse
    std::string::iterator tmp_iterator =
        path_tmp.begin() + path_tmp.find(cut_path);
    path_tmp.erase(tmp_iterator, path_tmp.end());

    // swap the pointer
    path_tmp.swap(tmp);
}

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:  // emphasize the following members are private
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
};

class CalibrateImpl : noncopyable {
    struct PointMsg {
        int index;
        double value;
    } __attribute__((packed));

public:
    // read config file and fill calibration map
    bool ReadConfigFile() {
        bool return_tmp = false;

        if (!is_open) {
            spdlog::error("read error");
            std::abort();
        }

        lseek(calibrate_fd, 0, SEEK_SET);
        std::array<uint8_t, kMaxBufferSize> tmp_;
        if (-1 == read(calibrate_fd, tmp_.data(), kMaxBufferSize)) {
            spdlog::error("read error");
            std::abort();
        }

        int offset_addr = 0;

        memcpy((char*)&is_calibration, (char*)(tmp_.data() + offset_addr),
               sizeof(bool));
        offset_addr += sizeof(bool);
        memcpy((char*)&points_num, (char*)(tmp_.data() + offset_addr),
               sizeof(uint8_t));
        offset_addr += sizeof(uint8_t);

        spdlog::info("is_calibration:{},points_num:{}", is_calibration,
                     points_num);

        if (is_calibration) {
            spdlog::info(
                "already calibrate edge line laser data , point num :{}",
                points_num);
            for (int i = 0; i < points_num; i++) {
                PointMsg msg;
                memcpy((char*)&msg, (char*)(tmp_.data() + offset_addr),
                       sizeof(PointMsg));

                int tmp_index = msg.index;
                double tmp_value = msg.value;
                calibrate_map.emplace(std::make_pair(tmp_index, tmp_value));

                offset_addr += sizeof(PointMsg);
            }
            for (auto& map_tmp : calibrate_map) {
                spdlog::info("read map , index : {} , value : {}",
                             map_tmp.first, map_tmp.second);
            }
            return_tmp = true;
        }

        return return_tmp;
    }

    bool WriteCalibrationData(std::map<int, double>& tmp) {
        bool return_tmp = true;

        if (!is_open) {
            spdlog::error("write error");
            std::abort();
        }

        is_calibration = true;
        points_num = tmp.size();

        calibrate_map.swap(tmp);

        int offset_addr = 0;
        std::array<uint8_t, kMaxBufferSize> tmp_;

        memcpy((char*)(tmp_.data() + offset_addr), (char*)&is_calibration,
               sizeof(bool));
        offset_addr += sizeof(bool);
        memcpy((char*)(tmp_.data() + offset_addr), (char*)&points_num,
               sizeof(uint8_t));
        offset_addr += sizeof(uint8_t);

        for (auto& map_tmp : calibrate_map) {
            PointMsg msg;
            msg.index = map_tmp.first;
            msg.value = map_tmp.second;

            memcpy((char*)(tmp_.data() + offset_addr), (char*)&msg,
                   sizeof(PointMsg));

            offset_addr += sizeof(PointMsg);

            spdlog::info(
                "write map , index : {} , value : {} , offset_addr : {}",
                map_tmp.first, map_tmp.second, offset_addr);
        }

        lseek(calibrate_fd, 0, SEEK_SET);
        if (-1 == write(calibrate_fd, (char*)tmp_.data(), kMaxBufferSize)) {
            return_tmp = false;

            spdlog::error("write error");
            std::abort();
        } else {
            spdlog::info("wirte calirate msg done,the points_num : {},{},{}",
                         points_num, calibrate_map.size(), tmp.size());
        }

        return return_tmp;
    }

    bool ClearTheFileMsg() {
        bool return_tmp = true;

        std::array<uint8_t, kMaxBufferSize> tmp_;
        memset((char*)tmp_.data(), 0, kMaxBufferSize);

        lseek(calibrate_fd, 0, SEEK_SET);
        if (-1 == write(calibrate_fd, (char*)tmp_.data(), kMaxBufferSize)) {
            return_tmp = false;
            spdlog::error("write error");
            std::abort();
        }
        close(calibrate_fd);

        return return_tmp;
    }

    int GetCalibrateFileFd() { return calibrate_fd; }

    bool GetIsCalibrate() { return is_calibration; }

    uint8_t GetPointsNum() { return points_num; }

    std::map<int, double>& GetCalibrateMap() { return calibrate_map; }

public:
    explicit CalibrateImpl(std::string path)
        : is_calibration(false), file_path(path), points_num(0) {
        calibrate_fd = open(file_path.c_str(), O_RDWR);  // | O_CREAT, 0600
        if (calibrate_fd == -1) {
            spdlog::info("calibration.txt not exist , creat it and fill zero");
            calibrate_fd = open(file_path.c_str(), O_RDWR | O_CREAT, 0600);

            ClearTheFileMsg();
        } else {
            spdlog::info("calibration.txt already exist");
        }
        is_open = true;
    }

    ~CalibrateImpl() noexcept {
        if (is_open) {
            is_open = false;
            close(calibrate_fd);
        }
    }

private:
    // RAII , manage the calibrate file
    int calibrate_fd;

    bool is_calibration;  // is calibration success or not

    bool is_open;  // for fd

    uint8_t points_num;

    std::map<int, double> calibrate_map;

    std::string file_path;

private:
    static constexpr int kMaxLinePointsNum = 160;
    static constexpr int kMaxBufferSize =
        kMaxLinePointsNum * sizeof(PointMsg) + 10;  // near 2000
};

int main(void) {
    std::string aim_path{0};
    std::string cut_path("map_file");

    GetCurrentFilePath(aim_path, cut_path);

    std::unique_ptr<CalibrateImpl> file_ptr(
        new CalibrateImpl(aim_path + "calibration.txt"));

    CollectSensorData::AddSensorDataToFile(100, 200, 300, 400);

    return 0;
}

#endif  // LEDONG_CALIBRATION_H
