#pragma once

#include <string>
#include <iomanip>
#include <sstream>

class Book {
private:
    std::string m_id;
    std::string m_name;
    std::string m_author;
    std::string m_introduction;
    double m_price;
    int m_stock;

public:
    Book();
    Book(const std::string& id, const std::string& name, const std::string& author,
         const std::string& introduction, double price, int stock);

    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getAuthor() const;
    const std::string& getIntroduction() const;
    double getPrice() const;
    int getStock() const;

    // Setters
    void setId(const std::string& id);
    void setName(const std::string& name);
    void setAuthor(const std::string& author);
    void setIntroduction(const std::string& introduction);
    void setPrice(double price);
    void setStock(int stock);

    // Stock operations
    void addStock(int count);
    bool deductStock(int count);

    // Serialization
    std::string serialize() const;
    static Book deserialize(const std::string& line);
};
