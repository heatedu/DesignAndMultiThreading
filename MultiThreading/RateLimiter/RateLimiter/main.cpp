#include "RateLimiterController.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void sendBurstRequests(RateLimiterController& controller, int count, const std::string& rateLimitKey) {
    // Rate limiter to test
// Number of requests to send
// User key (or "" for global)
    std::vector<std::future<bool>> futures;
    
    for (int i = 0; i < count; ++i) {
        futures.push_back(controller.processRequest(rateLimitKey));
    }
    
    // Wait for all to complete and count results
    int allowed = 0;
    for (auto& future : futures) {
        if (future.get()) {
            allowed++;
        }
    }
    
    std::cout << "Results: " << allowed << " allowed, " << (count - allowed) 
              << " blocked (total: " << count << ")" << std::endl;
}

void demonstrateRateLimiting() {
    // Configure token bucket: 5 tokens max, 1 token per second refill
    std::unordered_map<std::string, int> config;
    config["capacity"] = 5;
    config["refreshRate"] = 1;
    
    RateLimiterController controller(RateLimiterType::TOKEN_BUCKET, config, 10);
    
    // EXAMPLE 1: Global rate limiting - Burst of requests
    std::cout << "\n=== EXAMPLE 1: Global rate limiting - Burst of requests ===" << std::endl;
    sendBurstRequests(controller, 10, "");
    
    // EXAMPLE 2: After waiting for refill
    std::cout << "\n=== EXAMPLE 2: Global rate limiting - After waiting for tokens refill ===" << std::endl;
    std::cout << "Waiting 5 seconds for tokens to refill..." << std::endl;
    sleep(5000);
    sendBurstRequests(controller, 10, "");
    
    // EXAMPLE 3: Per-user rate limiting
    std::vector<std::string> users = {"user1", "user2", "user3"};
    for (const auto& user : users) {
        std::cout << "\nRequests for " << user << ":" << std::endl;
        sendBurstRequests(controller, 7, user);
    }
    
    // EXAMPLE 4: High concurrency scenario
    std::cout << "\n=== EXAMPLE 4: High concurrency scenario ===" << std::endl;
    std::vector<std::future<bool>> futures;
    for (int i = 0; i < 20; ++i) {
        futures.push_back(controller.processRequest(""));
    }
    
    int allowed = 0;
    for (auto& future : futures) {
        if (future.get()) {
            allowed++;
        }
    }
    
    std::cout << "High concurrency results: " << allowed << " allowed, " 
              << (20 - allowed) << " blocked" << std::endl;
    
    controller.shutdown();
}

int main() {
    std::cout << "=== Rate Limiter System - Token Bucket Implementation ===" << std::endl;
    demonstrateRateLimiting();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}

