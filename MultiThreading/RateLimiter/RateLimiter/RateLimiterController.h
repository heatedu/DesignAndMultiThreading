#pragma once
#include "IRateLimiter.h"
#include "RateLimiterFactory.h"
#include <future>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

/**
 * Controller that processes requests using a rate limiter.
 * Provides asynchronous request handling using thread pool.
 */
class RateLimiterController {
private:
    std::unique_ptr<IRateLimiter> rateLimiter;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    /**
     * Worker thread function that processes tasks from queue.
     */
    void workerThread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                
                if (stop && tasks.empty()) {
                    return;
                }
                
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

public:
    /**
     * Constructor that creates the rate limiter and thread pool.
     * @param type Rate limiter type.
     * @param config Configuration parameters.
     * @param threadCount Number of worker threads (default 10).
     */
    RateLimiterController(
        RateLimiterType type, 
        const std::unordered_map<std::string, int>& config,
        size_t threadCount = 10) 
        : stop(false) 
    {
        
        rateLimiter = RateLimiterFactory::createLimiter(type, config);
        
        // Create worker threads
        for (size_t i = 0; i < threadCount; ++i) 
        {
            workers.emplace_back(&RateLimiterController::workerThread, this);
        }
    }

    ~RateLimiterController() {
        shutdown();
    }

    /**
     * Processes a request asynchronously.
     * @param rateLimitKey Key for per-user limiting; empty for global.
     * @return Future that resolves to true if allowed, false if blocked.
     */
    std::future<bool> processRequest(const std::string& rateLimitKey) {
        auto promise = std::make_shared<std::promise<bool>>();
        auto future = promise->get_future();
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.push([this, rateLimitKey, promise]() {
                // THIS is what task() executes:
                bool allowed = rateLimiter->giveAccess(rateLimitKey); // ← Token check HERE
                if (allowed) {
                    std::cout << "Request with key [" << (rateLimitKey.empty() ? "global" : rateLimitKey) 
                              << "]: ✅ Allowed" << std::endl;
                } else {
                    std::cout << "Request with key [" << (rateLimitKey.empty() ? "global" : rateLimitKey) 
                              << "]: ❌ Blocked" << std::endl;
                }
                promise->set_value(allowed); // ← Tell caller: allowed or blocked
            });
        }
        condition.notify_one();
        
        return future;
    }

    /**
     * Updates the rate limiter configuration.
     * @param config Configuration options.
     */
    void updateConfiguration(const std::unordered_map<std::string, int>& config) {
        rateLimiter->updateConfiguration(config);
    }

    /**
     * Shuts down the controller and cleans up resources.
     */
    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (stop) return;  // Already shut down
            stop = true;
        }
        condition.notify_all();
        
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        
        if (rateLimiter) {
            rateLimiter->shutdown();
        }
    }
};

