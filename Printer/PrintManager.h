#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "Printer.h"
#include "PrintJob.h"
#include "SchedulingStrategy.h"

class PrintManager {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Printer>> printers;
    std::vector<std::shared_ptr<PrintJob>> jobQueue;

    std::unique_ptr<SchedulingStrategy> strategy;

    int jobCounter;
    std::mutex mtx;

    PrintManager();

public:
    static PrintManager& getInstance();

    void registerUser(const std::string& id, const std::string& name);
    void addPrinter();

    int submitJob(const std::string& userId,
                  const std::string& doc,
                  int pages);

    void dispatchJobs();

    void setStrategy(std::unique_ptr<SchedulingStrategy> strat);
};

#endif
