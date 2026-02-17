#pragma once
#include "LogLevel.h"
#include "LogAppender.h"
#include <memory>
//bundles level + appender together
//Allows runtime reconfiguration:
//logger->setConfig(new LoggerConfig(LogLevel::DEBUG, fileAppender));
class LoggerConfig {
private:
    LogLevel logLevel;
    std::shared_ptr<LogAppender> logAppender;

public:
    LoggerConfig(LogLevel level, std::shared_ptr<LogAppender> appender);
    
    LogLevel getLogLevel() const { return logLevel; }
    void setLogLevel(LogLevel level) { logLevel = level; }
    
    std::shared_ptr<LogAppender> getLogAppender() const { return logAppender; }
    void setLogAppender(std::shared_ptr<LogAppender> appender) { logAppender = appender; }
};
