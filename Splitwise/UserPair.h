#pragma once
#include "User.h"

/**
 * Represents a pair of users for tracking balances.
 */
class UserPair {
private:
    User user1;
    User user2;

public:
    UserPair(const User& user1, const User& user2);
    
    User getUser1() const;
    User getUser2() const;
    
    bool operator==(const UserPair& other) const;
    bool operator<(const UserPair& other) const; // For using as map key
};

// Hash function for UserPair
namespace std {
    template<>
    struct hash<UserPair> {
        size_t operator()(const UserPair& pair) const {
            return hash<User>()(pair.getUser1()) ^ (hash<User>()(pair.getUser2()) << 1);
        }
    };
}
