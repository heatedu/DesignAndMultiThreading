#include "Printer.h"
#include <iostream>
#include <thread>
#include <chrono>

Printer::Printer(int id) : printerId(id), status(PrinterStatus::IDLE) {}

int Printer::getPrinterId() const {
    return printerId;
}

bool Printer::isAvailable() const {
    return status == PrinterStatus::IDLE;
}

void Printer::print(int jobId, int pages) {
    status = PrinterStatus::BUSY;
    std::cout << "Printer " << printerId 
              << " printing job " << jobId << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Printer " << printerId 
              << " finished job " << jobId << std::endl;

    status = PrinterStatus::IDLE;
}

void Printer::setIdle() {
    status = PrinterStatus::IDLE;
}
