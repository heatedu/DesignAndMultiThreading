#pragma once
#include <string>

class User {
private:
    std::string name;
    std::string emailAddress;

public:
    User(const std::string& userName, const std::string& email);
    
    std::string getUserName() const;
    std::string getUserEmail() const;
    
    bool operator==(const User& other) const;
};

