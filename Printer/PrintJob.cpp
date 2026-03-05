#include "PrintJob.h"

PrintJob::PrintJob(int id,
                   std::shared_ptr<User> owner,
                   const std::string& doc,
                   int pages)
    : jobId(id),
      owner(owner),
      documentName(doc),
      pages(pages),
      state(JobState::CREATED) {}

int PrintJob::getJobId() const { return jobId; }

int PrintJob::getPages() const { return pages; }

std::shared_ptr<User> PrintJob::getOwner() const {
    return owner;
}

JobState PrintJob::getState() const {
    return state;
}

void PrintJob::setState(JobState newState) {
    state = newState;
    owner->notify(jobId, "State changed");
}
