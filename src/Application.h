#pragma once

#include "managers/UserManager.h"
#include "managers/AdminManager.h"
#include "managers/BookManager.h"
#include "managers/PurchaseManager.h"
#include "models/ShoppingCart.h"
#include "database/Database.h"
#include <memory>

class Application {
private:
    std::shared_ptr<Database> m_database;
    std::unique_ptr<UserManager> m_userManager;
    std::unique_ptr<AdminManager> m_adminManager;
    std::unique_ptr<BookManager> m_bookManager;
    std::unique_ptr<PurchaseManager> m_purchaseManager;
    
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
    void searchBooksUI();
    void showLowStockBooksUI();
    void showAllPurchaseHistoryUI();
    void adminChangePassword();

    // User operations
    void userLogin();
    void userRegister();
    void userMenu();
    void userViewBooks();
    void userBuyBook();
    void userApplyVIP();
    void userChargeBalance();
    void userViewCart();
    void userViewPurchaseHistory();
    void userChangePassword();

    // System configuration
    void systemConfig();
    void showAdminList();
    void showUserList();
    void deleteUserUI();
    void clearBookData();
    void clearUserData();

    // Helper methods
    bool verifyPassword(const std::string& inputPassword, const std::string& correctPassword);
    bool initializePersistence();
    void showPersistenceError(const std::string& action);

public:
    Application();
    ~Application() = default;

    void run();
};
