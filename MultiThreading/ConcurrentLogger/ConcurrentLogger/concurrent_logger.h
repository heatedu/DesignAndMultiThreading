#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

class ConcurrentLogger {
public:
    ConcurrentLogger(const std::string& file, size_t capacity = 1024);
    ~ConcurrentLogger();

    void log(const std::string& msg);
    void stop();

private:
    void process();

    std::ofstream out;
    std::queue<std::string> buffer;
    size_t maxSize;

    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::thread worker;
};
