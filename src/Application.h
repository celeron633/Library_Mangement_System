#pragma once

#include "managers/UserManager.h"
#include "managers/AdminManager.h"
#include "managers/BookManager.h"
#include "models/ShoppingCart.h"
#include <memory>

class Application {
private:
    std::unique_ptr<UserManager> m_userManager;
    std::unique_ptr<AdminManager> m_adminManager;
    std::unique_ptr<BookManager> m_bookManager;
    
    User* m_currentUser = nullptr;
    Admin* m_currentAdmin = nullptr;
    ShoppingCart m_cart;

    // Login menu
    void showLoginMenu();

    // Admin operations
    void adminLogin();
    void adminCreateAccount();
    void adminMenu();
    void adminBookOperations();
    void adminBookInquiry();
    void addBookUI();
    void deleteBookUI();
    void editBookUI();
    void editBookStockUI();
    void queryBookByIdUI();
    void showAllBooksUI();
    void sortBooksUI();
    void calculateTotalPriceUI();

    // User operations
    void userLogin();
    void userRegister();
    void userMenu();
    void userViewBooks();
    void userBuyBook();
    void userApplyVIP();
    void userChargeBalance();
    void userViewCart();

    // System configuration
    void systemConfig();
    void showAdminList();
    void showUserList();
    void deleteUserUI();
    void clearBookData();
    void clearUserData();

    // Helper methods
    bool verifyPassword(const std::string& inputPassword, const std::string& correctPassword);

public:
    Application();
    ~Application() = default;

    void run();
};
