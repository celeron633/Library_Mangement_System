#pragma once

#include "../models/Book.h"
#include "../database/Database.h"
#include <vector>
#include <algorithm>
#include <memory>

class BookManager {
private:
    std::vector<Book> m_books;
    std::shared_ptr<Database> m_database;

public:
    explicit BookManager(std::shared_ptr<Database> database);

    // Persistence
    bool load();
    bool save() const;
    bool clearAll();

    // Book operations
    bool addBook(const Book& book);
    bool deleteBook(const std::string& id);
    Book* findBookById(const std::string& id);
    const Book* findBookByIdConst(const std::string& id) const;
    Book* findBookByName(const std::string& name);
    bool bookExists(const std::string& id) const;
    bool bookNameExists(const std::string& name) const;
    std::vector<Book> search(const std::string& keyword) const;
    std::vector<Book> getLowStockBooks(int threshold = 5) const;

    // Getters
    const std::vector<Book>& getAllBooks() const;
    size_t getBookCount() const;
    double getTotalPrice() const;

    // Operations
    void sortByPrice();
    void sortByName();
};
