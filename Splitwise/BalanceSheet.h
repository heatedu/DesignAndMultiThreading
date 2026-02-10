#pragma once
#include <map>
#include <vector>
#include "ExpenseObserver.h"
#include "User.h"
#include "UserPair.h"
#include "Transaction.h"
#include "Expense.h"

/**
 * Balance sheet that observes expenses and calculates balances.
 * Implements the Observer Design Pattern as a concrete observer.
 */
class BalanceSheet : public ExpenseObserver {
private:
    /*Example : balances[UserPair(Bob, Alice)] = 20.0
    Means : Bob owes Alice $20*/
    std::map<UserPair, double> balances;
    
    /**
     * Updates the balances based on a new or updated expense.
     */
    void updateBalances(const Expense& expense);
    
    /**
     * Helper method to calculate the sum of balances in a subset.
     */
    double sumOfMask(const std::vector<double>& values, int mask) const;
    
    /**
     * DFS with memoization for optimal settlements.
     */
    int dfs(int mask, std::vector<int>& dp, const std::vector<double>& creditList) const;
    
    /**
     * DFS for suboptimal backtracking approach.
     */
    int subOptimalDfs(int currentUserIndex, std::vector<double>& creditList, int n) const;

public:
    void onExpenseAdded(const Expense& expense) override;
    void onExpenseUpdated(const Expense& expense) override;
    
    /**
     * Gets the net balance between two users.
     * @return The amount user1 owes user2 (negative if user2 owes user1)
     */
    double getBalance(const User& user1, const User& user2) const;
    
    /**
     * Calculates the total balance for a single user.
     * Negative = user owes money, Positive = user is owed money
     */
    double getTotalBalance(const User& user) const;
    
    /**
     * Simplifies the balances into a list of transactions to settle all debts.
     * Simple and straightforward implementation.
     */
    std::vector<Transaction> getSimplifiedSettlements() const;
    
    /**
     * Calculates the minimum number of transactions using backtracking.
     * Time Complexity: O((n-1)!)
     */
    int getSubOptimalMinimumSettlements() const;
    
    /**
     * Calculates the minimum number of transactions using dynamic programming.
     * Time Complexity: O(n * 2^n)
     */
    int getOptimalMinimumSettlements() const;
};
