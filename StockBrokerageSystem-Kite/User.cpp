#include "User.h"

User::User(const std::string& userId, const std::string& name, const std::string& email)
    : userId_(userId), name_(name), email_(email) {}

std::string User::getUserId() const { return userId_; }
std::string User::getName() const { return name_; }
std::string User::getEmail() const { return email_; }
