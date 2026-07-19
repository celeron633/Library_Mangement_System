#pragma once

#include "../models/Admin.h"
#include "../database/Database.h"
#include <memory>
#include <vector>

class AdminManager {
private:
    std::vector<Admin> m_admins;
    std::shared_ptr<Database> m_database;

public:
    explicit AdminManager(std::shared_ptr<Database> database);

    // Persistence
    bool load();
    bool save() const;
    bool clearAll();

    // Admin operations
    bool addAdmin(const Admin& admin);
    bool deleteAdmin(const std::string& name);
    Admin* findAdminByName(const std::string& name);
    const Admin* findAdminByNameConst(const std::string& name) const;
    bool adminExists(const std::string& name) const;

    // Getters
    const std::vector<Admin>& getAllAdmins() const;
    size_t getAdminCount() const;
};
