#pragma once
#include <string>

class User {
private:
    std::string userId_;
    std::string name_;
    std::string email_;

public:
    User(const std::string& userId, const std::string& name, const std::string& email);

    std::string getUserId() const;
    std::string getName() const;
    std::string getEmail() const;
};
