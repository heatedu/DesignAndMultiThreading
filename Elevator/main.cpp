#include "Building.h"
#include "ElevatorDisplay.h"
#include "ElevatorRequest.h"
#include "FCFSSchedulingStrategy.h"
#include "ScanSchedulingStrategy.h"
#include "LookSchedulingStrategy.h"
#include <iostream>
#include <memory>

void displayElevatorStatus(const std::vector<Elevator*>& elevators) {
    std::cout << "\n=== Elevator Status ===" << std::endl;
    for (auto elevator : elevators) {
        std::string dirStr;
        switch (elevator->getDirection()) {
            case Direction::UP: dirStr = "UP"; break;
            case Direction::DOWN: dirStr = "DOWN"; break;
            case Direction::IDLE: dirStr = "IDLE"; break;
        }
        
        std::string stateStr;
        switch (elevator->getState()) {
            case ElevatorState::IDLE: stateStr = "IDLE"; break;
            case ElevatorState::MOVING: stateStr = "MOVING"; break;
            case ElevatorState::STOPPED: stateStr = "STOPPED"; break;
            case ElevatorState::MAINTENANCE: stateStr = "MAINTENANCE"; break;
        }
        
        std::cout << "Elevator " << elevator->getId() 
                  << ": Floor " << elevator->getCurrentFloor()
                  << ", Direction " << dirStr
                  << ", State " << stateStr
                  << ", Destinations [";
        
        auto destinations = elevator->getDestinationFloors();
        for (size_t i = 0; i < destinations.size(); i++) {
            std::cout << destinations[i];
            if (i < destinations.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "======================\n" << std::endl;
}

int main() {
    // Initialize a building with 10 floors and 3 elevators
    Building building("Office Tower", 10, 3);
    ElevatorController* controller = building.getElevatorController();
    
    // Create an ElevatorDisplay to observe and display elevator events
    ElevatorDisplay display;
    for (auto elevator : controller->getElevators()) {
        elevator->addObserver(&display);
    }
    
    // Display simulation details and options
    std::cout << "========================================" << std::endl;
    std::cout << "   Elevator System Simulation" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Building: " << building.getName() << std::endl;
    std::cout << "Floors: " << building.getNumberOfFloors() << std::endl;
    std::cout << "Elevators: " << controller->getElevators().size() << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    bool running = true;
    
    while (running) {
        std::cout << "\n--- Select an option ---" << std::endl;
        std::cout << "1. Request elevator (external)" << std::endl;
        std::cout << "2. Request floor (internal)" << std::endl;
        std::cout << "3. Simulate next step" << std::endl;
        std::cout << "4. Change scheduling strategy" << std::endl;
        std::cout << "5. Exit simulation" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                // Handle external elevator request
                int externalElevatorId;
                std::cout << "Enter elevator ID: ";
                std::cin >> externalElevatorId;
                
                controller->setCurrentElevator(externalElevatorId);
                
                int floorNum;
                std::cout << "Enter floor number: ";
                std::cin >> floorNum;
                
                int dirChoice;
                std::cout << "Direction (1 for UP, 2 for DOWN): ";
                std::cin >> dirChoice;
                
                Direction dir = dirChoice == 1 ? Direction::UP : Direction::DOWN;
                
                // ✅ Use Command Pattern - Create command and execute it
                ElevatorRequest externalRequest(externalElevatorId, floorNum, false, dir, controller);
                externalRequest.execute();
                break;
            }
            
            case 2: {
                // Handle internal elevator floor request
                int elevatorId;
                std::cout << "Enter elevator ID: ";
                std::cin >> elevatorId;
                
                controller->setCurrentElevator(elevatorId);
                
                int destFloor;
                std::cout << "Enter destination floor: ";
                std::cin >> destFloor;
                
                // Determine direction based on current floor
                Elevator* elevator = controller->getElevators()[elevatorId - 1];
                Direction dir = destFloor > elevator->getCurrentFloor() ? Direction::UP : Direction::DOWN;
                
                // ✅ Use Command Pattern - Create command and execute it
                ElevatorRequest internalRequest(elevatorId, destFloor, true, dir, controller);
                internalRequest.execute();
                break;
            }
            
            case 3: {
                // Simulate the next step in the system
                std::cout << "\n>>> Simulating next step..." << std::endl;
                controller->step();
                displayElevatorStatus(controller->getElevators());
                break;
            }
            
            case 4: {
                // Change the scheduling strategy
                std::cout << "\nSelect strategy:" << std::endl;
                std::cout << "1. SCAN Algorithm" << std::endl;
                std::cout << "2. FCFS Algorithm" << std::endl;
                std::cout << "3. LOOK Algorithm" << std::endl;
                std::cout << "Choice: ";
                
                int strategyChoice;
                std::cin >> strategyChoice;
                
                if (strategyChoice == 1) {
                    controller->setSchedulingStrategy(std::make_unique<ScanSchedulingStrategy>());
                    std::cout << "Strategy set to SCAN Algorithm" << std::endl;
                } else if (strategyChoice == 2) {
                    controller->setSchedulingStrategy(std::make_unique<FCFSSchedulingStrategy>());
                    std::cout << "Strategy set to FCFS Algorithm" << std::endl;
                } else if (strategyChoice == 3) {
                    controller->setSchedulingStrategy(std::make_unique<LookSchedulingStrategy>());
                    std::cout << "Strategy set to LOOK Algorithm" << std::endl;
                } else {
                    std::cout << "Invalid choice!" << std::endl;
                }
                break;
            }
            
            case 5: {
                // Exit the simulation
                running = false;
                break;
            }
            
            default: {
                std::cout << "Invalid choice!" << std::endl;
                break;
            }
        }
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "   Simulation Ended" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}

