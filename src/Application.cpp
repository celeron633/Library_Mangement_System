#include "Application.h"
#include "utils/UIManager.h"
#include "ui/MenuUI.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>

Application::Application() 
    : m_userManager(std::make_unique<UserManager>()),
      m_adminManager(std::make_unique<AdminManager>()),
      m_bookManager(std::make_unique<BookManager>()) {
}

void Application::run() {
    // 初始化数据
    m_adminManager->loadFromFile();
    m_userManager->loadFromFile();
    m_bookManager->loadFromFile();

    // 检查管理员是否存在
    if (m_adminManager->getAdminCount() == 0) {
        UIManager::printTitle("欢迎使用图书馆管理系统");
        UIManager::printInfo("系统中没有管理员账号，需要创建第一个管理员");
        UIManager::pause();
        adminCreateAccount();
        UIManager::printSuccess("管理员账号创建成功！");
        UIManager::pause();
    }

    showLoginMenu();
}

void Application::showLoginMenu() {
    while (true) {
        UIManager::printTitle("图书馆管理系统 - 登录");
        UIManager::printMenuItem(1, "管理员登录");
        UIManager::printMenuItem(2, "用户登录");
        UIManager::printMenuItem(3, "用户注册");
        UIManager::printMenuItem(4, "系统设置");
        UIManager::printMenuItem(5, "退出系统");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                userLogin();
                break;
            case 3:
                userRegister();
                break;
            case 4:
                systemConfig();
                break;
            case 5:
                UIManager::printCenter("谢谢你的使用，再见！");
                exit(0);
            default:
                UIManager::printError("输入错误，请重新选择");
                UIManager::pause();
        }
    }
}

void Application::adminLogin() {
    UIManager::printTitle("管理员登录");

    std::string name;
    UIManager::printInfo("请输入管理员名字:");
    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_currentAdmin = m_adminManager->findAdminByName(name);
    if (!m_currentAdmin) {
        UIManager::printError("该管理员不存在");
        UIManager::pause();
        return;
    }

    std::string password = getPasswordInput("请输入密码");
    if (verifyPassword(password, m_currentAdmin->getPassword())) {
        UIManager::printSuccess("登录成功！");
        UIManager::pause();
        adminMenu();
    } else {
        UIManager::printError("密码错误");
        UIManager::pause();
    }

    m_currentAdmin = nullptr;
}

void Application::adminCreateAccount() {
    UIManager::printTitle("创建管理员账号");

    while (true) {
        std::string name;
        UIManager::printInfo("请输入管理员名字：");
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (m_adminManager->adminExists(name)) {
            UIManager::printError("该管理员已存在");
            continue;
        }

        std::string password, confirmPassword;
        password = UIManager::getHiddenInput("请输入密码");
        confirmPassword = UIManager::getHiddenInput("请确认密码");

        if (password != confirmPassword) {
            UIManager::printError("两次输入的密码不一致");
            continue;
        }

        m_adminManager->addAdmin(Admin(name, password));
        m_adminManager->saveToFile();
        UIManager::printSuccess("管理员创建成功！");

        std::string choice;
        UIManager::printInfo("继续创建管理员？(y/n):");
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == "n" || choice == "no") {
            break;
        }
    }
}

void Application::adminMenu() {
    while (true) {
        UIManager::printTitle("管理员菜单");
        UIManager::printMenuItem(1, "显示所有书籍");
        UIManager::printMenuItem(2, "书籍操作");
        UIManager::printMenuItem(3, "书籍查询");
        UIManager::printMenuItem(4, "返回");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                showAllBooksUI();
                break;
            case 2:
                adminBookOperations();
                break;
            case 3:
                adminBookInquiry();
                break;
            case 4:
                return;
            default:
                UIManager::printError("输入错误");
                UIManager::pause();
        }
    }
}

void Application::adminBookOperations() {
    while (true) {
        UIManager::printTitle("书籍操作菜单");
        UIManager::printMenuItem(1, "添加书籍");
        UIManager::printMenuItem(2, "删除书籍");
        UIManager::printMenuItem(3, "编辑书籍信息");
        UIManager::printMenuItem(4, "编辑库存");
        UIManager::printMenuItem(5, "返回");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                addBookUI();
                break;
            case 2:
                deleteBookUI();
                break;
            case 3:
                editBookUI();
                break;
            case 4:
                editBookStockUI();
                break;
            case 5:
                return;
            default:
                UIManager::printError("输入错误");
                UIManager::pause();
        }
    }
}

void Application::adminBookInquiry() {
    while (true) {
        UIManager::printTitle("书籍查询菜单");
        UIManager::printMenuItem(1, "按价格排序显示");
        UIManager::printMenuItem(2, "计算所有图书总价");
        UIManager::printMenuItem(3, "查询书籍简介");
        UIManager::printMenuItem(4, "返回");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                sortBooksUI();
                break;
            case 2:
                calculateTotalPriceUI();
                break;
            case 3:
                queryBookByIdUI();
                break;
            case 4:
                return;
            default:
                UIManager::printError("输入错误");
                UIManager::pause();
        }
    }
}

void Application::addBookUI() {
    UIManager::printTitle("添加书籍");

    std::string id, name, author, intro;
    double price;
    int stock;

    std::cout << "    请输入书籍编号: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m_bookManager->bookExists(id)) {
        UIManager::printError("该编号的书籍已存在");
        UIManager::pause();
        return;
    }

    std::cout << "    请输入书籍名称: ";
    std::getline(std::cin, name);

    if (m_bookManager->bookNameExists(name)) {
        UIManager::printError("该书籍已存在");
        UIManager::pause();
        return;
    }

    std::cout << "    请输入作者: ";
    std::getline(std::cin, author);

    std::cout << "    请输入简介: ";
    std::getline(std::cin, intro);

    std::cout << "    请输入价格: ";
    std::cin >> price;

    std::cout << "    请输入库存: ";
    std::cin >> stock;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_bookManager->addBook(Book(id, name, author, intro, price, stock));
    m_bookManager->saveToFile();
    UIManager::printSuccess("书籍添加成功！");
    UIManager::pause();
}

void Application::deleteBookUI() {
    UIManager::printTitle("删除书籍");

    std::string id;
    std::cout << "    请输入要删除的书籍编号: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m_bookManager->deleteBook(id)) {
        m_bookManager->saveToFile();
        UIManager::printSuccess("书籍删除成功！");
    } else {
        UIManager::printError("未找到该书籍");
    }
    UIManager::pause();
}

void Application::editBookUI() {
    UIManager::printTitle("编辑书籍信息");

    std::string id;
    std::cout << "    请输入要编辑的书籍编号: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Book* book = m_bookManager->findBookById(id);
    if (!book) {
        UIManager::printError("未找到该书籍");
        UIManager::pause();
        return;
    }

    std::string name, author, intro;
    double price;
    int stock;

    std::cout << "    请输入新的书籍名称: ";
    std::getline(std::cin, name);

    std::cout << "    请输入新的作者: ";
    std::getline(std::cin, author);

    std::cout << "    请输入新的简介: ";
    std::getline(std::cin, intro);

    std::cout << "    请输入新的价格: ";
    std::cin >> price;

    std::cout << "    请输入新的库存: ";
    std::cin >> stock;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    book->setName(name);
    book->setAuthor(author);
    book->setIntroduction(intro);
    book->setPrice(price);
    book->setStock(stock);

    m_bookManager->saveToFile();
    UIManager::printSuccess("书籍编辑成功！");
    UIManager::pause();
}

void Application::editBookStockUI() {
    UIManager::printTitle("编辑库存");

    std::string id;
    std::cout << "    请输入要编辑的书籍编号: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Book* book = m_bookManager->findBookById(id);
    if (!book) {
        UIManager::printError("未找到该书籍");
        UIManager::pause();
        return;
    }

    int newStock;
    std::cout << "    请输入新的库存数量: ";
    std::cin >> newStock;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    book->setStock(newStock);
    m_bookManager->saveToFile();
    UIManager::printSuccess("库存编辑成功！");
    UIManager::pause();
}

void Application::queryBookByIdUI() {
    UIManager::printTitle("查询书籍简介");

    std::string id;
    std::cout << "    请输入书籍编号: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    const Book* book = m_bookManager->findBookByIdConst(id);
    if (!book) {
        UIManager::printError("未找到该书籍");
        UIManager::pause();
        return;
    }

    UIManager::printSeparator();
    std::cout << "    书籍简介: " << book->getIntroduction() << "\n";
    UIManager::printSeparator();
    UIManager::pause();
}

void Application::showAllBooksUI() {
    UIManager::printTitle("所有书籍");

    if (m_bookManager->getBookCount() == 0) {
        UIManager::printInfo("暂无书籍");
        UIManager::pause();
        return;
    }

    MenuUI::printBookTableHeader();
    for (const auto& book : m_bookManager->getAllBooks()) {
        MenuUI::printBookRow(book.getId(), book.getName(), book.getAuthor(),
                           book.getIntroduction(), book.getPrice(), book.getStock());
    }
    std::cout << "\n";
    UIManager::pause();
}

void Application::sortBooksUI() {
    UIManager::printTitle("按价格排序的书籍");

    if (m_bookManager->getBookCount() == 0) {
        UIManager::printInfo("暂无书籍");
        UIManager::pause();
        return;
    }

    m_bookManager->sortByPrice();
    MenuUI::printBookTableHeader();
    for (const auto& book : m_bookManager->getAllBooks()) {
        MenuUI::printBookRow(book.getId(), book.getName(), book.getAuthor(),
                           book.getIntroduction(), book.getPrice(), book.getStock());
    }
    std::cout << "\n";
    UIManager::pause();
}

void Application::calculateTotalPriceUI() {
    UIManager::printTitle("书籍总价计算");

    if (m_bookManager->getBookCount() == 0) {
        UIManager::printInfo("暂无书籍");
        UIManager::pause();
        return;
    }

    double totalPrice = m_bookManager->getTotalPrice();
    UIManager::printInfo("所有书籍总价: ¥" + std::to_string(totalPrice));
    UIManager::pause();
}

void Application::userLogin() {
    UIManager::printTitle("用户登录");

    std::string name;
    UIManager::printInfo("请输入用户名:");
    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_currentUser = m_userManager->findUserByName(name);
    if (!m_currentUser) {
        UIManager::printError("该用户不存在");
        UIManager::pause();
        return;
    }

    std::string password = getPasswordInput("请输入密码");
    if (verifyPassword(password, m_currentUser->getPassword())) {
        UIManager::printSuccess("登录成功！");
        UIManager::pause();
        userMenu();
    } else {
        UIManager::printError("密码错误");
        UIManager::pause();
    }

    m_currentUser = nullptr;
}

void Application::userRegister() {
    UIManager::printTitle("用户注册");

    std::string name;
    UIManager::printInfo("请输入用户名：");
    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m_userManager->userExists(name)) {
        UIManager::printError("该用户已存在");
        UIManager::pause();
        return;
    }

    std::string password, confirmPassword;
    password = UIManager::getHiddenInput("请输入密码");
    confirmPassword = UIManager::getHiddenInput("请确认密码");

    if (password != confirmPassword) {
        UIManager::printError("两次输入的密码不一致");
        UIManager::pause();
        return;
    }

    m_userManager->addUser(User(name, password, 0.0, false));
    m_userManager->saveToFile();
    UIManager::printSuccess("注册成功！");
    UIManager::pause();
}

void Application::userMenu() {
    while (true) {
        UIManager::printTitle("用户菜单");
        MenuUI::printUserCard(m_currentUser->getName(), m_currentUser->getBalance(), 
                            m_currentUser->isVIP());

        UIManager::printMenuItem(1, "浏览书籍");
        UIManager::printMenuItem(2, "购买书籍");
        UIManager::printMenuItem(3, "申请VIP");
        UIManager::printMenuItem(4, "充值余额");
        UIManager::printMenuItem(5, "查看购物车");
        UIManager::printMenuItem(6, "返回");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                userViewBooks();
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
                return;
            default:
                UIManager::printError("输入错误");
                UIManager::pause();
        }
    }
}

void Application::userViewBooks() {
    UIManager::printTitle("浏览书籍");

    if (m_bookManager->getBookCount() == 0) {
        UIManager::printInfo("暂无书籍");
        UIManager::pause();
        return;
    }

    MenuUI::printBookTableHeader();
    for (const auto& book : m_bookManager->getAllBooks()) {
        MenuUI::printBookRow(book.getId(), book.getName(), book.getAuthor(),
                           book.getIntroduction(), book.getPrice(), book.getStock());
    }
    std::cout << "\n";
    UIManager::pause();
}

void Application::userBuyBook() {
    UIManager::printTitle("购买书籍");

    std::string bookId;
    std::cout << "    请输入要购买的书籍编号: ";
    std::cin >> bookId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Book* book = m_bookManager->findBookById(bookId);
    if (!book) {
        UIManager::printError("该书籍不存在");
        UIManager::pause();
        return;
    }

    int quantity;
    std::cout << "    请输入购买数量: ";
    std::cin >> quantity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (quantity > book->getStock()) {
        UIManager::printError("库存不足");
        UIManager::pause();
        return;
    }

    double totalPrice = book->getPrice() * quantity;
    if (m_currentUser->isVIP()) {
        totalPrice *= 0.9;  // VIP 9折
    }

    if (m_currentUser->getBalance() < totalPrice) {
        UIManager::printError("余额不足");
        UIManager::pause();
        return;
    }

    book->deductStock(quantity);
    m_currentUser->deductBalance(totalPrice);
    m_cart.addItem(*book, quantity);
    m_bookManager->saveToFile();
    m_userManager->saveToFile();

    if (m_currentUser->isVIP()) {
        UIManager::printSuccess("购买成功！VIP享受9折优惠");
    } else {
        UIManager::printSuccess("购买成功！");
    }
    UIManager::pause();
}

void Application::userApplyVIP() {
    UIManager::printTitle("申请VIP");

    if (m_currentUser->isVIP()) {
        UIManager::printInfo("你已经是VIP用户");
        UIManager::pause();
        return;
    }

    UIManager::printInfo("需要确认身份，请输入密码");
    std::string password = getPasswordInput("请输入密码");

    if (verifyPassword(password, m_currentUser->getPassword())) {
        m_currentUser->setVIP(true);
        m_userManager->saveToFile();
        UIManager::printSuccess("恭喜！你已成为VIP用户，可享受9折优惠！");
    } else {
        UIManager::printError("密码错误");
    }
    UIManager::pause();
}

void Application::userChargeBalance() {
    UIManager::printTitle("充值余额");

    UIManager::printInfo("需要确认身份，请输入密码");
    std::string password = getPasswordInput("请输入密码");

    if (!verifyPassword(password, m_currentUser->getPassword())) {
        UIManager::printError("密码错误");
        UIManager::pause();
        return;
    }

    double amount;
    std::cout << "    请输入充值金额: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (amount <= 0) {
        UIManager::printError("金额必须大于0");
        UIManager::pause();
        return;
    }

    m_currentUser->addBalance(amount);
    m_userManager->saveToFile();
    UIManager::printSuccess("充值成功！");
    UIManager::pause();
}

void Application::userViewCart() {
    UIManager::printTitle("购物车");

    if (m_cart.isEmpty()) {
        UIManager::printInfo("购物车为空");
        UIManager::pause();
        return;
    }

    MenuUI::printCartTableHeader();
    for (const auto& item : m_cart.getItems()) {
        MenuUI::printCartRow(item.book.getId(), item.book.getName(),
                           item.book.getPrice(), item.quantity, item.getSubtotal());
    }

    std::cout << std::string(55, '-') << "\n";
    double total = m_currentUser->isVIP() ? m_cart.getTotalPriceWithDiscount(0.9) : m_cart.getTotalPrice();
    std::cout << std::setw(45) << std::right << "总计: ¥"
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << total << "\n";
    std::cout << "\n";
    UIManager::pause();
}

void Application::systemConfig() {
    UIManager::printTitle("系统设置");
    UIManager::printInfo("需要管理员权限，请输入管理员名字");
    
    std::string name;
    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_currentAdmin = m_adminManager->findAdminByName(name);
    if (!m_currentAdmin) {
        UIManager::printError("该管理员不存在");
        UIManager::pause();
        return;
    }

    std::string password = getPasswordInput("请输入密码");
    if (!verifyPassword(password, m_currentAdmin->getPassword())) {
        UIManager::printError("密码错误");
        UIManager::pause();
        m_currentAdmin = nullptr;
        return;
    }

    while (true) {
        UIManager::printTitle("系统设置菜单");
        UIManager::printMenuItem(1, "显示管理员列表");
        UIManager::printMenuItem(2, "创建新管理员");
        UIManager::printMenuItem(3, "显示用户列表");
        UIManager::printMenuItem(4, "删除用户");
        UIManager::printMenuItem(5, "清除所有书籍数据");
        UIManager::printMenuItem(6, "清除所有用户数据");
        UIManager::printMenuItem(7, "返回");
        UIManager::printMenuEnd();

        int choice = UIManager::getMenuChoice();
        switch (choice) {
            case 1:
                showAdminList();
                break;
            case 2:
                adminCreateAccount();
                break;
            case 3:
                showUserList();
                break;
            case 4:
                deleteUserUI();
                break;
            case 5:
                clearBookData();
                break;
            case 6:
                clearUserData();
                break;
            case 7:
                m_currentAdmin = nullptr;
                return;
            default:
                UIManager::printError("输入错误");
                UIManager::pause();
        }
    }
}

void Application::showAdminList() {
    UIManager::printTitle("管理员列表");

    if (m_adminManager->getAdminCount() == 0) {
        UIManager::printInfo("暂无管理员");
        UIManager::pause();
        return;
    }

    std::cout << "\n";
    std::cout << std::setw(20) << std::left << "管理员名称"
              << std::setw(25) << std::left << "密码"
              << "\n";
    std::cout << std::string(45, '-') << "\n";

    for (const auto& admin : m_adminManager->getAllAdmins()) {
        std::cout << std::setw(20) << std::left << admin.getName()
                  << std::setw(25) << std::left << admin.getPassword()
                  << "\n";
    }
    std::cout << "\n";
    UIManager::pause();
}

void Application::showUserList() {
    UIManager::printTitle("用户列表");

    if (m_userManager->getUserCount() == 0) {
        UIManager::printInfo("暂无用户");
        UIManager::pause();
        return;
    }

    MenuUI::printUserTableHeader();
    for (const auto& user : m_userManager->getAllUsers()) {
        MenuUI::printUserRow(user.getName(), user.getBalance(), user.isVIP());
    }
    std::cout << "\n";
    UIManager::pause();
}

void Application::deleteUserUI() {
    UIManager::printTitle("删除用户");

    std::string name;
    std::cout << "    请输入要删除的用户名: ";
    std::cin >> name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m_userManager->deleteUser(name)) {
        m_userManager->saveToFile();
        UIManager::printSuccess("用户删除成功！");
    } else {
        UIManager::printError("未找到该用户");
    }
    UIManager::pause();
}

void Application::clearBookData() {
    UIManager::printTitle("清除书籍数据");
    UIManager::printWarning("此操作将删除所有书籍信息，是否继续？(y/n)");

    std::string choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == "y" || choice == "yes") {
        std::ofstream file("book.dat", std::ios::trunc);
        file.close();
        UIManager::printSuccess("书籍数据已清除");
    } else {
        UIManager::printInfo("操作已取消");
    }
    UIManager::pause();
}

void Application::clearUserData() {
    UIManager::printTitle("清除用户数据");
    UIManager::printWarning("此操作将删除所有用户信息，是否继续？(y/n)");

    std::string choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == "y" || choice == "yes") {
        std::ofstream file("user.dat", std::ios::trunc);
        file.close();
        UIManager::printSuccess("用户数据已清除");
    } else {
        UIManager::printInfo("操作已取消");
    }
    UIManager::pause();
}

bool Application::verifyPassword(const std::string& inputPassword, const std::string& correctPassword) {
    return inputPassword == correctPassword;
}

std::string Application::getPasswordInput(const std::string& prompt) {
    return UIManager::getHiddenInput(prompt);
}

int Application::getIntInput(int min, int max) {
    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            UIManager::printError("输入无效，请重试");
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}
