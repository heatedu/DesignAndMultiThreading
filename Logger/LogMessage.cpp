#include "../include/LogMessage.h"
#include <sstream>

LogMessage::LogMessage(LogLevel level, const std::string& message) 
    : level(level), message(message) {
    timestamp = std::chrono::system_clock::now().time_since_epoch().count();
}

std::string LogMessage::toString() const {
    std::stringstream ss;
    ss << "[" << logLevelToString(level) << "] " << timestamp << " - " << message;
    return ss.str();
}
