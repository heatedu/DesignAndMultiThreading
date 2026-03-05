#include "User.h"

User::User(const std::string& id, const std::string& name)
    : userId(id), name(name) {}

std::string User::getUserId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}

void User::notify(int jobId, const std::string& message) {
    std::cout << "Notification for User [" << name 
              << "] - Job " << jobId 
              << ": " << message << std::endl;
}
