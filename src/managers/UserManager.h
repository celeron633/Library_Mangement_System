#pragma once

#include "../models/User.h"
#include "../database/Database.h"
#include <vector>
#include <memory>

class UserManager {
private:
    std::vector<User> m_users;
    std::shared_ptr<Database> m_database;

public:
    explicit UserManager(std::shared_ptr<Database> database);

    // Persistence
    bool load();
    bool save() const;
    bool clearAll();

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
