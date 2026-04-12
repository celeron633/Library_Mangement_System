#include "Application.h"

#include "tui/admin_ui.h"
#include "tui/common_ui.h"
#include "tui/login_ui.h"
#include "tui/system_ui.h"
#include "tui/user_ui.h"

#include <fstream>

Application::Application()
    : m_userManager(std::make_unique<UserManager>()),
      m_adminManager(std::make_unique<AdminManager>()),
      m_bookManager(std::make_unique<BookManager>()) {
}

void Application::run() {
    m_adminManager->loadFromFile();
    m_userManager->loadFromFile();
    m_bookManager->loadFromFile();

    if (m_adminManager->getAdminCount() == 0) {
        showMessage("欢迎使用图书馆管理系统",
                    "系统中没有管理员账号，需要创建第一个管理员");
        adminCreateAccount();
    }

    showLoginMenu();
}

void Application::showLoginMenu() {
    while (true) {
        // 0=admin login, 1=user login, 2=register, 3=system config, 4=exit
        int choice = showMainMenu();
        switch (choice) {
            case 0:
                adminLogin();
                break;
            case 1:
                userLogin();
                break;
            case 2:
                userRegister();
                break;
            case 3:
                systemConfig();
                break;
            case 4:
                showMessage("", "谢谢你的使用，再见！");
                return;
            default:
                break;
        }
    }
}

void Application::adminLogin() {
    auto [name, password] = showLoginForm("管理员登录");
    if (name.empty()) return;

    m_currentAdmin = m_adminManager->findAdminByName(name);
    if (!m_currentAdmin) {
        showMessage("错误", "该管理员不存在", MessageType::ERROR_MSG);
        return;
    }

    if (verifyPassword(password, m_currentAdmin->getPassword())) {
        showMessage("成功", "登录成功！", MessageType::SUCCESS);
        adminMenu();
    } else {
        showMessage("错误", "密码错误", MessageType::ERROR_MSG);
    }

    m_currentAdmin = nullptr;
}

void Application::adminCreateAccount() {
    while (true) {
        AdminCreateData data = showCreateAdminForm();
        if (!data.confirmed) break;

        if (m_adminManager->adminExists(data.name)) {
            showMessage("错误", "该管理员已存在", MessageType::ERROR_MSG);
            continue;
        }

        if (data.password != data.confirmPassword) {
            showMessage("错误", "两次输入的密码不一致", MessageType::ERROR_MSG);
            continue;
        }

        m_adminManager->addAdmin(Admin(data.name, data.password));
        m_adminManager->saveToFile();
        showMessage("成功", "管理员创建成功！", MessageType::SUCCESS);

        if (!showConfirmDialog("继续创建管理员？")) break;
    }
}

void Application::adminMenu() {
    while (true) {
        // 0=all books, 1=operations, 2=inquiry, 3=back
        int choice = showAdminMenu();
        switch (choice) {
            case 0:
                showAllBooksUI();
                break;
            case 1:
                adminBookOperations();
                break;
            case 2:
                adminBookInquiry();
                break;
            case 3:
                return;
            default:
                break;
        }
    }
}

void Application::adminBookOperations() {
    while (true) {
        // 0=add, 1=delete, 2=edit info, 3=edit stock, 4=back
        int choice = showAdminBookOperationsMenu();
        switch (choice) {
            case 0:
                addBookUI();
                break;
            case 1:
                deleteBookUI();
                break;
            case 2:
                editBookUI();
                break;
            case 3:
                editBookStockUI();
                break;
            case 4:
                return;
            default:
                break;
        }
    }
}

void Application::adminBookInquiry() {
    while (true) {
        // 0=sort by price, 1=total price, 2=query intro, 3=back
        int choice = showAdminBookInquiryMenu();
        switch (choice) {
            case 0:
                sortBooksUI();
                break;
            case 1:
                calculateTotalPriceUI();
                break;
            case 2:
                queryBookByIdUI();
                break;
            case 3:
                return;
            default:
                break;
        }
    }
}

void Application::addBookUI() {
    AddBookData data = showAddBookForm();
    if (!data.confirmed) return;

    if (m_bookManager->bookExists(data.id)) {
        showMessage("错误", "该编号的书籍已存在", MessageType::ERROR_MSG);
        return;
    }

    if (m_bookManager->bookNameExists(data.name)) {
        showMessage("错误", "该书籍名称已存在", MessageType::ERROR_MSG);
        return;
    }

    m_bookManager->addBook(
        Book(data.id, data.name, data.author, data.intro, data.price, data.stock));
    m_bookManager->saveToFile();
    showMessage("成功", "书籍添加成功！", MessageType::SUCCESS);
}

void Application::deleteBookUI() {
    std::string id =
        showBookIdInputForm("删除书籍", "请输入要删除的书籍编号");
    if (id.empty()) return;

    if (m_bookManager->deleteBook(id)) {
        m_bookManager->saveToFile();
        showMessage("成功", "书籍删除成功！", MessageType::SUCCESS);
    } else {
        showMessage("错误", "未找到该书籍", MessageType::ERROR_MSG);
    }
}

void Application::editBookUI() {
    std::string id =
        showBookIdInputForm("编辑书籍", "请输入要编辑的书籍编号");
    if (id.empty()) return;

    Book* book = m_bookManager->findBookById(id);
    if (!book) {
        showMessage("错误", "未找到该书籍", MessageType::ERROR_MSG);
        return;
    }

    EditBookData data = showEditBookForm(*book);
    if (!data.confirmed) return;

    book->setName(data.name);
    book->setAuthor(data.author);
    book->setIntroduction(data.intro);
    book->setPrice(data.price);
    book->setStock(data.stock);

    m_bookManager->saveToFile();
    showMessage("成功", "书籍编辑成功！", MessageType::SUCCESS);
}

void Application::editBookStockUI() {
    auto [id, newStock] = showEditStockForm();
    if (id.empty()) return;

    Book* book = m_bookManager->findBookById(id);
    if (!book) {
        showMessage("错误", "未找到该书籍", MessageType::ERROR_MSG);
        return;
    }

    book->setStock(newStock);
    m_bookManager->saveToFile();
    showMessage("成功", "库存编辑成功！", MessageType::SUCCESS);
}

void Application::queryBookByIdUI() {
    std::string id =
        showBookIdInputForm("查询书籍简介", "请输入书籍编号");
    if (id.empty()) return;

    const Book* book = m_bookManager->findBookByIdConst(id);
    if (!book) {
        showMessage("错误", "未找到该书籍", MessageType::ERROR_MSG);
        return;
    }

    showBookIntroDialog(*book);
}

void Application::showAllBooksUI() {
    showBooksTable(m_bookManager->getAllBooks(), "所有书籍");
}

void Application::sortBooksUI() {
    m_bookManager->sortByPrice();
    showBooksTable(m_bookManager->getAllBooks(), "按价格排序的书籍");
}

void Application::calculateTotalPriceUI() {
    if (m_bookManager->getBookCount() == 0) {
        showMessage("提示", "暂无书籍", MessageType::INFO);
        return;
    }
    showTotalPriceDialog(m_bookManager->getTotalPrice());
}

void Application::userLogin() {
    auto [name, password] = showLoginForm("用户登录");
    if (name.empty()) return;

    m_currentUser = m_userManager->findUserByName(name);
    if (!m_currentUser) {
        showMessage("错误", "该用户不存在", MessageType::ERROR_MSG);
        return;
    }

    if (verifyPassword(password, m_currentUser->getPassword())) {
        showMessage("成功", "登录成功！", MessageType::SUCCESS);
        userMenu();
    } else {
        showMessage("错误", "密码错误", MessageType::ERROR_MSG);
    }

    m_currentUser = nullptr;
}

void Application::userRegister() {
    RegisterData data = showRegisterForm();
    if (!data.confirmed || data.username.empty()) return;

    if (m_userManager->userExists(data.username)) {
        showMessage("错误", "该用户已存在", MessageType::ERROR_MSG);
        return;
    }

    if (data.password != data.confirmPassword) {
        showMessage("错误", "两次输入的密码不一致", MessageType::ERROR_MSG);
        return;
    }

    m_userManager->addUser(User(data.username, data.password, 0.0, false));
    m_userManager->saveToFile();
    showMessage("成功", "注册成功！", MessageType::SUCCESS);
}

void Application::userMenu() {
    m_cart.clear();
    while (true) {
        // 0=view books, 1=buy, 2=VIP, 3=charge, 4=cart, 5=back
        int choice = showUserMenu(m_currentUser->getName(),
                                  m_currentUser->getBalance(),
                                  m_currentUser->isVIP());
        switch (choice) {
            case 0:
                userViewBooks();
                break;
            case 1:
                userBuyBook();
                break;
            case 2:
                userApplyVIP();
                break;
            case 3:
                userChargeBalance();
                break;
            case 4:
                userViewCart();
                break;
            case 5:
                return;
            default:
                break;
        }
    }
}

void Application::userViewBooks() {
    showBooksTable(m_bookManager->getAllBooks(), "浏览书籍");
}

void Application::userBuyBook() {
    BuyBookData data = showBuyBookForm(m_bookManager->getAllBooks());
    if (!data.confirmed) return;

    Book* book = m_bookManager->findBookById(data.bookId);
    if (!book) {
        showMessage("错误", "该书籍不存在", MessageType::ERROR_MSG);
        return;
    }

    if (data.quantity > book->getStock()) {
        showMessage("错误", "库存不足", MessageType::ERROR_MSG);
        return;
    }

    double totalPrice = book->getPrice() * data.quantity;
    if (m_currentUser->isVIP()) {
        totalPrice *= 0.9;
    }

    if (m_currentUser->getBalance() < totalPrice) {
        showMessage("错误", "余额不足", MessageType::ERROR_MSG);
        return;
    }

    book->deductStock(data.quantity);
    m_currentUser->deductBalance(totalPrice);
    m_cart.addItem(*book, data.quantity);
    m_bookManager->saveToFile();
    m_userManager->saveToFile();

    std::string msg = "购买成功！";
    if (m_currentUser->isVIP()) msg += "VIP享受9折优惠";
    showMessage("成功", msg, MessageType::SUCCESS);
}

void Application::userApplyVIP() {
    if (m_currentUser->isVIP()) {
        showMessage("提示", "你已经是VIP用户", MessageType::INFO);
        return;
    }

    std::string password = showPasswordInputForm("申请VIP - 请验证身份");
    if (password.empty()) return;

    if (verifyPassword(password, m_currentUser->getPassword())) {
        m_currentUser->setVIP(true);
        m_userManager->saveToFile();
        showMessage("成功", "恭喜！你已成为VIP用户，可享受9折优惠！",
                    MessageType::SUCCESS);
    } else {
        showMessage("错误", "密码错误", MessageType::ERROR_MSG);
    }
}

void Application::userChargeBalance() {
    std::string password = showPasswordInputForm("充值余额 - 请验证身份");
    if (password.empty()) return;

    if (!verifyPassword(password, m_currentUser->getPassword())) {
        showMessage("错误", "密码错误", MessageType::ERROR_MSG);
        return;
    }

    double amount = showChargeBalanceForm();
    if (amount <= 0) return;

    m_currentUser->addBalance(amount);
    m_userManager->saveToFile();
    showMessage("成功", "充值成功！", MessageType::SUCCESS);
}

void Application::userViewCart() {
    showCartView(m_cart, m_currentUser->isVIP());
}

void Application::systemConfig() {
    auto [admin_name, password] = showAdminAuthForm();
    if (admin_name.empty()) return;

    m_currentAdmin = m_adminManager->findAdminByName(admin_name);
    if (!m_currentAdmin) {
        showMessage("错误", "该管理员不存在", MessageType::ERROR_MSG);
        return;
    }

    if (!verifyPassword(password, m_currentAdmin->getPassword())) {
        showMessage("错误", "密码错误", MessageType::ERROR_MSG);
        m_currentAdmin = nullptr;
        return;
    }

    while (true) {
        // 0=admin list, 1=create admin, 2=user list, 3=delete user,
        // 4=clear books, 5=clear users, 6=back
        int choice = showSystemConfigMenu();
        switch (choice) {
            case 0:
                showAdminList();
                break;
            case 1:
                adminCreateAccount();
                break;
            case 2:
                showUserList();
                break;
            case 3:
                deleteUserUI();
                break;
            case 4:
                clearBookData();
                break;
            case 5:
                clearUserData();
                break;
            case 6:
                m_currentAdmin = nullptr;
                return;
            default:
                break;
        }
    }
}

void Application::showAdminList() {
    showAdminListTable(m_adminManager->getAllAdmins());
}

void Application::showUserList() {
    showUserListTable(m_userManager->getAllUsers());
}

void Application::deleteUserUI() {
    std::string name = showDeleteUserForm();
    if (name.empty()) return;

    if (m_userManager->deleteUser(name)) {
        m_userManager->saveToFile();
        showMessage("成功", "用户删除成功！", MessageType::SUCCESS);
    } else {
        showMessage("错误", "未找到该用户", MessageType::ERROR_MSG);
    }
}

void Application::clearBookData() {
    if (showConfirmDialog("此操作将删除所有书籍信息，是否继续？")) {
        std::ofstream file("book.dat", std::ios::trunc);
        file.close();
        showMessage("成功", "书籍数据已清除", MessageType::SUCCESS);
    } else {
        showMessage("提示", "操作已取消", MessageType::INFO);
    }
}

void Application::clearUserData() {
    if (showConfirmDialog("此操作将删除所有用户信息，是否继续？")) {
        std::ofstream file("user.dat", std::ios::trunc);
        file.close();
        showMessage("成功", "用户数据已清除", MessageType::SUCCESS);
    } else {
        showMessage("提示", "操作已取消", MessageType::INFO);
    }
}

bool Application::verifyPassword(const std::string& inputPassword,
                                  const std::string& correctPassword) {
    return inputPassword == correctPassword;
}
