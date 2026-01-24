#pragma once

#include "../models/Book.h"
#include <vector>
#include <algorithm>

class BookManager {
private:
    std::vector<Book> m_books;
    const std::string m_dataFile = "book.dat";

public:
    BookManager() = default;

    // File operations
    bool loadFromFile();
    bool saveToFile() const;

    // Book operations
    bool addBook(const Book& book);
    bool deleteBook(const std::string& id);
    Book* findBookById(const std::string& id);
    const Book* findBookByIdConst(const std::string& id) const;
    Book* findBookByName(const std::string& name);
    bool bookExists(const std::string& id) const;
    bool bookNameExists(const std::string& name) const;

    // Getters
    const std::vector<Book>& getAllBooks() const;
    size_t getBookCount() const;
    double getTotalPrice() const;

    // Operations
    void sortByPrice();
    void sortByName();
};
