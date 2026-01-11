#include "concurrent_logger.h"

ConcurrentLogger::ConcurrentLogger(const std::string& file, size_t capacity)
    : maxSize(capacity), running(true)
{
    out.open(file, std::ios::app);
    worker = std::thread(&ConcurrentLogger::process, this); //only single consumer thread
    //the logger must be ready BEFORE any producer can log.
}

ConcurrentLogger::~ConcurrentLogger() {
    stop();
}

void ConcurrentLogger::log(const std::string& msg) {
    std::unique_lock<std::mutex> lock(mtx);

    // wait if buffer full
    while (buffer.size() >= maxSize && running) {
        cv.wait(lock);
    }

    buffer.push(msg);
    cv.notify_one();   // wake consumer
}

void ConcurrentLogger::stop() {
    running = false;
    cv.notify_all();
    if (worker.joinable())
        worker.join();
    out.close();
}

void ConcurrentLogger::process() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // wait if buffer empty
        while (buffer.empty() && running) {
            cv.wait(lock);
        }

        if (!running && buffer.empty())
            break;

        std::string msg = buffer.front();
        buffer.pop();
        cv.notify_one();   // wake producer

        lock.unlock();
        out << msg << std::endl;
    }
}
