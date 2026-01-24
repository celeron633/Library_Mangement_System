#include "BookManager.h"
#include <fstream>
#include <iostream>

bool BookManager::loadFromFile() {
    std::ifstream file(m_dataFile);
    if (!file.is_open()) {
        std::cerr << "无法打开图书数据文件: " << m_dataFile << "\n";
        return false;
    }

    m_books.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            m_books.push_back(Book::deserialize(line));
        }
    }
    file.close();
    return true;
}

bool BookManager::saveToFile() const {
    std::ofstream file(m_dataFile, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "无法创建图书数据文件: " << m_dataFile << "\n";
        return false;
    }

    for (const auto& book : m_books) {
        file << book.serialize() << "\n";
    }
    file.close();
    return true;
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
