#include "ShoppingCart.h"

void ShoppingCart::addItem(const Book& book, int quantity) {
    if (quantity > 0) {
        m_items.emplace_back(book, quantity);
    }
}

void ShoppingCart::removeItem(size_t index) {
    if (index < m_items.size()) {
        m_items.erase(m_items.begin() + index);
    }
}

void ShoppingCart::clear() {
    m_items.clear();
}

const std::vector<CartItem>& ShoppingCart::getItems() const {
    return m_items;
}

double ShoppingCart::getTotalPrice() const {
    double total = 0.0;
    for (const auto& item : m_items) {
        total += item.getSubtotal();
    }
    return total;
}

double ShoppingCart::getTotalPriceWithDiscount(double discount) const {
    return getTotalPrice() * discount;
}

bool ShoppingCart::isEmpty() const {
    return m_items.empty();
}
