#include <iostream>
#include <iomanip>
#include "../include/User.h"
#include "../include/Expense.h"
#include "../include/Transaction.h"
#include "../include/SplitFactory.h"
#include "../include/ExpenseManager.h"
#include "../include/BalanceSheet.h"

int main() {
    std::cout << "=== Splitwise Expense Sharing System ===\n" << std::endl;
    
    // Create users
    User alice("u1", "Alice", "alice@example.com");
    User bob("u2", "Bob", "bob@example.com");
    User charlie("u3", "Charlie", "charlie@example.com");
    
    // Create expense manager and balance sheet
    ExpenseManager expenseManager;
    BalanceSheet balanceSheet;
    
    // Register the balance sheet as an observer (Observer Pattern)
    expenseManager.addObserver(&balanceSheet);
    
    // Create participants list
    std::vector<User> participants = {alice, bob, charlie};
    
    // === EXAMPLE 1: Equal Split ===
    std::cout << "--- Expense 1: Dinner (Equal Split) ---" << std::endl;
    std::cout << "Alice pays $60 for dinner, split equally among 3 people" << std::endl;
    
    // Use Factory Pattern to create an Equal Split
    auto equalSplit = SplitFactory::createSplit("EQUAL");
    std::map<std::string, std::any> splitDetails;
    auto dinnerShares = equalSplit->calculateSplit(60.0, participants, splitDetails);
    
    Expense dinnerExpense("e1", "Dinner", 60.0, alice, participants, dinnerShares);
    expenseManager.addExpense(dinnerExpense);
    
    std::cout << "Each person owes: $" << std::fixed << std::setprecision(2) 
              << dinnerShares[alice] << std::endl << std::endl;
    
    // === EXAMPLE 2: Percentage Split ===
    std::cout << "--- Expense 2: Movie Tickets (Percentage Split) ---" << std::endl;
    std::cout << "Bob pays $45 for movie tickets" << std::endl;
    std::cout << "Split: Alice=40%, Bob=30%, Charlie=30%" << std::endl;
    
    // Use Factory Pattern to create a Percentage Split
    std::map<User, double> percentages;
    percentages[alice] = 40.0;
    percentages[bob] = 30.0;
    percentages[charlie] = 30.0;
    
    std::map<std::string, std::any> percentageSplitDetails;
    percentageSplitDetails["percentages"] = percentages;
    
    auto percentageSplit = SplitFactory::createSplit("PERCENTAGE");
    auto movieShares = percentageSplit->calculateSplit(45.0, participants, percentageSplitDetails);
    
    Expense movieExpense("e2", "Movie", 45.0, bob, participants, movieShares);
    expenseManager.addExpense(movieExpense);
    
    std::cout << "Alice owes: $" << movieShares[alice] << std::endl;
    std::cout << "Bob owes: $" << movieShares[bob] << std::endl;
    std::cout << "Charlie owes: $" << movieShares[charlie] << std::endl << std::endl;
    
    // === Display Balances ===
    std::cout << "=== INDIVIDUAL BALANCES ===" << std::endl;
    std::cout << "Alice's total balance: $" << std::fixed << std::setprecision(2) 
              << balanceSheet.getTotalBalance(alice) << std::endl;
    std::cout << "Bob's total balance: $" << balanceSheet.getTotalBalance(bob) << std::endl;
    std::cout << "Charlie's total balance: $" << balanceSheet.getTotalBalance(charlie) 
              << std::endl << std::endl;
    
    std::cout << "=== PAIRWISE BALANCES ===" << std::endl;
    std::cout << "Alice and Bob: $" << balanceSheet.getBalance(alice, bob) 
              << " (positive = Alice owes Bob)" << std::endl;
    std::cout << "Alice and Charlie: $" << balanceSheet.getBalance(alice, charlie) << std::endl;
    std::cout << "Bob and Charlie: $" << balanceSheet.getBalance(bob, charlie) 
              << std::endl << std::endl;
    
    // === Simplified Settlements ===
    std::cout << "=== SIMPLIFIED SETTLEMENTS ===" << std::endl;
    auto settlements = balanceSheet.getSimplifiedSettlements();
    
    std::cout << "Number of transactions: " << settlements.size() << std::endl;
    for (const auto& transaction : settlements) {
        std::cout << transaction.getFrom().getName() << " pays "
                  << transaction.getTo().getName() << " $"
                  << std::fixed << std::setprecision(2) << transaction.getAmount() << std::endl;
    }
    std::cout << std::endl;
    
    // === Optimal Minimum Settlements (DP Algorithm) ===
    std::cout << "=== OPTIMAL MINIMUM SETTLEMENTS (DP) ===" << std::endl;
    int optimalSettlements = balanceSheet.getOptimalMinimumSettlements();
    std::cout << "Minimum transactions needed (using DP): " << optimalSettlements 
              << std::endl << std::endl;
    
    // === Suboptimal Backtracking Approach ===
    std::cout << "=== SUBOPTIMAL MINIMUM SETTLEMENTS (Backtracking) ===" << std::endl;
    int suboptimalSettlements = balanceSheet.getSubOptimalMinimumSettlements();
    std::cout << "Minimum transactions needed (using Backtracking): " << suboptimalSettlements 
              << std::endl << std::endl;
    
    // === Design Patterns Used ===
    std::cout << "=== DESIGN PATTERNS DEMONSTRATED ===" << std::endl;
    std::cout << "1. Factory Pattern: SplitFactory creates different split types" << std::endl;
    std::cout << "   - EqualSplit for equal distribution" << std::endl;
    std::cout << "   - PercentageSplit for percentage-based distribution" << std::endl;
    std::cout << "   - Easy to extend with new split types" << std::endl << std::endl;
    
    std::cout << "2. Observer Pattern: BalanceSheet observes ExpenseManager" << std::endl;
    std::cout << "   - BalanceSheet automatically updates when expenses are added" << std::endl;
    std::cout << "   - Loose coupling between components" << std::endl;
    std::cout << "   - Multiple observers can be added easily" << std::endl << std::endl;
    
    std::cout << "=== System Ready for Interview Discussion ===" << std::endl;
    
    return 0;
}
