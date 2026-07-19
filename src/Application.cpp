#include "Application.h"

#include "tui/admin_ui.h"
#include "tui/common_ui.h"
#include "tui/login_ui.h"
#include "tui/system_ui.h"
#include "tui/user_ui.h"
#include "database/LegacyDataMigrator.h"


Application::Application()
    : m_database(std::make_shared<Database>()),
      m_userManager(std::make_unique<UserManager>(m_database)),
      m_adminManager(std::make_unique<AdminManager>(m_database)),
      m_bookManager(std::make_unique<BookManager>(m_database)),
      m_purchaseManager(std::make_unique<PurchaseManager>(m_database)) {
}

void Application::run() {
    if (!initializePersistence()) return;

    if (m_adminManager->getAdminCount() == 0) {
        showMessage("欢迎使用图书馆管理系统",
                    "系统中没有管理员账号，需要创建第一个管理员");
        adminCreateAccount();
    }

    showLoginMenu();
}

bool Application::initializePersistence() {
    if (!m_database->open("library.db") || !m_database->initialize()) {
        showPersistenceError("初始化数据库");
        return false;
    }

    if (!m_adminManager->load() || !m_userManager->load() ||
        !m_bookManager->load()) {
        showPersistenceError("读取数据库");
        return false;
    }

    LegacyDataMigrator migrator(*m_database, *m_adminManager, *m_userManager,
                                *m_bookManager);
    if (!migrator.migrate(".")) {
        showPersistenceError("导入旧版数据");
        return false;
    }
    return true;
}

void Application::showPersistenceError(const std::string& action) {
    showMessage("数据库错误", action + "失败：" + m_database->lastError(),
                MessageType::ERROR_MSG);
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
        if (!m_adminManager->save()) {
            m_adminManager->load();
            showPersistenceError("创建管理员");
            return;
        }
        showMessage("成功", "管理员创建成功！", MessageType::SUCCESS);

        if (!showConfirmDialog("继续创建管理员？")) break;
    }
}

void Application::adminMenu() {
    while (true) {
        // 0=all books, 1=operations, 2=inquiry, 3=password, 4=back
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
                adminChangePassword();
                break;
            case 4:
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
        // 0=sort, 1=search, 2=low stock, 3=total, 4=intro,
        // 5=purchases, 6=back
        int choice = showAdminBookInquiryMenu();
        switch (choice) {
            case 0:
                sortBooksUI();
                break;
            case 1:
                searchBooksUI();
                break;
            case 2:
                showLowStockBooksUI();
                break;
            case 3:
                calculateTotalPriceUI();
                break;
            case 4:
                queryBookByIdUI();
                break;
            case 5:
                showAllPurchaseHistoryUI();
                break;
            case 6:
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
    if (!m_bookManager->save()) {
        m_bookManager->load();
        showPersistenceError("添加书籍");
        return;
    }
    showMessage("成功", "书籍添加成功！", MessageType::SUCCESS);
}

void Application::deleteBookUI() {
    std::string id =
        showBookIdInputForm("删除书籍", "请输入要删除的书籍编号");
    if (id.empty()) return;

    if (m_bookManager->deleteBook(id)) {
        if (!m_bookManager->save()) {
            m_bookManager->load();
            showPersistenceError("删除书籍");
            return;
        }
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

    if (!m_bookManager->save()) {
        m_bookManager->load();
        showPersistenceError("编辑书籍");
        return;
    }
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
    if (!m_bookManager->save()) {
        m_bookManager->load();
        showPersistenceError("编辑库存");
        return;
    }
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

void Application::searchBooksUI() {
    const std::string keyword =
        showTextInputForm("搜索书籍", "关键词", "编号、书名或作者");
    if (keyword.empty()) return;

    showBooksTable(m_bookManager->search(keyword), "搜索结果：" + keyword);
}

void Application::showLowStockBooksUI() {
    showBooksTable(m_bookManager->getLowStockBooks(), "低库存书籍（库存 ≤ 5）");
}

void Application::showAllPurchaseHistoryUI() {
    showPurchaseHistory(m_purchaseManager->getAll(), "全部购买记录");
}

void Application::adminChangePassword() {
    PasswordChangeData data = showChangePasswordForm("修改管理员密码");
    if (!data.confirmed) return;

    if (!verifyPassword(data.currentPassword, m_currentAdmin->getPassword())) {
        showMessage("错误", "当前密码错误", MessageType::ERROR_MSG);
        return;
    }
    if (data.newPassword == data.currentPassword) {
        showMessage("提示", "新密码不能与当前密码相同", MessageType::WARNING);
        return;
    }

    const std::string oldPassword = m_currentAdmin->getPassword();
    m_currentAdmin->setPassword(data.newPassword);
    if (!m_adminManager->save()) {
        m_currentAdmin->setPassword(oldPassword);
        showPersistenceError("修改管理员密码");
        return;
    }
    showMessage("成功", "管理员密码已修改", MessageType::SUCCESS);
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
    if (!m_userManager->save()) {
        m_userManager->load();
        showPersistenceError("注册用户");
        return;
    }
    showMessage("成功", "注册成功！", MessageType::SUCCESS);
}

void Application::userMenu() {
    m_cart.clear();
    while (true) {
        // 0=view, 1=search, 2=buy, 3=VIP, 4=charge, 5=cart,
        // 6=history, 7=password, 8=back
        int choice = showUserMenu(m_currentUser->getName(),
                                  m_currentUser->getBalance(),
                                  m_currentUser->isVIP());
        switch (choice) {
            case 0:
                userViewBooks();
                break;
            case 1:
                searchBooksUI();
                break;
            case 2:
                userBuyBook();
                break;
            case 3:
                userApplyVIP();
                break;
            case 4:
                userChargeBalance();
                break;
            case 5:
                userViewCart();
                break;
            case 6:
                userViewPurchaseHistory();
                break;
            case 7:
                userChangePassword();
                break;
            case 8:
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

    const int oldStock = book->getStock();
    const double oldBalance = m_currentUser->getBalance();
    book->deductStock(data.quantity);
    m_currentUser->deductBalance(totalPrice);

    Purchase purchase;
    purchase.userName = m_currentUser->getName();
    purchase.bookId = book->getId();
    purchase.bookName = book->getName();
    purchase.unitPrice = book->getPrice();
    purchase.quantity = data.quantity;
    purchase.discountRate = m_currentUser->isVIP() ? 0.9 : 1.0;
    purchase.total = totalPrice;

    if (!m_purchaseManager->recordPurchase(purchase, *m_currentUser, *book)) {
        book->setStock(oldStock);
        m_currentUser->setBalance(oldBalance);
        showPersistenceError("购买书籍");
        return;
    }
    m_cart.addItem(*book, data.quantity);

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
        if (!m_userManager->save()) {
            m_currentUser->setVIP(false);
            showPersistenceError("申请 VIP");
            return;
        }
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

    const double oldBalance = m_currentUser->getBalance();
    m_currentUser->addBalance(amount);
    if (!m_userManager->save()) {
        m_currentUser->setBalance(oldBalance);
        showPersistenceError("账户充值");
        return;
    }
    showMessage("成功", "充值成功！", MessageType::SUCCESS);
}

void Application::userViewCart() {
    showCartView(m_cart, m_currentUser->isVIP());
}

void Application::userViewPurchaseHistory() {
    showPurchaseHistory(m_purchaseManager->getByUser(m_currentUser->getName()),
                        "我的购买记录");
}

void Application::userChangePassword() {
    PasswordChangeData data = showChangePasswordForm("修改用户密码");
    if (!data.confirmed) return;

    if (!verifyPassword(data.currentPassword, m_currentUser->getPassword())) {
        showMessage("错误", "当前密码错误", MessageType::ERROR_MSG);
        return;
    }
    if (data.newPassword == data.currentPassword) {
        showMessage("提示", "新密码不能与当前密码相同", MessageType::WARNING);
        return;
    }

    const std::string oldPassword = m_currentUser->getPassword();
    m_currentUser->setPassword(data.newPassword);
    if (!m_userManager->save()) {
        m_currentUser->setPassword(oldPassword);
        showPersistenceError("修改用户密码");
        return;
    }
    showMessage("成功", "用户密码已修改", MessageType::SUCCESS);
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
        if (!m_userManager->save()) {
            m_userManager->load();
            showPersistenceError("删除用户");
            return;
        }
        showMessage("成功", "用户删除成功！", MessageType::SUCCESS);
    } else {
        showMessage("错误", "未找到该用户", MessageType::ERROR_MSG);
    }
}

void Application::clearBookData() {
    if (showConfirmDialog("此操作将删除所有书籍信息，是否继续？")) {
        if (!m_bookManager->clearAll()) {
            showPersistenceError("清除书籍数据");
            return;
        }
        showMessage("成功", "书籍数据已清除", MessageType::SUCCESS);
    } else {
        showMessage("提示", "操作已取消", MessageType::INFO);
    }
}

void Application::clearUserData() {
    if (showConfirmDialog("此操作将删除所有用户及购买记录，是否继续？")) {
        if (!m_database->beginTransaction()) {
            showPersistenceError("清除用户数据");
            return;
        }
        const bool success = m_database->execute("DELETE FROM purchases;") &&
                             m_database->execute("DELETE FROM users;") &&
                             m_database->commitTransaction();
        if (!success) {
            m_database->rollbackTransaction();
            showPersistenceError("清除用户数据");
            return;
        }
        m_userManager->load();
        showMessage("成功", "用户数据已清除", MessageType::SUCCESS);
    } else {
        showMessage("提示", "操作已取消", MessageType::INFO);
    }
}

bool Application::verifyPassword(const std::string& inputPassword,
                                  const std::string& correctPassword) {
    return inputPassword == correctPassword;
}
