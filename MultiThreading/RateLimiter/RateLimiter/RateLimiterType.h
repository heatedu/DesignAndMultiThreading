#pragma once

/**
 * Enumeration of supported rate limiter types.
 */
enum class RateLimiterType {
    TOKEN_BUCKET,
    FIXED_WINDOW,
    SLIDING_WINDOW,
    LEAKY_BUCKET
};

