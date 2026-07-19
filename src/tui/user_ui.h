#pragma once

#include <string>
#include <vector>

#include "../models/Book.h"
#include "../models/Purchase.h"
#include "../models/ShoppingCart.h"

// Returns: 0=view books, 1=search, 2=buy, 3=VIP, 4=charge, 5=cart,
//          6=purchase history, 7=change password, 8=back
int showUserMenu(const std::string& username, double balance, bool isVIP);

struct BuyBookData {
    std::string bookId;
    int quantity = 0;
    bool confirmed = false;
};

BuyBookData showBuyBookForm(const std::vector<Book>& books);

// Returns the amount to charge, 0 = cancelled
double showChargeBalanceForm();

void showCartView(const ShoppingCart& cart, bool isVIP);

void showPurchaseHistory(const std::vector<Purchase>& purchases,
                         const std::string& title);
