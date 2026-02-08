#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string userId;
    std::string name;
    std::string licenseNumber;

public:
    User(const std::string& id, const std::string& name, const std::string& license)
        : userId(id), name(name), licenseNumber(license) {}

    std::string getUserId() const { return userId; }
    std::string getName() const { return name; }
    std::string getLicense() const { return licenseNumber; }
};

#endif

