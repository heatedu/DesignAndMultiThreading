#pragma once
#include "LogMessage.h"
#include "LogAppender.h"
#include <memory>

// Chain of Responsibility Pattern: Abstract base class for log handlers
class LogHandler {
protected:
    LogLevel level;
    std::shared_ptr<LogHandler> nextHandler;
    std::shared_ptr<LogAppender> appender;

public:
    LogHandler(LogLevel level, std::shared_ptr<LogAppender> appender);
    virtual ~LogHandler() = default;
    
    void setNextHandler(std::shared_ptr<LogHandler> next);
    void logMessage(LogLevel level, const std::string& message);
    
protected:
    virtual void write(const std::string& message) = 0;
};

// Concrete Handlers
class InfoLogger : public LogHandler {
public:
    InfoLogger(LogLevel level, std::shared_ptr<LogAppender> appender);
    
protected:
    void write(const std::string& message) override;
};

class DebugLogger : public LogHandler {
public:
    DebugLogger(LogLevel level, std::shared_ptr<LogAppender> appender);
    
protected:
    void write(const std::string& message) override;
};

class ErrorLogger : public LogHandler {
public:
    ErrorLogger(LogLevel level, std::shared_ptr<LogAppender> appender);
    
protected:
    void write(const std::string& message) override;
};
