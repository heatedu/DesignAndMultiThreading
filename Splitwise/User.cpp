#include "../include/User.h"

User::User(const std::string& id, const std::string& name, const std::string& email)
    : id(id), name(name), email(email) {}

std::string User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

bool User::operator==(const User& other) const {
    return id == other.id;
}

bool User::operator<(const User& other) const {
    return id < other.id;
}
