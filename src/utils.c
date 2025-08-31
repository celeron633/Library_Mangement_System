#include "utils.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

void clear_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hConsole == INVALID_HANDLE_VALUE) return;

    // 获取缓冲区大小
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // 用空格填满整个缓冲区
    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', cellCount, homeCoords, &count)) return;

    // 重置每个位置的属性
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return;

    // 光标移回左上角
    SetConsoleCursorPosition(hConsole, homeCoords);
}

#else  // Linux / macOS / Unix 都走 ANSI 转义序列

void clear_screen() {
    // 光标移到左上角并清屏
    printf("\033[H\033[J");
    fflush(stdout);
}

#endif

void pause_and_wait(void)
{
    fprintf(stdout, "请任意按键继续...\r\n");
    getchar();
}

#ifdef _WIN32
#include <conio.h>   // Windows 平台直接用 conio.h 里的 getch
#else
#include <termios.h>
#include <unistd.h>

int getch(void) {
    struct termios oldt, newt;
    int ch;
    // 保存原来的终端属性
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // 关闭缓冲（ICANON）和回显（ECHO）
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();  // 立即获取一个字符（无需回车、不回显）

    // 恢复原来的属性
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif