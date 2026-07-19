#include "UserManager.h"
#include <algorithm>
#include <sqlite3.h>

UserManager::UserManager(std::shared_ptr<Database> database)
    : m_database(std::move(database)) {}

bool UserManager::load() {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(
            m_database->handle(),
            "SELECT name, password, balance, is_vip FROM users ORDER BY name;",
            -1, &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }

    m_users.clear();
    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        const auto* name = sqlite3_column_text(statement, 0);
        const auto* password = sqlite3_column_text(statement, 1);
        m_users.emplace_back(
            name ? reinterpret_cast<const char*>(name) : "",
            password ? reinterpret_cast<const char*>(password) : "",
            sqlite3_column_double(statement, 2),
            sqlite3_column_int(statement, 3) != 0);
    }

    sqlite3_finalize(statement);
    if (result != SQLITE_DONE) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }
    return true;
}

bool UserManager::save() const {
    if (!m_database->beginTransaction()) return false;
    if (!m_database->execute("DELETE FROM users;")) {
        m_database->rollbackTransaction();
        return false;
    }

    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(
            m_database->handle(),
            "INSERT INTO users(name, password, balance, is_vip) "
            "VALUES(?, ?, ?, ?);",
            -1, &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        m_database->rollbackTransaction();
        return false;
    }

    bool success = true;
    for (const auto& user : m_users) {
        sqlite3_bind_text(statement, 1, user.getName().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, user.getPassword().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_double(statement, 3, user.getBalance());
        sqlite3_bind_int(statement, 4, user.isVIP() ? 1 : 0);
        if (sqlite3_step(statement) != SQLITE_DONE) {
            success = false;
            m_database->setLastError(sqlite3_errmsg(m_database->handle()));
            break;
        }
        sqlite3_reset(statement);
        sqlite3_clear_bindings(statement);
    }
    sqlite3_finalize(statement);

    if (!success) {
        m_database->rollbackTransaction();
        return false;
    }
    if (!m_database->commitTransaction()) {
        m_database->rollbackTransaction();
        return false;
    }
    return true;
}

bool UserManager::clearAll() {
    m_users.clear();
    return m_database->execute("DELETE FROM users;");
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
