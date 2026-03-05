#ifndef FIFO_H
#define FIFO_H

#include "SchedulingStrategy.h"

class FIFO : public SchedulingStrategy {
public:
    std::shared_ptr<PrintJob> 
    selectJob(std::vector<std::shared_ptr<PrintJob>>& jobs) override;
};

#endif
