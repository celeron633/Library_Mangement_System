#include "User.h"
#include <sstream>
#include <iomanip>

User::User() 
    : m_name(""), m_password(""), m_balance(0.0), m_isVIP(false) {}

User::User(const std::string& name, const std::string& password, double balance, bool isVIP)
    : m_name(name), m_password(password), m_balance(balance), m_isVIP(isVIP) {}

const std::string& User::getName() const {
    return m_name;
}

const std::string& User::getPassword() const {
    return m_password;
}

double User::getBalance() const {
    return m_balance;
}

bool User::isVIP() const {
    return m_isVIP;
}

void User::setPassword(const std::string& password) {
    m_password = password;
}

void User::setBalance(double balance) {
    m_balance = balance;
}

void User::addBalance(double amount) {
    if (amount >= 0) {
        m_balance += amount;
    }
}

void User::deductBalance(double amount) {
    if (amount >= 0 && m_balance >= amount) {
        m_balance -= amount;
    }
}

void User::setVIP(bool isVIP) {
    m_isVIP = isVIP;
}

std::string User::serialize() const {
    std::ostringstream oss;
    oss << m_name << " " << m_password << " " 
        << std::fixed << std::setprecision(2) << m_balance << " "
        << (m_isVIP ? "1" : "0");
    return oss.str();
}

User User::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string name, password, vipStr;
    double balance = 0.0;
    
    iss >> name >> password >> balance >> vipStr;
    
    User user(name, password, balance, vipStr == "1");
    return user;
}
