#pragma once

#include <string>
#include <iostream>
#include <vector>

class UIManager {
public:
    // 清屏
    static void clearScreen();

    // 打印带边框的标题
    static void printTitle(const std::string& title);

    // 打印菜单项
    static void printMenuItem(int index, const std::string& text);

    // 打印菜单结尾
    static void printMenuEnd();

    // 打印分隔线
    static void printSeparator();

    // 打印居中文本
    static void printCenter(const std::string& text);

    // 获取用户输入（带验证）
    static int getMenuChoice();

    // 打印提示信息
    static void printInfo(const std::string& message);
    static void printSuccess(const std::string& message);
    static void printError(const std::string& message);
    static void printWarning(const std::string& message);

    // 暂停
    static void pause();

    // 格式化数字为固定宽度
    static std::string formatWidth(const std::string& str, int width, bool rightAlign = false);
};
