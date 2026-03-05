#include "ShortestJobFirst.h"
#include "PrintJob.h"
#include <limits>

std::shared_ptr<PrintJob>
ShortestJobFirst::selectJob(std::vector<std::shared_ptr<PrintJob>>& jobs) {
    if (jobs.empty()) return nullptr;

    std::shared_ptr<PrintJob> shortest;
    int minPages = std::numeric_limits<int>::max();

    for (auto& job : jobs) {
        if (job->getPages() < minPages) {
            minPages  = job->getPages();
            shortest  = job;
        }
    }
    return shortest;
}
