#pragma once

#include <string>
#include <vector>

#include "../models/Book.h"
#include "../models/ShoppingCart.h"

// Returns: 0=view books, 1=buy book, 2=apply VIP, 3=charge balance, 4=view cart, 5=back
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
