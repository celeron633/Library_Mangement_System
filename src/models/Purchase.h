#pragma once

#include <cstdint>
#include <string>

struct Purchase {
    std::int64_t id = 0;
    std::string userName;
    std::string bookId;
    std::string bookName;
    double unitPrice = 0.0;
    int quantity = 0;
    double discountRate = 1.0;
    double total = 0.0;
    std::string purchasedAt;
};
