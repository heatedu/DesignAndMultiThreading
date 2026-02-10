#include "../include/UserPair.h"

UserPair::UserPair(const User& user1, const User& user2)
    : user1(user1), user2(user2) {}

User UserPair::getUser1() const {
    return user1;
}

User UserPair::getUser2() const {
    return user2;
}

bool UserPair::operator==(const UserPair& other) const {
    return (user1 == other.user1 && user2 == other.user2);
}

bool UserPair::operator<(const UserPair& other) const {
    if (user1 < other.user1) return true;
    if (other.user1 < user1) return false;
    return user2 < other.user2;
}
