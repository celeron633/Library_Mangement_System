#pragma once

#include <string>
#include <utility>

// Returns: 0=admin login, 1=user login, 2=user register, 3=system config, 4=exit
int showMainMenu();

// Returns {username, password}; empty username means cancelled
std::pair<std::string, std::string> showLoginForm(const std::string& title);

// Returns password; empty string means cancelled
std::string showPasswordInputForm(const std::string& title);

struct RegisterData {
    std::string username;
    std::string password;
    std::string confirmPassword;
    bool confirmed = false;
};

RegisterData showRegisterForm();
