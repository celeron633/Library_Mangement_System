#pragma once

#include "Book.h"
#include <vector>

struct CartItem {
    Book book;
    int quantity;

    CartItem(const Book& b, int q) : book(b), quantity(q) {}

    double getSubtotal() const {
        return book.getPrice() * quantity;
    }
};

class ShoppingCart {
private:
    std::vector<CartItem> m_items;

public:
    ShoppingCart() = default;

    void addItem(const Book& book, int quantity);
    void removeItem(size_t index);
    void clear();

    const std::vector<CartItem>& getItems() const;
    double getTotalPrice() const;
    double getTotalPriceWithDiscount(double discount = 1.0) const;
    bool isEmpty() const;
};
