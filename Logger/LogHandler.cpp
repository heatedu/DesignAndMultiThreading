#include "../include/LogHandler.h"
#include <iostream>

LogHandler::LogHandler(LogLevel level, std::shared_ptr<LogAppender> appender)
    : level(level), appender(appender), nextHandler(nullptr) {}

void LogHandler::setNextHandler(std::shared_ptr<LogHandler> next) {
    nextHandler = next;
}

void LogHandler::logMessage(LogLevel msgLevel, const std::string& message) {
    if (static_cast<int>(this->level) <= static_cast<int>(msgLevel))//The handler processes messages at its level OR HIGHER severity. 
    {
        LogMessage logMsg(msgLevel, message);
        if (appender) {
            appender->append(logMsg);
        }
        write(message);
    }
    
    if (nextHandler) {
        nextHandler->logMessage(msgLevel, message);
    }
}

// InfoLogger Implementation
InfoLogger::InfoLogger(LogLevel level, std::shared_ptr<LogAppender> appender)
    : LogHandler(level, appender) {}

void InfoLogger::write(const std::string& message) {
    // Additional processing specific to INFO logs (if needed)
}

// DebugLogger Implementation
DebugLogger::DebugLogger(LogLevel level, std::shared_ptr<LogAppender> appender)
    : LogHandler(level, appender) {}

void DebugLogger::write(const std::string& message) {
    // Additional processing specific to DEBUG logs (if needed)
}

// ErrorLogger Implementation
ErrorLogger::ErrorLogger(LogLevel level, std::shared_ptr<LogAppender> appender)
    : LogHandler(level, appender) {}

void ErrorLogger::write(const std::string& message) {
    // Additional processing specific to ERROR logs (if needed)
}
