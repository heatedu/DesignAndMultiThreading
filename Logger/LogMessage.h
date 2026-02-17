#pragma once
#include "LogLevel.h"
#include <string>
#include <chrono>

class LogMessage {
private:
    LogLevel level;
    std::string message;
    long long timestamp;

public:
    LogMessage(LogLevel level, const std::string& message);
    
    LogLevel getLevel() const { return level; }
    std::string getMessage() const { return message; }
    long long getTimestamp() const { return timestamp; }
    std::string toString() const;
};
