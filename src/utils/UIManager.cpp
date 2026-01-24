#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void UIManager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}

void UIManager::printTitle(const std::string& title) {
    clearScreen();
    int width = 60;
    int titleLen = title.length();
    int padding = (width - titleLen - 4) / 2;

    std::cout << "\n";
    std::cout << std::string(width, '=') << "\n";
    std::cout << "||" << std::string(padding, ' ') << title << std::string(padding, ' ') << "||\n";
    std::cout << std::string(width, '=') << "\n\n";
}

void UIManager::printMenuItem(int index, const std::string& text) {
    std::cout << "    " << index << ". " << text << "\n";
}

void UIManager::printMenuEnd() {
    std::cout << "\n" << std::string(60, '=') << "\n\n";
}

void UIManager::printSeparator() {
    std::cout << std::string(60, '-') << "\n";
}

void UIManager::printCenter(const std::string& text) {
    int width = 60;
    int textLen = text.length();
    int padding = (width - textLen) / 2;
    std::cout << std::string(padding, ' ') << text << "\n";
}

int UIManager::getMenuChoice() {
    int choice;
    std::cout << "    请输入你的选择: ";
    std::cin >> choice;
    std::cin.ignore(10000, '\n');
    return choice;
}

void UIManager::printInfo(const std::string& message) {
    std::cout << "ℹ  " << message << "\n";
}

void UIManager::printSuccess(const std::string& message) {
    std::cout << "✓ " << message << "\n";
}

void UIManager::printError(const std::string& message) {
    std::cout << "✗ " << message << "\n";
}

void UIManager::printWarning(const std::string& message) {
    std::cout << "⚠ " << message << "\n";
}

void UIManager::pause() {
    std::cout << "\n    按 Enter 继续...";
    std::cin.ignore(10000, '\n');
}

std::string UIManager::formatWidth(const std::string& str, int width, bool rightAlign) {
    std::ostringstream oss;
    if (rightAlign) {
        oss << std::right << std::setw(width) << str;
    } else {
        oss << std::left << std::setw(width) << str;
    }
    return oss.str();
}
