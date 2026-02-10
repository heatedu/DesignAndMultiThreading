#pragma once
#include <vector>
#include "ExpenseSubject.h"
#include "ExpenseObserver.h"
#include "Expense.h"

/**
 * Concrete implementation of ExpenseSubject that manages expense notifications.
 * Implements the Observer Design Pattern.
 */
class ExpenseManager : public ExpenseSubject {
private:
    std::vector<ExpenseObserver*> observers;
    std::vector<Expense> expenses;

public:
    void addObserver(ExpenseObserver* observer) override;
    void removeObserver(ExpenseObserver* observer) override;
    void notifyExpenseAdded(const Expense& expense) override;
    void notifyExpenseUpdated(const Expense& expense) override;
    
    /**
     * Adds a new expense to the system and notifies observers.
     */
    void addExpense(const Expense& expense);
    
    /**
     * Updates an existing expense and notifies observers.
     */
    void updateExpense(const Expense& expense);
    
    /**
     * Retrieves all expenses in the system.
     */
    std::vector<Expense> getAllExpenses() const;
};
