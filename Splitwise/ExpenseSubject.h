#pragma once
#include "Expense.h"
#include "ExpenseObserver.h"

/**
 * Interface for objects that can be observed for expense updates.
 * Implements the Subject part of the Observer Design Pattern.
 */
class ExpenseSubject {
public:
    virtual ~ExpenseSubject() = default;
    
    /**
     * Adds an observer to the notification list.
     */
    virtual void addObserver(ExpenseObserver* observer) = 0;
    
    /**
     * Removes an observer from the notification list.
     */
    virtual void removeObserver(ExpenseObserver* observer) = 0;
    
    /**
     * Notifies all observers about a new expense.
     */
    virtual void notifyExpenseAdded(const Expense& expense) = 0;
    
    /**
     * Notifies all observers about an updated expense.
     */
    virtual void notifyExpenseUpdated(const Expense& expense) = 0;
};
