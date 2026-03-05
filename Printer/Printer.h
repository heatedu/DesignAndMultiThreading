#ifndef PRINTER_H
#define PRINTER_H

#include <string>

enum class PrinterStatus {
    IDLE,
    BUSY
};

class Printer {
private:
    int printerId;
    PrinterStatus status;

public:
    Printer(int id);

    int getPrinterId() const;
    bool isAvailable() const;

    void print(int jobId, int pages);
    void setIdle();
};

#endif
