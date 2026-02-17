# Logger System - C++ Implementation

## Design Patterns Implemented

1. **Chain of Responsibility Pattern** - Log handlers process messages in a chain
2. **Strategy Pattern** - Different log appenders (Console, File)
3. **Singleton Pattern** - Global logger instance with thread safety

## Project Structure

```
Logger/
├── include/           # Header files
│   ├── LogLevel.h
│   ├── LogMessage.h
│   ├── LogAppender.h
│   ├── LogHandler.h
│   ├── LoggerConfig.h
│   └── Logger.h
├── src/              # Implementation files
│   ├── LogMessage.cpp
│   ├── ConsoleAppender.cpp
│   ├── FileAppender.cpp
│   ├── LogHandler.cpp
│   ├── LoggerConfig.cpp
│   ├── Logger.cpp
│   └── main.cpp
├── Logger.sln        # Visual Studio Solution
└── Logger.vcxproj    # Visual Studio Project
```

## How to Build and Run

### In Visual Studio:
1. Open `Logger.sln` in Visual Studio
2. Build Solution (Ctrl+Shift+B)
3. Run (F5 or Ctrl+F5)

The executable will be in `bin\Debug\` or `bin\Release\`

## Key Components

### 1. LogLevel (Enum)
- DEBUG, INFO, WARNING, ERROR, FATAL severity levels

### 2. LogMessage
- Encapsulates log entry with level, message, timestamp

### 3. LogAppender (Strategy Pattern)
- **Interface**: LogAppender
- **Implementations**: ConsoleAppender, FileAppender

### 4. LogHandler (Chain of Responsibility)
- **Abstract**: LogHandler
- **Concrete**: InfoLogger, DebugLogger, ErrorLogger

### 5. Logger (Singleton)
- Thread-safe singleton instance
- Configurable at runtime

## Interview Discussion Points

- **Chain of Responsibility**: How handlers filter and process logs
- **Strategy Pattern**: Swapping output destinations at runtime
- **Singleton**: Thread-safe lazy initialization with mutex
- **Extensibility**: Adding new log levels or appenders
- **Performance**: Considerations for high-volume logging
