#include "Database.h"

#include <sqlite3.h>

Database::~Database() {
    if (m_connection) {
        sqlite3_close(m_connection);
    }
}

bool Database::open(const std::string& path) {
    if (m_connection) {
        sqlite3_close(m_connection);
        m_connection = nullptr;
    }

    if (sqlite3_open(path.c_str(), &m_connection) != SQLITE_OK) {
        m_lastError = m_connection ? sqlite3_errmsg(m_connection)
                                   : "无法创建数据库连接";
        return false;
    }

    sqlite3_busy_timeout(m_connection, 3000);
    return true;
}

bool Database::initialize() {
    static const char* schema = R"SQL(
        PRAGMA foreign_keys = ON;
        PRAGMA journal_mode = WAL;

        CREATE TABLE IF NOT EXISTS admins (
            name TEXT PRIMARY KEY,
            password TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS users (
            name TEXT PRIMARY KEY,
            password TEXT NOT NULL,
            balance REAL NOT NULL DEFAULT 0 CHECK (balance >= 0),
            is_vip INTEGER NOT NULL DEFAULT 0 CHECK (is_vip IN (0, 1))
        );

        CREATE TABLE IF NOT EXISTS books (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL UNIQUE,
            author TEXT NOT NULL,
            introduction TEXT NOT NULL,
            price REAL NOT NULL CHECK (price >= 0),
            stock INTEGER NOT NULL CHECK (stock >= 0)
        );

        CREATE TABLE IF NOT EXISTS purchases (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_name TEXT NOT NULL,
            book_id TEXT NOT NULL,
            book_name TEXT NOT NULL,
            unit_price REAL NOT NULL,
            quantity INTEGER NOT NULL CHECK (quantity > 0),
            discount_rate REAL NOT NULL DEFAULT 1.0,
            total REAL NOT NULL,
            purchased_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        );

        CREATE INDEX IF NOT EXISTS idx_purchases_user_time
            ON purchases(user_name, purchased_at DESC, id DESC);

        CREATE TABLE IF NOT EXISTS app_metadata (
            key TEXT PRIMARY KEY,
            value TEXT NOT NULL
        );
    )SQL";

    return execute(schema);
}

bool Database::execute(const std::string& sql) {
    char* error = nullptr;
    const int result = sqlite3_exec(m_connection, sql.c_str(), nullptr, nullptr,
                                    &error);
    if (result != SQLITE_OK) {
        m_lastError = error ? error : sqlite3_errmsg(m_connection);
        sqlite3_free(error);
        return false;
    }
    return true;
}

bool Database::beginTransaction() {
    return execute("BEGIN IMMEDIATE TRANSACTION;");
}

bool Database::commitTransaction() {
    return execute("COMMIT;");
}

void Database::rollbackTransaction() {
    execute("ROLLBACK;");
}

std::optional<std::string> Database::getMetadata(const std::string& key) {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(m_connection,
                           "SELECT value FROM app_metadata WHERE key = ?;", -1,
                           &statement, nullptr) != SQLITE_OK) {
        m_lastError = sqlite3_errmsg(m_connection);
        return std::nullopt;
    }

    sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    std::optional<std::string> value;
    if (sqlite3_step(statement) == SQLITE_ROW) {
        const auto* text = sqlite3_column_text(statement, 0);
        value = text ? reinterpret_cast<const char*>(text) : "";
    }
    sqlite3_finalize(statement);
    return value;
}

bool Database::setMetadata(const std::string& key, const std::string& value) {
    sqlite3_stmt* statement = nullptr;
    const char* sql = R"SQL(
        INSERT INTO app_metadata(key, value) VALUES(?, ?)
        ON CONFLICT(key) DO UPDATE SET value = excluded.value;
    )SQL";
    if (sqlite3_prepare_v2(m_connection, sql, -1, &statement, nullptr) !=
        SQLITE_OK) {
        m_lastError = sqlite3_errmsg(m_connection);
        return false;
    }

    sqlite3_bind_text(statement, 1, key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, value.c_str(), -1, SQLITE_TRANSIENT);
    const bool success = sqlite3_step(statement) == SQLITE_DONE;
    if (!success) m_lastError = sqlite3_errmsg(m_connection);
    sqlite3_finalize(statement);
    return success;
}

sqlite3* Database::handle() const {
    return m_connection;
}

const std::string& Database::lastError() const {
    return m_lastError;
}

void Database::setLastError(const std::string& error) {
    m_lastError = error;
}
