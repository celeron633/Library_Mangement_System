#pragma once

#include <filesystem>

#include "Database.h"
#include "../managers/AdminManager.h"
#include "../managers/BookManager.h"
#include "../managers/UserManager.h"

class LegacyDataMigrator {
private:
    Database& m_database;
    AdminManager& m_admins;
    UserManager& m_users;
    BookManager& m_books;

public:
    LegacyDataMigrator(Database& database, AdminManager& admins,
                       UserManager& users, BookManager& books);

    bool migrate(const std::filesystem::path& directory);
};
