#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>

// Models
#include "include/enums/BookingStatus.h"
#include "include/enums/SeatCategory.h"
#include "include/models/Movie.h"
#include "include/models/Seat.h"
#include "include/models/Screen.h"
#include "include/models/Show.h"
#include "include/models/Theatre.h"
#include "include/models/User.h"
#include "include/models/Booking.h"
#include "include/models/SeatLock.h"

// Strategies (Strategy Pattern)
#include "include/strategies/ISeatLockStrategy.h"
#include "include/strategies/InMemoryLockStrategy.h"
#include "include/strategies/PaymentStrategy.h"
#include "include/strategies/DebitCardStrategy.h"
#include "include/strategies/UpiStrategy.h"

// Managers (Simplified - merged controllers + services)
#include "include/managers/TheatreManager.h"
#include "include/managers/ShowManager.h"
#include "include/managers/BookingManager.h"
#include "include/managers/PaymentManager.h"

std::string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::CREATED: return "CREATED";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::EXPIRED: return "EXPIRED";
        default: return "UNKNOWN";
    }
}

std::string seatCategoryToString(SeatCategory category) {
    switch (category) {
        case SeatCategory::SILVER: return "SILVER";
        case SeatCategory::GOLD: return "GOLD";
        case SeatCategory::PLATINUM: return "PLATINUM";
        default: return "UNKNOWN";
    }
}

void printVector(const std::vector<int>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]";
}

int main() {
    try {
        std::cout << "=== BookMyShow System - Simplified Design ===" << std::endl;
        std::cout << std::endl;

        // Initialize Managers (merged controllers + services)
        auto theatreManager = std::make_shared<TheatreManager>();
        auto showManager = std::make_shared<ShowManager>();
        
        // Strategy Pattern: Lock strategy with 10-minute timeout (600 seconds)
        auto lockStrategy = std::make_shared<InMemoryLockStrategy>(600);
        
        // Initialize booking manager with lock strategy
        auto bookingManager = std::make_shared<BookingManager>(lockStrategy);
        
        // Strategy Pattern: Payment strategy (can swap with UpiStrategy)
        auto paymentStrategy = std::make_shared<DebitCardStrategy>();
        auto paymentManager = std::make_shared<PaymentManager>(paymentStrategy, bookingManager);
        
        // Step 1: Create a theatre
        std::cout << "Creating a new theatre..." << std::endl;
        int theatreId = theatreManager->createTheatre("PVR Cinemas");
        std::cout << "Theatre created with ID: " << theatreId << std::endl;
        std::cout << std::endl;
        
        // Step 2: Create a screen in the theatre
        std::cout << "Creating a new screen..." << std::endl;
        int screenId = theatreManager->createScreen("Screen 1", theatreId);
        std::cout << "Screen created with ID: " << screenId << std::endl;
        std::cout << std::endl;
        
        // Step 3: Create seats in the screen
        std::cout << "Creating seats..." << std::endl;
        // Create 5 rows with 10 seats each
        for (int row = 1; row <= 5; row++) {
            SeatCategory category;
            if (row == 1) {
                category = SeatCategory::PLATINUM; // First row is premium
            } else if (row <= 3) {
                category = SeatCategory::GOLD;     // Next two rows are gold
            } else {
                category = SeatCategory::SILVER;   // Rest are silver
            }
            
            for (int seatNum = 1; seatNum <= 10; seatNum++) {
                int seatId = theatreManager->createSeat(row, category, screenId);
                std::cout << "Created seat at row " << row << " with ID: " << seatId 
                         << " and category: " << seatCategoryToString(category) << std::endl;
            }
        }
        std::cout << std::endl;
        
        // Step 4: Create a movie
        std::cout << "Creating a new movie..." << std::endl;
        int movieId = showManager->createMovie("Inception", 150);
        std::cout << "Movie created with ID: " << movieId << std::endl;
        std::cout << std::endl;
        
        // Step 5: Create a show
        std::cout << "Creating a new show..." << std::endl;
        auto showTime = std::chrono::system_clock::now();
        auto screen = theatreManager->getScreen(screenId);
        int showId = showManager->createShow(movieId, screen, showTime, 150);
        std::cout << "Show created with ID: " << showId << std::endl;
        std::cout << std::endl;
        
        // Step 6: Get available seats for the show
        std::cout << "Checking available seats..." << std::endl;
        auto show = showManager->getShow(showId);
        auto availableSeats = bookingManager->getAvailableSeats(show);
        std::vector<int> availableIds;
        for (const auto& seat : availableSeats) {
            availableIds.push_back(seat->getSeatId());
        }
        std::cout << "Available seats: ";
        printVector(availableIds);
        std::cout << std::endl;
        std::cout << "Total available seats: " << availableSeats.size() << std::endl;
        std::cout << std::endl;
        
        // Step 7: Create a user
        std::cout << "Creating a user..." << std::endl;
        User user("John Doe", "john.doe@example.com");
        std::cout << "User created: " << user.getUserName() 
                 << " with email: " << user.getUserEmail() << std::endl;
        std::cout << std::endl;
        
        // Step 8: Book tickets sequentially
        std::cout << "Sequential booking of seats 1, 2, 3..." << std::endl;
        std::vector<std::shared_ptr<Seat>> seatsToBook;
        seatsToBook.push_back(theatreManager->getSeat(1));
        seatsToBook.push_back(theatreManager->getSeat(2));
        seatsToBook.push_back(theatreManager->getSeat(3));
        std::string bookingId = bookingManager->createBooking(user, show, seatsToBook);
        std::cout << "Booking created with ID: " << bookingId << std::endl;
        std::cout << std::endl;
        
        // Step 9: Process payment for the booking
        std::cout << "Processing payment..." << std::endl;
        paymentManager->processPayment(bookingId, user);
        std::cout << std::endl;
        
        // Step 10: Verify booking status
        auto booking = bookingManager->getBooking(bookingId);
        std::cout << "Booking status: " << bookingStatusToString(booking->getBookingStatus()) << std::endl;
        std::cout << "Is booking confirmed? " << (booking->isConfirmed() ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
        
        // Step 11: Check available seats again after booking
        std::cout << "Checking available seats after booking..." << std::endl;
        availableSeats = bookingManager->getAvailableSeats(show);
        availableIds.clear();
        for (const auto& seat : availableSeats) {
            availableIds.push_back(seat->getSeatId());
        }
        std::cout << "Available seats: ";
        printVector(availableIds);
        std::cout << std::endl;
        std::cout << "Total available seats: " << availableSeats.size() << std::endl;
        std::cout << std::endl;
        
        // ------------------------------
        // CONCURRENT BOOKING SIMULATION (Multithreading Pattern)
        // ------------------------------
        std::cout << "=== Simulating concurrent booking attempts ===" << std::endl;
        std::cout << std::endl;
        
        std::thread t1([&]() {
            try {
                // User 1 (John Doe) trying to book seats 5, 6, 7
                std::vector<std::shared_ptr<Seat>> seats1;
                seats1.push_back(theatreManager->getSeat(5));
                seats1.push_back(theatreManager->getSeat(6));
                seats1.push_back(theatreManager->getSeat(7));
                std::string bookingIdT1 = bookingManager->createBooking(user, show, seats1);
                std::cout << "User1 booking (seats 5,6,7) succeeded with Booking ID: " 
                         << bookingIdT1 << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "User1 booking (seats 5,6,7) failed: " << e.what() << std::endl;
            }
        });
        
        std::thread t2([&]() {
            try {
                // User 2 trying to book seats 7, 8, 9 (seat 7 overlaps with User1's attempt)
                User user2("Jane Doe", "jane.doe@example.com");
                std::vector<std::shared_ptr<Seat>> seats2;
                seats2.push_back(theatreManager->getSeat(7));
                seats2.push_back(theatreManager->getSeat(8));
                seats2.push_back(theatreManager->getSeat(9));
                std::string bookingIdT2 = bookingManager->createBooking(user2, show, seats2);
                std::cout << "User2 booking (seats 7,8,9) succeeded with Booking ID: " 
                         << bookingIdT2 << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "User2 booking (seats 7,8,9) failed: " << e.what() << std::endl;
            }
        });
        
        t1.join();
        t2.join();
        
        std::cout << std::endl;
        
        // Final available seats after concurrent attempts
        std::cout << "Final available seats after concurrent booking attempts: ";
        availableSeats = bookingManager->getAvailableSeats(show);
        availableIds.clear();
        for (const auto& seat : availableSeats) {
            availableIds.push_back(seat->getSeatId());
        }
        printVector(availableIds);
        std::cout << std::endl;
        std::cout << "Total available seats: " << availableSeats.size() << std::endl;
        std::cout << std::endl;
        
        std::cout << "=== BookMyShow System Demo Completed Successfully ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
