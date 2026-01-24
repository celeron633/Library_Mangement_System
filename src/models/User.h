#pragma once

#include <string>
#include <iostream>

class User {
private:
    std::string m_name;
    std::string m_password;
    double m_balance;
    bool m_isVIP;

public:
    User();
    User(const std::string& name, const std::string& password, double balance = 0.0, bool isVIP = false);

    // Getters
    const std::string& getName() const;
    const std::string& getPassword() const;
    double getBalance() const;
    bool isVIP() const;

    // Setters
    void setPassword(const std::string& password);
    void setBalance(double balance);
    void addBalance(double amount);
    void deductBalance(double amount);
    void setVIP(bool isVIP);

    // Serialization
    std::string serialize() const;
    static User deserialize(const std::string& line);
};
