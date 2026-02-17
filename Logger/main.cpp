#include "../include/Logger.h"
#include "../include/LogHandler.h"
#include <iostream>
#include <memory>

// Helper function to build the chain of handlers
std::shared_ptr<LogHandler> getChainOfLoggers(std::shared_ptr<LogAppender> appender) {
    auto errorLogger = std::make_shared<ErrorLogger>(LogLevel::ERROR, appender);
    auto debugLogger = std::make_shared<DebugLogger>(LogLevel::DEBUG, appender);
    auto infoLogger = std::make_shared<InfoLogger>(LogLevel::INFO, appender);
    
    infoLogger->setNextHandler(debugLogger);
    debugLogger->setNextHandler(errorLogger);
    
    return infoLogger;
}

int main() {
    std::cout << "=== Logging System Demo ===" << std::endl << std::endl;
    
    // 1. Demonstrate Chain of Responsibility Pattern
    std::cout << "--- Chain of Responsibility Pattern ---" << std::endl;
    auto consoleAppender = std::make_shared<ConsoleAppender>();
    auto loggerChain = getChainOfLoggers(consoleAppender);
    
    std::cout << "\nLogging INFO level message:" << std::endl;
    loggerChain->logMessage(LogLevel::INFO, "This is an information message.");
    
    std::cout << "\nLogging DEBUG level message:" << std::endl;
    loggerChain->logMessage(LogLevel::DEBUG, "This is a debug level message.");
    
    std::cout << "\nLogging ERROR level message:" << std::endl;
    loggerChain->logMessage(LogLevel::ERROR, "This is an error message.");
    
    // 2. Demonstrate Strategy Pattern with different appenders
    std::cout << "\n\n--- Strategy Pattern (File Appender) ---" << std::endl;
    auto fileAppender = std::make_shared<FileAppender>("logs.txt");
    auto fileLoggerChain = getChainOfLoggers(fileAppender);
    
    std::cout << "Logging to file..." << std::endl;
    fileLoggerChain->logMessage(LogLevel::INFO, "File: Information message");
    fileLoggerChain->logMessage(LogLevel::WARNING, "File: Warning message");
    fileLoggerChain->logMessage(LogLevel::ERROR, "File: Error message");
    std::cout << "Check logs.txt for output" << std::endl;
    
    // 3. Demonstrate Singleton Pattern
    std::cout << "\n\n--- Singleton Pattern ---" << std::endl;
    auto logger1 = Logger::getInstance(LogLevel::DEBUG, consoleAppender);
    logger1->info("Singleton Logger: Info message");
    logger1->debug("Singleton Logger: Debug message");
    logger1->error("Singleton Logger: Error message");
    
    std::cout << "\nSwitching appender to file..." << std::endl;
    auto newConfig = std::make_shared<LoggerConfig>(LogLevel::INFO, fileAppender);
    logger1->setConfig(newConfig);
    logger1->warning("Singleton Logger: Warning to file");
    logger1->fatal("Singleton Logger: Fatal to file");
    std::cout << "Check logs.txt for additional output" << std::endl;
    
    // 4. Demonstrate multiple log levels
    std::cout << "\n\n--- All Log Levels ---" << std::endl;
    auto logger2 = Logger::getInstance(LogLevel::DEBUG, consoleAppender);
    logger2->debug("This is a DEBUG message");
    logger2->info("This is an INFO message");
    logger2->warning("This is a WARNING message");
    logger2->error("This is an ERROR message");
    logger2->fatal("This is a FATAL message");
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}
