#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../database/Database.h"
#include "../models/Book.h"
#include "../models/Purchase.h"
#include "../models/User.h"

class PurchaseManager {
private:
    std::shared_ptr<Database> m_database;

public:
    explicit PurchaseManager(std::shared_ptr<Database> database);

    bool recordPurchase(const Purchase& purchase, const User& user,
                        const Book& book);
    std::vector<Purchase> getByUser(const std::string& username) const;
    std::vector<Purchase> getAll() const;
    bool clearAll();
};
