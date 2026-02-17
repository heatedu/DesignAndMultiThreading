#include "../include/LoggerConfig.h"

LoggerConfig::LoggerConfig(LogLevel level, std::shared_ptr<LogAppender> appender)
    : logLevel(level), logAppender(appender) {}
