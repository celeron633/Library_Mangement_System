#include "PurchaseManager.h"

#include <sqlite3.h>

namespace {

std::vector<Purchase> queryPurchases(const std::shared_ptr<Database>& database,
                                     const char* sql,
                                     const std::string* username) {
    std::vector<Purchase> purchases;
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(database->handle(), sql, -1, &statement, nullptr) !=
        SQLITE_OK) {
        database->setLastError(sqlite3_errmsg(database->handle()));
        return purchases;
    }

    if (username) {
        sqlite3_bind_text(statement, 1, username->c_str(), -1,
                          SQLITE_TRANSIENT);
    }

    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        Purchase purchase;
        purchase.id = sqlite3_column_int64(statement, 0);
        purchase.userName = reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 1));
        purchase.bookId = reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 2));
        purchase.bookName = reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 3));
        purchase.unitPrice = sqlite3_column_double(statement, 4);
        purchase.quantity = sqlite3_column_int(statement, 5);
        purchase.discountRate = sqlite3_column_double(statement, 6);
        purchase.total = sqlite3_column_double(statement, 7);
        purchase.purchasedAt = reinterpret_cast<const char*>(
            sqlite3_column_text(statement, 8));
        purchases.push_back(std::move(purchase));
    }

    sqlite3_finalize(statement);
    if (result != SQLITE_DONE) {
        database->setLastError(sqlite3_errmsg(database->handle()));
        purchases.clear();
    }
    return purchases;
}

}  // namespace

PurchaseManager::PurchaseManager(std::shared_ptr<Database> database)
    : m_database(std::move(database)) {}

bool PurchaseManager::recordPurchase(const Purchase& purchase, const User& user,
                                     const Book& book) {
    if (!m_database->beginTransaction()) return false;

    sqlite3_stmt* user_statement = nullptr;
    if (sqlite3_prepare_v2(m_database->handle(),
                           "UPDATE users SET balance = ?, is_vip = ? WHERE name = ?;",
                           -1, &user_statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        m_database->rollbackTransaction();
        return false;
    }
    sqlite3_bind_double(user_statement, 1, user.getBalance());
    sqlite3_bind_int(user_statement, 2, user.isVIP() ? 1 : 0);
    sqlite3_bind_text(user_statement, 3, user.getName().c_str(), -1,
                      SQLITE_TRANSIENT);
    bool success = sqlite3_step(user_statement) == SQLITE_DONE &&
                   sqlite3_changes(m_database->handle()) == 1;
    sqlite3_finalize(user_statement);

    sqlite3_stmt* book_statement = nullptr;
    if (success &&
        sqlite3_prepare_v2(m_database->handle(),
                           "UPDATE books SET stock = ? WHERE id = ?;", -1,
                           &book_statement, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(book_statement, 1, book.getStock());
        sqlite3_bind_text(book_statement, 2, book.getId().c_str(), -1,
                          SQLITE_TRANSIENT);
        success = sqlite3_step(book_statement) == SQLITE_DONE &&
                  sqlite3_changes(m_database->handle()) == 1;
        sqlite3_finalize(book_statement);
    } else if (success) {
        success = false;
    }

    sqlite3_stmt* purchase_statement = nullptr;
    if (success &&
        sqlite3_prepare_v2(
            m_database->handle(),
            "INSERT INTO purchases(user_name, book_id, book_name, unit_price, "
            "quantity, discount_rate, total) VALUES(?, ?, ?, ?, ?, ?, ?);",
            -1, &purchase_statement, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(purchase_statement, 1, purchase.userName.c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(purchase_statement, 2, purchase.bookId.c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(purchase_statement, 3, purchase.bookName.c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_double(purchase_statement, 4, purchase.unitPrice);
        sqlite3_bind_int(purchase_statement, 5, purchase.quantity);
        sqlite3_bind_double(purchase_statement, 6, purchase.discountRate);
        sqlite3_bind_double(purchase_statement, 7, purchase.total);
        success = sqlite3_step(purchase_statement) == SQLITE_DONE;
        sqlite3_finalize(purchase_statement);
    } else if (success) {
        success = false;
    }

    if (!success) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        m_database->rollbackTransaction();
        return false;
    }

    if (!m_database->commitTransaction()) {
        m_database->rollbackTransaction();
        return false;
    }
    return true;
}

std::vector<Purchase> PurchaseManager::getByUser(
    const std::string& username) const {
    static const char* sql = R"SQL(
        SELECT id, user_name, book_id, book_name, unit_price, quantity,
               discount_rate, total, purchased_at
        FROM purchases WHERE user_name = ?
        ORDER BY purchased_at DESC, id DESC;
    )SQL";
    return queryPurchases(m_database, sql, &username);
}

std::vector<Purchase> PurchaseManager::getAll() const {
    static const char* sql = R"SQL(
        SELECT id, user_name, book_id, book_name, unit_price, quantity,
               discount_rate, total, purchased_at
        FROM purchases ORDER BY purchased_at DESC, id DESC;
    )SQL";
    return queryPurchases(m_database, sql, nullptr);
}

bool PurchaseManager::clearAll() {
    return m_database->execute("DELETE FROM purchases;");
}
