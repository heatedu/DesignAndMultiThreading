#pragma once
#include "LoggerConfig.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>

// Singleton Pattern: Ensures single instance of Logger
class Logger {
private:
    /*
        Hash map storing all logger instances
        Key: String (logLevel + appender address)
        Value: shared_ptr to Logger
        Why map? Support multiple configurations (e.g., INFO+Console, ERROR+File)
    */
    static std::unordered_map<std::string, std::shared_ptr<Logger>> instances;
    static std::mutex mutex_;
    
    std::shared_ptr<LoggerConfig> config;
    std::mutex configMutex;
    
    // Private constructor
    Logger(LogLevel logLevel, std::shared_ptr<LogAppender> logAppender);

public:
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    static std::shared_ptr<Logger> getInstance(LogLevel logLevel, std::shared_ptr<LogAppender> logAppender);
    
    void setConfig(std::shared_ptr<LoggerConfig> newConfig);// Change logger configuration at runtime
    void log(LogLevel level, const std::string& message);
    
    // Convenience methods
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
};
