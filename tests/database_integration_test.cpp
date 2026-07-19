#include "src/database/Database.h"
#include "src/database/LegacyDataMigrator.h"
#include "src/managers/AdminManager.h"
#include "src/managers/BookManager.h"
#include "src/managers/PurchaseManager.h"
#include "src/managers/UserManager.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

int main() {
    const auto unique = std::to_string(
        std::chrono::steady_clock::now().time_since_epoch().count());
    const auto testDirectory =
        std::filesystem::temp_directory_path() /
        ("library_management_test_" + unique);
    assert(std::filesystem::create_directory(testDirectory));
    const auto databasePath = testDirectory / "library.db";

    {
        auto database = std::make_shared<Database>();
        assert(database->open(databasePath.string()));
        assert(database->initialize());

        AdminManager admins(database);
        UserManager users(database);
        BookManager books(database);
        PurchaseManager purchases(database);

        assert(admins.load());
        assert(users.load());
        assert(books.load());
        assert(admins.addAdmin(Admin("admin", "secret")));
        assert(users.addUser(User("reader", "password", 200.0, true)));
        assert(books.addBook(Book("B001", "现代 C++", "作者甲", "测试简介",
                                  50.0, 3)));
        assert(books.addBook(
            Book("B002", "数据库基础", "作者乙", "测试简介", 60.0, 10)));
        assert(admins.save());
        assert(users.save());
        assert(books.save());

        AdminManager reloadedAdmins(database);
        UserManager reloadedUsers(database);
        BookManager reloadedBooks(database);
        assert(reloadedAdmins.load());
        assert(reloadedUsers.load());
        assert(reloadedBooks.load());
        assert(reloadedAdmins.getAdminCount() == 1);
        assert(reloadedUsers.getUserCount() == 1);
        assert(reloadedBooks.getBookCount() == 2);
        assert(reloadedBooks.search("数据库").size() == 1);
        assert(reloadedBooks.search("b001").size() == 1);
        assert(reloadedBooks.getLowStockBooks().size() == 1);

        User* user = reloadedUsers.findUserByName("reader");
        Book* book = reloadedBooks.findBookById("B001");
        assert(user && book);
        user->deductBalance(90.0);
        assert(book->deductStock(2));

        Purchase purchase;
        purchase.userName = user->getName();
        purchase.bookId = book->getId();
        purchase.bookName = book->getName();
        purchase.unitPrice = book->getPrice();
        purchase.quantity = 2;
        purchase.discountRate = 0.9;
        purchase.total = 90.0;
        assert(purchases.recordPurchase(purchase, *user, *book));

        assert(reloadedUsers.load());
        assert(reloadedBooks.load());
        assert(reloadedUsers.findUserByName("reader")->getBalance() == 110.0);
        assert(reloadedBooks.findBookById("B001")->getStock() == 1);
        const auto history = purchases.getByUser("reader");
        assert(history.size() == 1);
        assert(history.front().total == 90.0);
        assert(history.front().quantity == 2);
    }

    {
        std::ofstream(testDirectory / "admin.dat") << "legacy_admin secret\n";
        std::ofstream(testDirectory / "user.dat")
            << "legacy_user password 88.50 1\n";
        std::ofstream(testDirectory / "book.dat")
            << "B100 LegacyBook LegacyAuthor LegacyIntro 25.00 4\n";

        auto database = std::make_shared<Database>();
        assert(database->open((testDirectory / "migration.db").string()));
        assert(database->initialize());
        AdminManager admins(database);
        UserManager users(database);
        BookManager books(database);
        assert(admins.load());
        assert(users.load());
        assert(books.load());

        LegacyDataMigrator migrator(*database, admins, users, books);
        assert(migrator.migrate(testDirectory));
        assert(admins.getAdminCount() == 1);
        assert(users.getUserCount() == 1);
        assert(books.getBookCount() == 1);
        assert(database->getMetadata("legacy_dat_migration_v1") == "completed");

        std::ofstream(testDirectory / "admin.dat", std::ios::app)
            << "should_not_import secret\n";
        assert(migrator.migrate(testDirectory));
        assert(admins.getAdminCount() == 1);
    }

    std::error_code error;
    std::filesystem::remove_all(testDirectory, error);
    assert(!error);
    return 0;
}
