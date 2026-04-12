#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../models/Admin.h"
#include "../models/User.h"

// Returns: 0=admin list, 1=create admin, 2=user list, 3=delete user,
//          4=clear books, 5=clear users, 6=back
int showSystemConfigMenu();

// Admin auth: returns {adminName, password}, empty name = cancelled
std::pair<std::string, std::string> showAdminAuthForm();

struct AdminCreateData {
    std::string name;
    std::string password;
    std::string confirmPassword;
    bool confirmed = false;
};

AdminCreateData showCreateAdminForm();

void showAdminListTable(const std::vector<Admin>& admins);

void showUserListTable(const std::vector<User>& users);

// Returns username to delete, empty = cancelled
std::string showDeleteUserForm();
