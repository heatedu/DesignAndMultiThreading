#include "FIFO.h"
#include "PrintJob.h"

std::shared_ptr<PrintJob> 
FIFO::selectJob(std::vector<std::shared_ptr<PrintJob>>& jobs) {
    if (jobs.empty()) return nullptr;
    return jobs.front();
}
