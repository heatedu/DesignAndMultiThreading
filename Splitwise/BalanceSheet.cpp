#include "../include/BalanceSheet.h"
#include <algorithm>
#include <cmath>
#include <limits>

void BalanceSheet::onExpenseAdded(const Expense& expense) {
    updateBalances(expense);
}

void BalanceSheet::onExpenseUpdated(const Expense& expense) {
    updateBalances(expense);
}

void BalanceSheet::updateBalances(const Expense& expense) {
    User payer = expense.getPayer();
    std::map<User, double> shares = expense.getShares();
    
    for (const auto& entry : shares) {
        User participant = entry.first;
        double amount = entry.second;
        
        if (!(participant == payer)) {
            UserPair userPair(participant, payer);
            double currentBalance = 0.0;
            
            if (balances.find(userPair) != balances.end()) {
                currentBalance = balances[userPair];
            }
            
            balances[userPair] = currentBalance + amount;
        }
    }
}

double BalanceSheet::getBalance(const User& user1, const User& user2) const {
    UserPair pair1(user1, user2);
    UserPair pair2(user2, user1);
    
    double balance1 = 0.0;
    double balance2 = 0.0;
    
    if (balances.find(pair1) != balances.end()) {
        balance1 = balances.at(pair1);
    }
    
    if (balances.find(pair2) != balances.end()) {
        balance2 = balances.at(pair2);
    }
    
    return balance1 - balance2;
}

double BalanceSheet::getTotalBalance(const User& user) const {
    double total = 0.0;
    
    for (const auto& entry : balances) {
        UserPair pair = entry.first;
        double amount = entry.second;
        
        if (pair.getUser1() == user) {
            total -= amount; // Money owed by the user
        } else if (pair.getUser2() == user) {
            total += amount; // Money owed to the user
        }
    }
    
    return total;
}

std::vector<Transaction> BalanceSheet::getSimplifiedSettlements() const {
    // Step 1: Calculate net balances for each user
    std::map<User, double> netBalances;
    
    for (const auto& entry : balances) {
        UserPair pair = entry.first;
        double amount = entry.second;
        
        User debtor = pair.getUser1();
        User creditor = pair.getUser2();
        
        netBalances[debtor] = netBalances[debtor] - amount;
        netBalances[creditor] = netBalances[creditor] + amount;
    }
    
    // Step 2: Separate users into debtors and creditors
    std::vector<User> debtors;
    std::vector<User> creditors;
    
    for (const auto& entry : netBalances) {
        User user = entry.first;
        double balance = entry.second;
        
        if (balance < 0) {
            debtors.push_back(user);
        } else if (balance > 0) {
            creditors.push_back(user);
        }
    }
    
    // Step 2.5: Sort for greedy approach (largest debts/credits first)
    // This maximizes the amount settled per transaction
    std::sort(debtors.begin(), debtors.end(), 
        [&netBalances](const User& a, const User& b) {
            return std::abs(netBalances[a]) > std::abs(netBalances[b]);
        });
    
    std::sort(creditors.begin(), creditors.end(),
        [&netBalances](const User& a, const User& b) {
            return netBalances[a] > netBalances[b];
        });
    
    // Step 3: Match debtors and creditors to create transactions
    std::vector<Transaction> transactions;
    size_t debtorIndex = 0;
    size_t creditorIndex = 0;
    
    while (debtorIndex < debtors.size() && creditorIndex < creditors.size()) {
        User debtor = debtors[debtorIndex];
        User creditor = creditors[creditorIndex];
        double debtorBalance = netBalances[debtor];
        double creditorBalance = netBalances[creditor];
        
        double transferAmount = std::min(std::abs(debtorBalance), creditorBalance);
        
        transactions.push_back(Transaction(debtor, creditor, transferAmount));
        
        netBalances[debtor] = debtorBalance + transferAmount;
        netBalances[creditor] = creditorBalance - transferAmount;
        
        if (std::abs(netBalances[debtor]) < 0.001) {
            debtorIndex++;
        }
        if (std::abs(netBalances[creditor]) < 0.001) {
            creditorIndex++;
        }
    }
    
    return transactions;
}

int BalanceSheet::getSubOptimalMinimumSettlements() const {
    // Step 1: Calculate net balances for each user
    std::map<User, double> netBalances;
    
    for (const auto& entry : balances) {
        UserPair pair = entry.first;
        double amount = entry.second;
        
        User debtor = pair.getUser1();
        User creditor = pair.getUser2();
        
        netBalances[debtor] = netBalances[debtor] - amount;
        netBalances[creditor] = netBalances[creditor] + amount;
    }
    
    std::vector<double> activeBalances;
    
    for (const auto& entry : netBalances) {
        if (std::abs(entry.second) > 0.001) {
            activeBalances.push_back(entry.second);
        }
    }
    
    int n = activeBalances.size();
    return subOptimalDfs(0, activeBalances, n);
}

int BalanceSheet::subOptimalDfs(int currentUserIndex, std::vector<double>& activeBalances, int n) const {
    // Skip already settled users
    while (currentUserIndex < n && std::abs(activeBalances[currentUserIndex]) < 0.001) {
        currentUserIndex++;
    }
    
    if (currentUserIndex == n) {
        return 0;
    }
    
    int cost = std::numeric_limits<int>::max();
    
    for (int nextIndex = currentUserIndex + 1; nextIndex < n; nextIndex++)//settling current person with ALL future people in the list
    {
        if (activeBalances[nextIndex] * activeBalances[currentUserIndex] < 0) //Only try if they have OPPOSITE signs (one owes, one is owed)
        {
            /*
            Both directions are handled by the same code!
            Multiplication check ensures opposite signs (one positive, one negative)
            Addition operation naturally combines them:
            Positive + Negative = Settlement
            Negative + Positive = Settlement (same thing!)
            Order doesn't matter - the math works symmetrically
            The algorithm explores all valid pairs regardless of which is creditor or debtor
            */
            activeBalances[nextIndex] += activeBalances[currentUserIndex];
            cost = std::min(cost, 1 + subOptimalDfs(currentUserIndex + 1, activeBalances, n));
            activeBalances[nextIndex] -= activeBalances[currentUserIndex];
        }
    }
    
    return cost;
}

//A bitmask is a binary number where each bit represents whether to include that index.
double BalanceSheet::sumOfMask(const std::vector<double>& values, int mask) const {
    double sum = 0;
    for (size_t i = 0; i < values.size(); i++) {
        if ((mask & (1 << i)) != 0) {
            sum += values[i];
        }
    }
    return sum;
}

int BalanceSheet::dfs(int mask, std::vector<int>& dp, const std::vector<double>& creditList) const {
    if (mask == 0) {
        return 0;
    }
    
    if (dp[mask] != -1) {
        return dp[mask];
    }
    
    int maxSubGroups = 0;
    int n = creditList.size();
    /*
        creditList = [10, -5, -5]  (indices 0, 1, 2)

        mask = 7 (binary: 111) → Consider all 3 people
        mask = 5 (binary: 101) → Consider people at index 0 and 2
        mask = 3 (binary: 011) → Consider people at index 1 and 2
        mask = 0 (binary: 000) → Consider nobody (base case)
    */
    
    for (int submask = 1; submask < (1 << n); submask++) {
        /*
            The condition (submask & mask) == submask ensures:
            We only consider people in our current mask
            We don't try to use people already grouped
            We don't try people not in our subset
            submask is a TRUE subset of mask
            Without it: We'd try to form groups with people who aren't available, leading to incorrect results or counting people multiple times!
        */
        if ((submask & mask) == submask && std::abs(sumOfMask(creditList, submask)) < 0.001) 
        {
            //Valid Group Found:
            maxSubGroups = std::max(maxSubGroups, 1 + dfs(mask ^ submask, dp, creditList));
        }
    }
    
    dp[mask] = maxSubGroups;
    return maxSubGroups;
}

int BalanceSheet::getOptimalMinimumSettlements() const {
    // Step 1: Calculate net balances for each user
    std::map<User, double> netBalances;
    
    for (const auto& entry : balances) {
        UserPair pair = entry.first;
        double amount = entry.second;
        
        User debtor = pair.getUser1();
        User creditor = pair.getUser2();
        
        netBalances[debtor] = netBalances[debtor] - amount;
        netBalances[creditor] = netBalances[creditor] + amount;
    }
    
    std::vector<double> creditList;
    
    for (const auto& entry : netBalances) {
        if (std::abs(entry.second) > 0.001) {
            creditList.push_back(entry.second);
        }
    }
    
    int n = creditList.size();
    std::vector<int> dp(1 << n, -1);
    dp[0] = 0;
    
    int maxSubGroups = dfs((1 << n) - 1, dp, creditList);
    
    return n - maxSubGroups;
}
