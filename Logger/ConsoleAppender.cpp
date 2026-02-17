#include "../include/LogAppender.h"
#include <iostream>

void ConsoleAppender::append(const LogMessage& logMessage) {
    std::cout << logMessage.toString() << std::endl;
}
