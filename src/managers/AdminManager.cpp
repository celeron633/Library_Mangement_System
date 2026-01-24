#include "AdminManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

bool AdminManager::loadFromFile() {
    std::ifstream file(m_dataFile);
    if (!file.is_open()) {
        std::cerr << "无法打开管理员数据文件: " << m_dataFile << "\n";
        return false;
    }

    m_admins.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            m_admins.push_back(Admin::deserialize(line));
        }
    }
    file.close();
    return true;
}

bool AdminManager::saveToFile() const {
    std::ofstream file(m_dataFile, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "无法创建管理员数据文件: " << m_dataFile << "\n";
        return false;
    }

    for (const auto& admin : m_admins) {
        file << admin.serialize() << "\n";
    }
    file.close();
    return true;
}

bool AdminManager::dataFileExists() const {
    std::ifstream file(m_dataFile);
    return file.good();
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
