#ifndef SHORTEST_JOB_FIRST_H
#define SHORTEST_JOB_FIRST_H

#include "SchedulingStrategy.h"

class ShortestJobFirst : public SchedulingStrategy {
public:
    std::shared_ptr<PrintJob>
    selectJob(std::vector<std::shared_ptr<PrintJob>>& jobs) override;
};

#endif
