#include "AdminManager.h"
#include <algorithm>
#include <sqlite3.h>

AdminManager::AdminManager(std::shared_ptr<Database> database)
    : m_database(std::move(database)) {}

bool AdminManager::load() {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(m_database->handle(),
                           "SELECT name, password FROM admins ORDER BY name;", -1,
                           &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }

    m_admins.clear();
    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        const auto* name = sqlite3_column_text(statement, 0);
        const auto* password = sqlite3_column_text(statement, 1);
        m_admins.emplace_back(
            name ? reinterpret_cast<const char*>(name) : "",
            password ? reinterpret_cast<const char*>(password) : "");
    }

    sqlite3_finalize(statement);
    if (result != SQLITE_DONE) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }
    return true;
}

bool AdminManager::save() const {
    if (!m_database->beginTransaction()) return false;
    if (!m_database->execute("DELETE FROM admins;")) {
        m_database->rollbackTransaction();
        return false;
    }

    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(m_database->handle(),
                           "INSERT INTO admins(name, password) VALUES(?, ?);", -1,
                           &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        m_database->rollbackTransaction();
        return false;
    }

    bool success = true;
    for (const auto& admin : m_admins) {
        sqlite3_bind_text(statement, 1, admin.getName().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, admin.getPassword().c_str(), -1,
                          SQLITE_TRANSIENT);
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

bool AdminManager::clearAll() {
    m_admins.clear();
    return m_database->execute("DELETE FROM admins;");
}

bool AdminManager::addAdmin(const Admin& admin) {
    if (adminExists(admin.getName())) {
        return false;
    }
    m_admins.push_back(admin);
    return true;
}

bool AdminManager::deleteAdmin(const std::string& name) {
    auto it = std::find_if(m_admins.begin(), m_admins.end(),
                           [&name](const Admin& a) { return a.getName() == name; });
    if (it != m_admins.end()) {
        m_admins.erase(it);
        return true;
    }
    return false;
}

Admin* AdminManager::findAdminByName(const std::string& name) {
    auto it = std::find_if(m_admins.begin(), m_admins.end(),
                           [&name](Admin& a) { return a.getName() == name; });
    return (it != m_admins.end()) ? &(*it) : nullptr;
}

const Admin* AdminManager::findAdminByNameConst(const std::string& name) const {
    auto it = std::find_if(m_admins.begin(), m_admins.end(),
                           [&name](const Admin& a) { return a.getName() == name; });
    return (it != m_admins.end()) ? &(*it) : nullptr;
}

bool AdminManager::adminExists(const std::string& name) const {
    return findAdminByNameConst(name) != nullptr;
}

const std::vector<Admin>& AdminManager::getAllAdmins() const {
    return m_admins;
}

size_t AdminManager::getAdminCount() const {
    return m_admins.size();
}
