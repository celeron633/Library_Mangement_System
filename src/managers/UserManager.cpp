#include "UserManager.h"
#include <fstream>
#include <algorithm>

bool UserManager::loadFromFile() {
    std::ifstream file(m_dataFile);
    if (!file.is_open()) {
        return false;
    }

    m_users.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            m_users.push_back(User::deserialize(line));
        }
    }
    file.close();
    return true;
}

bool UserManager::saveToFile() const {
    std::ofstream file(m_dataFile, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& user : m_users) {
        file << user.serialize() << "\n";
    }
    file.close();
    return true;
}

bool UserManager::addUser(const User& user) {
    if (userExists(user.getName())) {
        return false;
    }
    m_users.push_back(user);
    return true;
}

bool UserManager::deleteUser(const std::string& name) {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&name](const User& u) { return u.getName() == name; });
    if (it != m_users.end()) {
        m_users.erase(it);
        return true;
    }
    return false;
}

User* UserManager::findUserByName(const std::string& name) {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&name](User& u) { return u.getName() == name; });
    return (it != m_users.end()) ? &(*it) : nullptr;
}

const User* UserManager::findUserByNameConst(const std::string& name) const {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&name](const User& u) { return u.getName() == name; });
    return (it != m_users.end()) ? &(*it) : nullptr;
}

bool UserManager::userExists(const std::string& name) const {
    return findUserByNameConst(name) != nullptr;
}

const std::vector<User>& UserManager::getAllUsers() const {
    return m_users;
}

size_t UserManager::getUserCount() const {
    return m_users.size();
}
