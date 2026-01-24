#include "MenuUI.h"

void MenuUI::printBookTableHeader() {
    std::cout << "\n";
    std::cout << std::setw(10) << std::left << "编号"
              << std::setw(15) << std::left << "书名"
              << std::setw(15) << std::left << "作者"
              << std::setw(15) << std::left << "简介"
              << std::setw(10) << std::right << "价格"
              << std::setw(10) << std::right << "库存"
              << "\n";
    std::cout << std::string(75, '-') << "\n";
}

void MenuUI::printBookRow(const std::string& id, const std::string& name,
                         const std::string& author, const std::string& intro,
                         double price, int stock) {
    std::cout << std::setw(10) << std::left << (id.length() > 10 ? id.substr(0, 10) : id)
              << std::setw(15) << std::left << (name.length() > 15 ? name.substr(0, 15) : name)
              << std::setw(15) << std::left << (author.length() > 15 ? author.substr(0, 15) : author)
              << std::setw(15) << std::left << (intro.length() > 15 ? intro.substr(0, 15) : intro)
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << price
              << std::setw(10) << std::right << stock
              << "\n";
}

void MenuUI::printUserTableHeader() {
    std::cout << "\n";
    std::cout << std::setw(20) << std::left << "用户名"
              << std::setw(15) << std::right << "余额"
              << std::setw(10) << std::right << "VIP"
              << "\n";
    std::cout << std::string(45, '-') << "\n";
}

void MenuUI::printUserRow(const std::string& name, double balance, bool isVIP) {
    std::cout << std::setw(20) << std::left << name
              << std::setw(15) << std::right << std::fixed << std::setprecision(2) << balance
              << std::setw(10) << std::right << (isVIP ? "是" : "否")
              << "\n";
}

void MenuUI::printCartTableHeader() {
    std::cout << "\n";
    std::cout << std::setw(10) << std::left << "编号"
              << std::setw(15) << std::left << "书名"
              << std::setw(10) << std::right << "价格"
              << std::setw(8) << std::right << "数量"
              << std::setw(12) << std::right << "小计"
              << "\n";
    std::cout << std::string(55, '-') << "\n";
}

void MenuUI::printCartRow(const std::string& id, const std::string& name,
                         double price, int quantity, double subtotal) {
    std::cout << std::setw(10) << std::left << id
              << std::setw(15) << std::left << (name.length() > 15 ? name.substr(0, 15) : name)
              << std::setw(10) << std::right << std::fixed << std::setprecision(2) << price
              << std::setw(8) << std::right << quantity
              << std::setw(12) << std::right << std::fixed << std::setprecision(2) << subtotal
              << "\n";
}

void MenuUI::printUserCard(const std::string& name, double balance, bool isVIP) {
    int width = 50;
    std::cout << "\n" << std::string(width, '=') << "\n";
    std::cout << "  用户: " << name << "\n";
    std::cout << "  余额: ¥" << std::fixed << std::setprecision(2) << balance << "\n";
    std::cout << "  等级: " << (isVIP ? "VIP会员" : "普通用户") << "\n";
    std::cout << std::string(width, '=') << "\n\n";
}
