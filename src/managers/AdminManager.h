#pragma once

#include "../models/Admin.h"
#include <vector>

class AdminManager {
private:
    std::vector<Admin> m_admins;
    const std::string m_dataFile = "admin.dat";

public:
    AdminManager() = default;

    // File operations
    bool loadFromFile();
    bool saveToFile() const;
    bool dataFileExists() const;

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
