#ifndef VEHICLE_TYPES_H
#define VEHICLE_TYPES_H

// Enum for different vehicle types
enum class VehicleType {
    ECONOMY,
    LUXURY,
    SUV,
    SEDAN,
    HATCHBACK
};

// Enum for vehicle status lifecycle
enum class VehicleStatus {
    AVAILABLE,
    RESERVED,
    RENTED,
    MAINTENANCE,
    OUT_OF_SERVICE
};

// Enum for reservation status with proper state management
enum class ReservationStatus {
    PENDING,        // Initial state after creation
    CONFIRMED,      // After payment confirmation
    IN_PROGRESS,    // Vehicle picked up
    COMPLETED,      // Vehicle returned
    CANCELLED,      // Cancelled by user or system
    NO_SHOW         // User didn't show up for pickup
};

// Enum for payment types
enum class PaymentType {
    CREDIT_CARD,
    DEBIT_CARD,
    CASH,
    UPI,
    NET_BANKING
};

// Enum for payment status
enum class PaymentStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    FAILED,
    REFUNDED
};

#endif

