#pragma once
#include "IRateLimiter.h"
#include "RateLimiterType.h"
#include "TokenBucketStrategy.h"
#include <unordered_map>
#include <memory>
#include <stdexcept>

/**
 * ============================================================================
 * Simple Factory Pattern - Creates rate limiter instances
 * ============================================================================
 * 
 * Usage:
 *   auto limiter = RateLimiterFactory::createLimiter(
 *       RateLimiterType::TOKEN_BUCKET,
 *       {{"capacity", 5}, {"refreshRate", 1}}
 *   );
 */
class RateLimiterFactory {
private:
    // Helper: Get config value with default
    static int getConfigValue(
        const std::unordered_map<std::string, int>& config,
        const std::string& key,
        int defaultValue
    ) {
        auto it = config.find(key);
        return (it != config.end()) ? it->second : defaultValue;
    }

public:
    /**
     * Creates a rate limiter based on type.
     * Simple switch-based factory - easy to understand!
     * 
     * @param type Type of rate limiter (TOKEN_BUCKET, FIXED_WINDOW, etc.)
     * @param config Configuration map with parameters
     * @return Newly created rate limiter instance
     */
    static std::unique_ptr<IRateLimiter> createLimiter(
        RateLimiterType type, 
        const std::unordered_map<std::string, int>& config
    ) {
        // Simple switch statement - classic factory pattern!
        switch (type) {
            
            case RateLimiterType::TOKEN_BUCKET: {
                // Extract configuration
                int capacity = getConfigValue(config, "capacity", 10);
                int refreshRate = getConfigValue(config, "refreshRate", 10);
                
                // Support alternative config key
                if (config.find("refreshRate") == config.end()) {
                    refreshRate = getConfigValue(config, "tokensPerSecond", 10);
                }
                
                // Create and return
                return std::make_unique<TokenBucketStrategy>(capacity, refreshRate);
            }
            
            // Add more cases here for other algorithms
            // case RateLimiterType::FIXED_WINDOW: {
            //     int maxRequests = getConfigValue(config, "maxRequests", 100);
            //     int windowSize = getConfigValue(config, "windowSize", 60);
            //     return std::make_unique<FixedWindowStrategy>(maxRequests, windowSize);
            // }
            
            // case RateLimiterType::SLIDING_WINDOW: {
            //     ...
            //     return std::make_unique<SlidingWindowStrategy>(...);
            // }
            
            default:
                throw std::invalid_argument("Unsupported rate limiter type");
        }
    }
};

