#pragma once

#include <string>

class Admin {
private:
    std::string m_name;
    std::string m_password;

public:
    Admin();
    Admin(const std::string& name, const std::string& password);

    // Getters
    const std::string& getName() const;
    const std::string& getPassword() const;

    // Setters
    void setPassword(const std::string& password);

    // Serialization
    std::string serialize() const;
    static Admin deserialize(const std::string& line);
};
