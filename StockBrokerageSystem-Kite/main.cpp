// ============================================================
// StockBrokerageSystem - Demo / Entry Point
// Design Patterns used:
//   1. Singleton  — StockBroker (single instance)
//   2. Factory    — OrderFactory (creates BuyOrder / SellOrder)
//   3. Observer   — Stock (subject) notifies Account (observer)
// ============================================================

#include <iostream>
#include "StockBroker.h"

int main() {
    // ---- Get the singleton broker instance ----
    StockBroker& broker = StockBroker::getInstance();

    // ---- Add some stocks to the market ----
    broker.addStock("AAPL", "Apple Inc.", 150.00);
    broker.addStock("GOOGL", "Alphabet Inc.", 2800.00);
    broker.addStock("AMZN", "Amazon.com Inc.", 3400.00);

    // ---- Create users & accounts ----
    User user1("U1", "Alice", "alice@example.com");
    User user2("U2", "Bob", "bob@example.com");

    Account* acc1 = broker.createAccount("ACC-1", user1, 100000.00);
    Account* acc2 = broker.createAccount("ACC-2", user2, 50000.00);

    // ---- Register accounts as observers on stocks (Observer pattern) ----
    Stock* aapl  = broker.getStock("AAPL");
    Stock* googl = broker.getStock("GOOGL");
    Stock* amzn  = broker.getStock("AMZN");

    aapl->registerObserver(acc1);
    googl->registerObserver(acc1);
    googl->registerObserver(acc2);

    // ---- Place some BUY orders (Factory pattern creates orders) ----
    std::cout << "\n--- Placing BUY orders ---\n";
    broker.placeOrder(OrderType::BUY, *acc1, *aapl, 10, aapl->getPrice());     // Alice buys 10 AAPL
    broker.placeOrder(OrderType::BUY, *acc1, *googl, 5, googl->getPrice());    // Alice buys 5 GOOGL
    broker.placeOrder(OrderType::BUY, *acc2, *amzn, 3, amzn->getPrice());      // Bob buys 3 AMZN

    // ---- View portfolios ----
    broker.printPortfolio(*acc1);
    broker.printPortfolio(*acc2);

    // ---- Simulate a price update (Observer notifies holders) ----
    std::cout << "\n--- Stock price update ---\n";
    aapl->updatePrice(160.00);
    googl->updatePrice(2900.00);

    // ---- Place a SELL order ----
    std::cout << "\n--- Placing SELL order ---\n";
    broker.placeOrder(OrderType::SELL, *acc1, *aapl, 5, aapl->getPrice());     // Alice sells 5 AAPL

    // ---- Try selling more than owned (should be REJECTED) ----
    std::cout << "\n--- Attempting invalid SELL ---\n";
    broker.placeOrder(OrderType::SELL, *acc2, *aapl, 100, aapl->getPrice());   // Bob has no AAPL

    // ---- Try buying with insufficient funds (should be REJECTED) ----
    std::cout << "\n--- Attempting buy with insufficient funds ---\n";
    broker.placeOrder(OrderType::BUY, *acc2, *googl, 1000, googl->getPrice()); // too expensive

    // ---- Final state ----
    broker.printPortfolio(*acc1);
    broker.printPortfolio(*acc2);

    broker.printTransactionHistory(*acc1);
    broker.printTransactionHistory(*acc2);

    return 0;
}
