#include "ATMMachine.h"
#include "Card.h"
#include "Account.h"
#include <iostream>
#include <string>

void displayMenu() {
    std::cout << "\n=== ATM Simulation ===\n";
    std::cout << "1. Insert Card\n";
    std::cout << "2. Enter PIN\n";
    std::cout << "3. Select Operation\n";
    std::cout << "4. Withdraw Cash\n";
    std::cout << "5. Check Balance\n";
    std::cout << "6. Eject Card\n";
    std::cout << "7. View ATM Status\n";
    std::cout << "8. View Cash Inventory\n";
    std::cout << "9. View Transaction Logs\n";
    std::cout << "10. Toggle Maintenance Mode\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    // Setup accounts and cards
    Account acc1("ACC001", 5000.0);
    Account acc2("ACC002", 2000.0);
    
    Card card1("1234-5678-9012-3456", "1234", &acc1);
    Card card2("9876-5432-1098-7654", "5678", &acc2);
    
    // Create ATM
    ATMMachine atm;
    
    std::cout << "=== ATM Machine System ===\n";
    std::cout << "Test Cards Available:\n";
    std::cout << "Card 1: " << card1.getCardNumber() << " | PIN: 1234 | Balance: $5000\n";
    std::cout << "Card 2: " << card2.getCardNumber() << " | PIN: 5678 | Balance: $2000\n";
    
    Card* selectedCard = nullptr;
    int choice;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                std::cout << "Select card (1 or 2): ";
                int cardChoice;
                std::cin >> cardChoice;
                selectedCard = (cardChoice == 1) ? &card1 : &card2;
                atm.insertCard(selectedCard);
                break;
            }
            case 2: {
                std::cout << "Enter PIN: ";
                std::string pin;
                std::cin >> pin;
                atm.enterPin(pin);
                break;
            }
            case 3: {
                std::cout << "Select operation (1-Withdraw, 2-Balance, 3-Exit): ";
                int op;
                std::cin >> op;
                atm.selectOperation(op);
                break;
            }
            case 4: {
                std::cout << "Enter amount: $";
                double amount;
                std::cin >> amount;
                atm.withdrawCash(amount);
                break;
            }
            case 5: {
                atm.checkBalance();
                break;
            }
            case 6: {
                atm.ejectCard();
                selectedCard = nullptr;
                break;
            }
            case 7: {
                atm.displayStatus();
                break;
            }
            case 8: {
                atm.getCashDispenser().displayInventory();
                break;
            }
            case 9: {
                atm.getTransactionLog().displayLogs();
                break;
            }
            case 10: {
                atm.setMaintenanceMode(!atm.isInMaintenanceMode());
                break;
            }
            case 0: {
                std::cout << "Exiting ATM system. Goodbye!\n";
                return 0;
            }
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
    
    return 0;
}

