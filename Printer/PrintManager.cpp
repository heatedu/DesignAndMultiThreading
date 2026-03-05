#include "PrintManager.h"
#include <iostream>

PrintManager::PrintManager() : jobCounter(1) {}

PrintManager& PrintManager::getInstance() {
    static PrintManager instance;
    return instance;
}

void PrintManager::registerUser(const std::string& id,
                                const std::string& name) {
    users[id] = std::make_shared<User>(id, name);
}

void PrintManager::addPrinter() {
    printers.push_back(
        std::make_shared<Printer>(printers.size() + 1)
    );
}

int PrintManager::submitJob(const std::string& userId,
                            const std::string& doc,
                            int pages) {
    std::lock_guard<std::mutex> lock(mtx);

    auto user = users[userId];
    auto job = std::make_shared<PrintJob>(
        jobCounter++, user, doc, pages
    );

    jobQueue.push_back(job);
    job->setState(JobState::QUEUED);

    return job->getJobId();
}

void PrintManager::dispatchJobs() {
    for (auto& printer : printers) {
        if (!printer->isAvailable()) continue;

        auto job = strategy->selectJob(jobQueue);
        if (!job) continue;

        job->setState(JobState::PRINTING);
        printer->print(job->getJobId(),
                       job->getPages());
        job->setState(JobState::COMPLETED);

        jobQueue.erase(jobQueue.begin());
    }
}

void PrintManager::setStrategy(
    std::unique_ptr<SchedulingStrategy> strat) {
    strategy = std::move(strat);
}
