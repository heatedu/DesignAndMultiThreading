#ifndef SCHEDULING_STRATEGY_H
#define SCHEDULING_STRATEGY_H

#include <vector>
#include <memory>

class PrintJob;

class SchedulingStrategy {
public:
    virtual ~SchedulingStrategy() = default;
    virtual std::shared_ptr<PrintJob> 
    selectJob(std::vector<std::shared_ptr<PrintJob>>& jobs) = 0;
};

#endif
