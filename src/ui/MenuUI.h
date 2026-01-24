#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <vector>

class MenuUI {
public:
    // 打印书籍列表表头
    static void printBookTableHeader();

    // 打印单个书籍的信息
    static void printBookRow(const std::string& id, const std::string& name,
                            const std::string& author, const std::string& intro,
                            double price, int stock);

    // 打印用户列表表头
    static void printUserTableHeader();

    // 打印单个用户的信息
    static void printUserRow(const std::string& name, double balance, bool isVIP);

    // 打印购物车表头
    static void printCartTableHeader();

    // 打印购物车项
    static void printCartRow(const std::string& id, const std::string& name,
                            double price, int quantity, double subtotal);

    // 打印用户信息卡片
    static void printUserCard(const std::string& name, double balance, bool isVIP);
};
