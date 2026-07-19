#pragma once

#include <optional>
#include <string>

struct sqlite3;

class Database {
private:
    sqlite3* m_connection = nullptr;
    std::string m_lastError;

public:
    Database() = default;
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool open(const std::string& path);
    bool initialize();
    bool execute(const std::string& sql);

    bool beginTransaction();
    bool commitTransaction();
    void rollbackTransaction();

    std::optional<std::string> getMetadata(const std::string& key);
    bool setMetadata(const std::string& key, const std::string& value);

    sqlite3* handle() const;
    const std::string& lastError() const;
    void setLastError(const std::string& error);
};
