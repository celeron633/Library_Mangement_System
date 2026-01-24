#include "Admin.h"
#include <sstream>

Admin::Admin() 
    : m_name(""), m_password("") {}

Admin::Admin(const std::string& name, const std::string& password)
    : m_name(name), m_password(password) {}

const std::string& Admin::getName() const {
    return m_name;
}

const std::string& Admin::getPassword() const {
    return m_password;
}

void Admin::setPassword(const std::string& password) {
    m_password = password;
}

std::string Admin::serialize() const {
    return m_name + " " + m_password;
}

Admin Admin::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string name, password;
    
    iss >> name >> password;
    
    return Admin(name, password);
}
