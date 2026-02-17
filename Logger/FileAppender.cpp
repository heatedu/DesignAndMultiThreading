#include "../include/LogAppender.h"
#include <iostream>

FileAppender::FileAppender(const std::string& filePath) : filePath(filePath) {}

void FileAppender::append(const LogMessage& logMessage) {
    std::ofstream file(filePath, std::ios::app);
    if (file.is_open()) {
        file << logMessage.toString() << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
}
