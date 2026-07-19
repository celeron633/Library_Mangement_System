#include "Book.h"

Book::Book() 
    : m_id(""), m_name(""), m_author(""), m_introduction(""), m_price(0.0), m_stock(0) {}

Book::Book(const std::string& id, const std::string& name, const std::string& author,
           const std::string& introduction, double price, int stock)
    : m_id(id), m_name(name), m_author(author), m_introduction(introduction),
      m_price(price), m_stock(stock) {}

const std::string& Book::getId() const {
    return m_id;
}

const std::string& Book::getName() const {
    return m_name;
}

const std::string& Book::getAuthor() const {
    return m_author;
}

const std::string& Book::getIntroduction() const {
    return m_introduction;
}

double Book::getPrice() const {
    return m_price;
}

int Book::getStock() const {
    return m_stock;
}

void Book::setId(const std::string& id) {
    m_id = id;
}

void Book::setName(const std::string& name) {
    m_name = name;
}

void Book::setAuthor(const std::string& author) {
    m_author = author;
}

void Book::setIntroduction(const std::string& introduction) {
    m_introduction = introduction;
}

void Book::setPrice(double price) {
    if (price >= 0) {
        m_price = price;
    }
}

void Book::setStock(int stock) {
    if (stock >= 0) {
        m_stock = stock;
    }
}

void Book::addStock(int count) {
    if (count > 0) {
        m_stock += count;
    }
}

bool Book::deductStock(int count) {
    if (count > 0 && m_stock >= count) {
        m_stock -= count;
        return true;
    }
    return false;
}

std::string Book::serialize() const {
    std::ostringstream oss;
    oss << m_id << " " << m_name << " " << m_author << " "
        << m_introduction << " " << std::fixed << std::setprecision(2) << m_price
        << " " << m_stock;
    return oss.str();
}

Book Book::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string id, name, author, introduction;
    double price = 0.0;
    int stock = 0;
    
    iss >> id >> name >> author >> introduction >> price >> stock;
    
    return Book(id, name, author, introduction, price, stock);
}
