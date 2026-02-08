#include "../../include/models/User.h"

User::User(const std::string& userName, const std::string& email)
    : name(userName), emailAddress(email) {}

std::string User::getUserName() const {
    return name;
}

std::string User::getUserEmail() const {
    return emailAddress;
}

bool User::operator==(const User& other) const {
    return emailAddress == other.emailAddress;
}

