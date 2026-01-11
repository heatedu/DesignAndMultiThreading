#pragma once
#include "IRateLimiter.h"
#include <mutex>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <chrono>

/**
 * Token Bucket algorithm implementation.
 * Supports both global and per-user rate limiting with automatic token refilling.
 */
class TokenBucketStrategy : public IRateLimiter {
private:
    /**
     * Inner class representing an individual token bucket.
     * Thread-safe using mutex.
     */
    class Bucket 
    {
    private:
        int tokens;
        mutable std::mutex lock;

    public:
        explicit Bucket(int initialTokens) : tokens(initialTokens) {}

        /**
         * Attempts to consume one token.
         * @return true if a token was consumed, false otherwise.
         */
        bool tryConsume() {
            std::lock_guard<std::mutex> guard(lock);
            if (tokens > 0) {
                tokens--;
                return true;
            }
            return false;
        }

        /**
         * Refills the bucket by adding tokens without exceeding capacity.
         */
        void refill(int refreshRate, int bucketCapacity) {
            std::lock_guard<std::mutex> guard(lock);
            tokens = std::min(bucketCapacity, tokens + refreshRate);
        }
    };

    int bucketCapacity;
    std::atomic<int> refreshRate;
    std::unique_ptr<Bucket> globalBucket;
    std::unordered_map<std::string, std::unique_ptr<Bucket>> userBuckets;
    mutable std::mutex mapMutex;  // Protects userBuckets map
    std::thread refillThread;
    std::atomic<bool> running;
    static const int REFILL_INTERVAL_MS = 1000;  // 1 second

    /**
     * Background task that refills tokens periodically.
     */
    void refillTask() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(REFILL_INTERVAL_MS));
            
            // Refill global bucket
            globalBucket->refill(refreshRate, bucketCapacity);
            
            // Refill all user buckets
            std::lock_guard<std::mutex> guard(mapMutex);
            for (auto& pair : userBuckets) {
                pair.second->refill(refreshRate, bucketCapacity);
            }
        }
    }

public:
    TokenBucketStrategy(int capacity, int tokensPerSecond)
        : bucketCapacity(capacity), refreshRate(tokensPerSecond), running(true) {
        globalBucket = std::make_unique<Bucket>(capacity);
        refillThread = std::thread(&TokenBucketStrategy::refillTask, this);
    }

    ~TokenBucketStrategy() override {
        shutdown();
    }

    bool giveAccess(const std::string& rateLimitKey) override {
        if (!rateLimitKey.empty()) {
            // Per-user bucket
            std::lock_guard<std::mutex> guard(mapMutex);
            auto it = userBuckets.find(rateLimitKey);
            if (it == userBuckets.end()) {
                userBuckets[rateLimitKey] = std::make_unique<Bucket>(bucketCapacity);
                return userBuckets[rateLimitKey]->tryConsume();
            }
            return it->second->tryConsume();
        } else {
            // Global bucket
            return globalBucket->tryConsume();
        }
    }

    void updateConfiguration(const std::unordered_map<std::string, int>& config) override {
        auto it = config.find("refreshRate");
        if (it != config.end()) {
            refreshRate = it->second;
        }
    }

    void shutdown() override {
        if (running) {
            running = false;
            if (refillThread.joinable()) {
                refillThread.join();
            }
        }
    }
};

