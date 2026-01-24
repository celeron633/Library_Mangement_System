#pragma once

#include "../models/User.h"
#include <vector>
#include <memory>

class UserManager {
private:
    std::vector<User> m_users;
    const std::string m_dataFile = "user.dat";

public:
    UserManager() = default;

    // File operations
    bool loadFromFile();
    bool saveToFile() const;

    // User operations
    bool addUser(const User& user);
    bool deleteUser(const std::string& name);
    User* findUserByName(const std::string& name);
    const User* findUserByNameConst(const std::string& name) const;
    bool userExists(const std::string& name) const;

    // Getters
    const std::vector<User>& getAllUsers() const;
    size_t getUserCount() const;
};
