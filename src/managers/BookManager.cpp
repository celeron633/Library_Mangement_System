#include "BookManager.h"
#include <cctype>
#include <iterator>
#include <sqlite3.h>

namespace {

std::string lowerAscii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char ch) {
                       return static_cast<char>(std::tolower(ch));
                   });
    return value;
}

}  // namespace

BookManager::BookManager(std::shared_ptr<Database> database)
    : m_database(std::move(database)) {}

bool BookManager::load() {
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(
            m_database->handle(),
            "SELECT id, name, author, introduction, price, stock "
            "FROM books ORDER BY id;",
            -1, &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }

    m_books.clear();
    int result = SQLITE_ROW;
    while ((result = sqlite3_step(statement)) == SQLITE_ROW) {
        const auto textAt = [&](int column) {
            const auto* value = sqlite3_column_text(statement, column);
            return value ? std::string(reinterpret_cast<const char*>(value))
                         : std::string();
        };
        m_books.emplace_back(textAt(0), textAt(1), textAt(2), textAt(3),
                             sqlite3_column_double(statement, 4),
                             sqlite3_column_int(statement, 5));
    }

    sqlite3_finalize(statement);
    if (result != SQLITE_DONE) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        return false;
    }
    return true;
}

bool BookManager::save() const {
    if (!m_database->beginTransaction()) return false;
    if (!m_database->execute("DELETE FROM books;")) {
        m_database->rollbackTransaction();
        return false;
    }

    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(
            m_database->handle(),
            "INSERT INTO books(id, name, author, introduction, price, stock) "
            "VALUES(?, ?, ?, ?, ?, ?);",
            -1, &statement, nullptr) != SQLITE_OK) {
        m_database->setLastError(sqlite3_errmsg(m_database->handle()));
        m_database->rollbackTransaction();
        return false;
    }

    bool success = true;
    for (const auto& book : m_books) {
        sqlite3_bind_text(statement, 1, book.getId().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 2, book.getName().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 3, book.getAuthor().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(statement, 4, book.getIntroduction().c_str(), -1,
                          SQLITE_TRANSIENT);
        sqlite3_bind_double(statement, 5, book.getPrice());
        sqlite3_bind_int(statement, 6, book.getStock());
        if (sqlite3_step(statement) != SQLITE_DONE) {
            success = false;
            m_database->setLastError(sqlite3_errmsg(m_database->handle()));
            break;
        }
        sqlite3_reset(statement);
        sqlite3_clear_bindings(statement);
    }
    sqlite3_finalize(statement);

    if (!success) {
        m_database->rollbackTransaction();
        return false;
    }
    if (!m_database->commitTransaction()) {
        m_database->rollbackTransaction();
        return false;
    }
    return true;
}

bool BookManager::clearAll() {
    m_books.clear();
    return m_database->execute("DELETE FROM books;");
}

bool BookManager::addBook(const Book& book) {
    if (bookExists(book.getId())) {
        return false;
    }
    m_books.push_back(book);
    return true;
}

bool BookManager::deleteBook(const std::string& id) {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&id](const Book& b) { return b.getId() == id; });
    if (it != m_books.end()) {
        m_books.erase(it);
        return true;
    }
    return false;
}

Book* BookManager::findBookById(const std::string& id) {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&id](Book& b) { return b.getId() == id; });
    return (it != m_books.end()) ? &(*it) : nullptr;
}

const Book* BookManager::findBookByIdConst(const std::string& id) const {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&id](const Book& b) { return b.getId() == id; });
    return (it != m_books.end()) ? &(*it) : nullptr;
}

Book* BookManager::findBookByName(const std::string& name) {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&name](Book& b) { return b.getName() == name; });
    return (it != m_books.end()) ? &(*it) : nullptr;
}

bool BookManager::bookExists(const std::string& id) const {
    return findBookByIdConst(id) != nullptr;
}

bool BookManager::bookNameExists(const std::string& name) const {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                           [&name](const Book& b) { return b.getName() == name; });
    return it != m_books.end();
}

std::vector<Book> BookManager::search(const std::string& keyword) const {
    std::vector<Book> matches;
    const std::string needle = lowerAscii(keyword);
    for (const auto& book : m_books) {
        if (lowerAscii(book.getId()).find(needle) != std::string::npos ||
            lowerAscii(book.getName()).find(needle) != std::string::npos ||
            lowerAscii(book.getAuthor()).find(needle) != std::string::npos) {
            matches.push_back(book);
        }
    }
    return matches;
}

std::vector<Book> BookManager::getLowStockBooks(int threshold) const {
    std::vector<Book> matches;
    std::copy_if(m_books.begin(), m_books.end(), std::back_inserter(matches),
                 [threshold](const Book& book) {
                     return book.getStock() <= threshold;
                 });
    return matches;
}

const std::vector<Book>& BookManager::getAllBooks() const {
    return m_books;
}

size_t BookManager::getBookCount() const {
    return m_books.size();
}

double BookManager::getTotalPrice() const {
    double total = 0.0;
    for (const auto& book : m_books) {
        total += book.getPrice() * book.getStock();
    }
    return total;
}

void BookManager::sortByPrice() {
    std::sort(m_books.begin(), m_books.end(),
              [](const Book& a, const Book& b) { return a.getPrice() < b.getPrice(); });
}

void BookManager::sortByName() {
    std::sort(m_books.begin(), m_books.end(),
              [](const Book& a, const Book& b) { return a.getName() < b.getName(); });
}
