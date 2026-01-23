#include <iostream>
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
    std::cout.flush();
#endif
}