#ifndef PRINT_JOB_H
#define PRINT_JOB_H

#include <memory>
#include <string>
#include "User.h"

enum class JobState {
    CREATED,
    QUEUED,
    PRINTING,
    COMPLETED,
    CANCELLED
};

class PrintJob {
private:
    int jobId;
    std::shared_ptr<User> owner;
    std::string documentName;
    int pages;
    JobState state;

public:
    PrintJob(int id,
             std::shared_ptr<User> owner,
             const std::string& doc,
             int pages);

    int getJobId() const;
    int getPages() const;
    std::shared_ptr<User> getOwner() const;
    JobState getState() const;

    void setState(JobState newState);
};

#endif
