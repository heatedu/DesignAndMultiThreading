#pragma once
#include <string>

/**
 * Represents a user in the Splitwise system.
 */
class User {
private:
    std::string id;
    std::string name;
    std::string email;

public:
    User(const std::string& id, const std::string& name, const std::string& email);
    
    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    
    bool operator==(const User& other) const;
    bool operator<(const User& other) const; // For using User as map key
};

// Hash function for User to use in unordered_map
namespace std {
    template<>
    struct hash<User> {
        size_t operator()(const User& user) const {
            return hash<string>()(user.getId());
        }
    };
}
