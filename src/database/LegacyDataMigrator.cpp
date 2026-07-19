#include "LegacyDataMigrator.h"

#include <fstream>
#include <string>

LegacyDataMigrator::LegacyDataMigrator(Database& database,
                                       AdminManager& admins,
                                       UserManager& users, BookManager& books)
    : m_database(database),
      m_admins(admins),
      m_users(users),
      m_books(books) {}

bool LegacyDataMigrator::migrate(const std::filesystem::path& directory) {
    if (m_database.getMetadata("legacy_dat_migration_v1").has_value()) {
        return true;
    }

    std::string line;
    const auto adminPath = directory / "admin.dat";
    if (m_admins.getAdminCount() == 0 && std::filesystem::exists(adminPath)) {
        std::ifstream file(adminPath);
        if (!file) {
            m_database.setLastError("无法读取旧版管理员数据");
            return false;
        }
        while (std::getline(file, line)) {
            Admin admin = Admin::deserialize(line);
            if (!admin.getName().empty()) m_admins.addAdmin(admin);
        }
        if (!m_admins.save()) return false;
    }

    const auto userPath = directory / "user.dat";
    if (m_users.getUserCount() == 0 && std::filesystem::exists(userPath)) {
        std::ifstream file(userPath);
        if (!file) {
            m_database.setLastError("无法读取旧版用户数据");
            return false;
        }
        while (std::getline(file, line)) {
            User user = User::deserialize(line);
            if (!user.getName().empty()) m_users.addUser(user);
        }
        if (!m_users.save()) return false;
    }

    const auto bookPath = directory / "book.dat";
    if (m_books.getBookCount() == 0 && std::filesystem::exists(bookPath)) {
        std::ifstream file(bookPath);
        if (!file) {
            m_database.setLastError("无法读取旧版书籍数据");
            return false;
        }
        while (std::getline(file, line)) {
            Book book = Book::deserialize(line);
            if (!book.getId().empty()) m_books.addBook(book);
        }
        if (!m_books.save()) return false;
    }

    return m_database.setMetadata("legacy_dat_migration_v1", "completed");
}
