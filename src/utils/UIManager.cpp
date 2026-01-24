#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/types.h>
#endif

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

std::string UIManager::getHiddenInput(const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << "    " << prompt << ": ";
    }
    std::cout.flush();

    std::string password;

#ifdef _WIN32
    // Windows 实现
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    
    // 保存原始模式
    DWORD originalMode = mode;
    
    // 禁用回显输入
    mode &= ~ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode);

    char ch;
    while (std::cin.get(ch)) {
        if (ch == '\n' || ch == '\r') {
            std::cout << "\n";
            break;
        }
        if (ch == '\b') {  // 退格键
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else if (ch >= 32 && ch < 127) {  // 可打印字符
            password += ch;
            std::cout << "*";
            std::cout.flush();
        }
    }

    // 恢复原始模式
    SetConsoleMode(hStdin, originalMode);

#else
    // Linux/Unix/macOS 实现
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // 禁用回显
    newt.c_lflag &= ~(ECHO | ICANON);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == '\n' || ch == '\r') {
            std::cout << "\n";
            break;
        }
        if (ch == '\b' || ch == 127) {  // 退格或删除键
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else if (ch >= 32 && ch < 127) {  // 可打印字符
            password += ch;
            std::cout << "*";
            std::cout.flush();
        }
    }

    // 恢复原始设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

#endif

    return password;
}
