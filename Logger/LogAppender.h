#pragma once
#include "LogMessage.h"
#include <string>
#include <fstream>

// Strategy Pattern: Interface for different log appending strategies
class LogAppender {
public:
    virtual ~LogAppender() = default;
    virtual void append(const LogMessage& logMessage) = 0;
};

// Console Appender - writes to stdout
class ConsoleAppender : public LogAppender {
public:
    void append(const LogMessage& logMessage) override;
};

// File Appender - writes to a file
class FileAppender : public LogAppender {
private:
    std::string filePath;
    
public:
    explicit FileAppender(const std::string& filePath);
    void append(const LogMessage& logMessage) override;
};
