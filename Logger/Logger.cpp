#include "../include/Logger.h"

std::unordered_map<std::string, std::shared_ptr<Logger>> Logger::instances;
std::mutex Logger::mutex_;

Logger::Logger(LogLevel logLevel, std::shared_ptr<LogAppender> logAppender) {
    //Minimum severity to log (e.g., INFO means ignore DEBUG)
    config = std::make_shared<LoggerConfig>(logLevel, logAppender);
}

std::shared_ptr<Logger> Logger::getInstance(LogLevel logLevel, std::shared_ptr<LogAppender> logAppender) {
    std::string key = std::to_string(static_cast<int>(logLevel)) + "_" + 
                      std::to_string(reinterpret_cast<uintptr_t>(logAppender.get()));
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (instances.find(key) == instances.end()) {
        instances[key] = std::shared_ptr<Logger>(new Logger(logLevel, logAppender));
    }
    
    return instances[key];
}

void Logger::setConfig(std::shared_ptr<LoggerConfig> newConfig) {
    std::lock_guard<std::mutex> lock(configMutex);
    config = newConfig;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(configMutex);
    
    if (static_cast<int>(level) >= static_cast<int>(config->getLogLevel())) {
        LogMessage logMessage(level, message);
        config->getLogAppender()->append(logMessage);
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}
