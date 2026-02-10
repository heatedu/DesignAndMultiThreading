#pragma once
#include "Expense.h"

/**
 * Interface for observers that need to be notified of expense updates.
 * Implements the Observer Design Pattern.
 */
class ExpenseObserver {
public:
    virtual ~ExpenseObserver() = default;
    
    /**
     * Called when a new expense is added to the system.
     */
    virtual void onExpenseAdded(const Expense& expense) = 0;
    
    /**
     * Called when an expense is updated in the system.
     */
    virtual void onExpenseUpdated(const Expense& expense) = 0;
};
