#pragma once
#include <string>
#include <unordered_map>
#include <memory>

/**
 * Interface for all rate limiting strategies.
 * New strategies can be added by implementing this interface.
 */
class IRateLimiter {
public:
    virtual ~IRateLimiter() = default;

    /**
     * Determines if a request should be allowed.
     * @param rateLimitKey: Key for per-user limiting; empty string for global.
     * @return true if allowed, false if blocked.
     */
    virtual bool giveAccess(const std::string& rateLimitKey) = 0;

    /**
     * Updates configuration dynamically.
     * @param config Map of configuration options.
     */
    virtual void updateConfiguration(const std::unordered_map<std::string, int>& config) = 0;

    /**
     * Cleanly shuts down the rate limiter.
     */
    virtual void shutdown() = 0;
};

