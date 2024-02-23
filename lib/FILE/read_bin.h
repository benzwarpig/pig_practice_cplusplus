#pragma once

#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define BLOCK_SIZE_256B 256
#define BLOCK_SIZE_1024B 1024
#define BLOCK_SIZE_2048B 2048

#define BLOCK_SIZE BLOCK_SIZE_2048B
constexpr auto *kBinFilePath =
    "/home/pig/wxc/PracticeCPlusPlus/bin/value_robot.bin";

class ReadFile {
    enum class ReadFileErrorCode {
        kNoError = (0x00),

    } ErrorType;

private:
    static constexpr long int kTempBufferSize = BLOCK_SIZE;

    static constexpr auto *kFilePath = kBinFilePath;

    int sub_package;

    // RAII
    int file_fd;

    std::vector<std::array<uint8_t, kTempBufferSize>> file_data;

public:
    int GetBinaryFileSize();

    ReadFile::ReadFileErrorCode ReadBinaryFlieAndSubpackage();

    void ShowTheFile();

public:
    ReadFile() {
        file_fd = open(kFilePath, O_RDONLY);
        if (file_fd < 0) {
            spdlog::error("Open File Error");
        }
    };
    ~ReadFile() { close(file_fd); }

private:
    ReadFile(const ReadFile &);
    ReadFile &operator=(const ReadFile &);
};

int ReadFile::GetBinaryFileSize() {
    struct stat file_stat;

    if (stat(kFilePath, &file_stat) < 0) {
        spdlog::error("get file_stat error");
    } else {
        sub_package =
            std::ceil(static_cast<float>(file_stat.st_size) / kTempBufferSize);
        spdlog::info("the file size is :{},subpackage :{}", file_stat.st_size,
                     sub_package);
    }
    return file_stat.st_size;
}

ReadFile::ReadFileErrorCode ReadFile::ReadBinaryFlieAndSubpackage() {
    do {
        std::array<uint8_t, kTempBufferSize> tmp;
        std::fill_n(tmp.begin(), kTempBufferSize, 0xff);
        auto size = read(file_fd, tmp.data(), kTempBufferSize);
        if (size == kTempBufferSize) {
            file_data.push_back(tmp);
        } else if (size == 0 || size == -1) {
            break;
        } else {
            file_data.push_back(tmp);
            //  < kTempBufferSize reserve
        }

    } while (true);

    return ReadFileErrorCode::kNoError;
}

void ReadFile::ShowTheFile() {
    for (auto &tmp : file_data) {
        for (auto &tmp_ : tmp) {
            printf("0x%2x  ", tmp_);
        }
        printf("\r\n");
    }
}
