#include "PrintManager.h"
#include "FIFO.h"
#include "ShortestJobFirst.h"
#include <iostream>

int main() {
    auto& manager = PrintManager::getInstance();

    // Register users
    manager.registerUser("u1", "Alice");
    manager.registerUser("u2", "Bob");

    // Add two printers
    manager.addPrinter();
    manager.addPrinter();

    // --- Demo 1: FIFO Strategy ---
    std::cout << "\n=== FIFO Strategy ===\n";
    manager.setStrategy(std::make_unique<FIFO>());

    int j1 = manager.submitJob("u1", "doc1.pdf", 10);
    int j2 = manager.submitJob("u2", "doc2.pdf", 5);

    std::cout << "Submitted jobs: " << j1 << ", " << j2 << "\n";
    manager.dispatchJobs();

    // --- Demo 2: ShortestJobFirst Strategy ---
    std::cout << "\n=== Shortest Job First Strategy ===\n";
    manager.setStrategy(std::make_unique<ShortestJobFirst>());

    int j3 = manager.submitJob("u1", "report.pdf", 20);
    int j4 = manager.submitJob("u2", "note.pdf",    2);   // shortest — should go first

    std::cout << "Submitted jobs: " << j3 << " (20 pages), "
              << j4 << " (2 pages)\n";
    manager.dispatchJobs();

    return 0;
}
