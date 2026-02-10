#include "../include/ExpenseManager.h"
#include <algorithm>
#include <stdexcept>

void ExpenseManager::addObserver(ExpenseObserver* observer) {
    observers.push_back(observer);
}

void ExpenseManager::removeObserver(ExpenseObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void ExpenseManager::notifyExpenseAdded(const Expense& expense) {
    for (ExpenseObserver* observer : observers) {
        observer->onExpenseAdded(expense);
    }
}

void ExpenseManager::notifyExpenseUpdated(const Expense& expense) {
    for (ExpenseObserver* observer : observers) {
        observer->onExpenseUpdated(expense);
    }
}

void ExpenseManager::addExpense(const Expense& expense) {
    expenses.push_back(expense);
    notifyExpenseAdded(expense);
}

void ExpenseManager::updateExpense(const Expense& expense) {
    for (size_t i = 0; i < expenses.size(); i++) {
        if (expenses[i].getId() == expense.getId()) {
            expenses[i] = expense;
            notifyExpenseUpdated(expense);
            return;
        }
    }
    throw std::invalid_argument("Expense with ID " + expense.getId() + " not found.");
}

std::vector<Expense> ExpenseManager::getAllExpenses() const {
    return expenses;
}
