#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

class User {
private:
    std::string userId;
    std::string name;

public:
    User(const std::string& id, const std::string& name);

    std::string getUserId() const;
    std::string getName() const;

    void notify(int jobId, const std::string& message);
};

#endif
