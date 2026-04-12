#pragma once

#include <string>
#include <utility>
#include <vector>

#include "../models/Book.h"

// Menu returns: 0=show all books, 1=book operations, 2=book inquiry, 3=back
int showAdminMenu();

// Returns: 0=add, 1=delete, 2=edit info, 3=edit stock, 4=back
int showAdminBookOperationsMenu();

// Returns: 0=sort by price, 1=total price, 2=query intro, 3=back
int showAdminBookInquiryMenu();

struct AddBookData {
    std::string id;
    std::string name;
    std::string author;
    std::string intro;
    double price = 0.0;
    int stock = 0;
    bool confirmed = false;
};

AddBookData showAddBookForm();

// Returns the book ID to delete, empty = cancelled
std::string showBookIdInputForm(const std::string& title, const std::string& label);

struct EditBookData {
    std::string name;
    std::string author;
    std::string intro;
    double price = 0.0;
    int stock = 0;
    bool confirmed = false;
};

EditBookData showEditBookForm(const Book& book);

// Returns {bookId, newStock}; empty id = cancelled
std::pair<std::string, int> showEditStockForm();

void showBooksTable(const std::vector<Book>& books, const std::string& title);

void showBookIntroDialog(const Book& book);

void showTotalPriceDialog(double total);
