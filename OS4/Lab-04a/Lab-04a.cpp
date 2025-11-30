#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include <cstdio>
#include <cctype>
struct ThreadArgs {
    int iterations;
    std::string username;
};
DWORD WINAPI Lab_04x(LPVOID lpParam) {
    ThreadArgs* args = static_cast<ThreadArgs*>(lpParam);
    int iterations = args->iterations;
    std::string username = args->username;

    if (iterations <= 0) {
        std::fprintf(stderr, "Ошибка: количество итераций должно быть положительным числом\n");
        return 1;
    }
    if (username.empty()) {
        std::fprintf(stderr, "Ошибка: имя пользователя не может быть пустым\n");
        return 1;
    }
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    printf("\nПроцесс - %lu Поток - %lu\n", pid, tid);
    std::string clean_username;
    for (char c : username) {
        if (isalpha(static_cast<unsigned char>(c))) {
            clean_username += c;
        }
    }
    if (clean_username.empty()) {
        clean_username = username;
    }
    for (int i = 0; i < iterations; ++i) {
        char current_letter = clean_username[i % clean_username.length()];

        printf("%lu - %lu - %d - %c\n", pid, tid, i + 1, current_letter);
        Sleep(350);
    }
    printf("Поток %lu завершил выполнение (%d итераций)\n", tid, iterations);
    return 0;
}
int main() {
    setlocale(LC_ALL, "RUS");
    char username[256];
    DWORD size = 256;
    GetUserNameA(username, &size);
    std::string my_username = username;
    std::cout << "=== Lab-04a: Управление потоками (WinAPI) ===" << std::endl;
    std::cout << "Имя пользователя: " << my_username << std::endl;
    std::cout << "PID главного процесса: " << GetCurrentProcessId() << std::endl;
    std::cout << "TID главного потока: " << GetCurrentThreadId() << std::endl;
    std::cout << "Запуск потоков..." << std::endl;
    std::cout << "=============================================" << std::endl;
    ThreadArgs args1{ 50, my_username };
    ThreadArgs args2{ 125, my_username };
    HANDLE hThread1 = CreateThread(nullptr, 0, Lab_04x, &args1, 0, nullptr);
    HANDLE hThread2 = CreateThread(nullptr, 0, Lab_04x, &args2, 0, nullptr);
    if (!hThread1 || !hThread2) {
        std::cerr << "Ошибка: не удалось создать потоки!" << std::endl;
        return 1;
    }
    std::cout << "Созданы потоки:" << std::endl;
    std::cout << "- Поток 1: 50 итераций" << std::endl;
    std::cout << "- Поток 2: 125 итераций" << std::endl;
    std::cout << "- Главный поток: 100 итераций" << std::endl;
    std::cout << "=============================================" << std::endl;
    // 100 итераций
    ThreadArgs mainArgs{ 100, my_username };
    Lab_04x(&mainArgs);
    std::cout << "Главный поток завершил работу. Ожидание дочерних потоков..." << std::endl;
    HANDLE threads[2] = { hThread1, hThread2 };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    std::cout << "=============================================" << std::endl;
    std::cout << "Все потоки завершены. Программа завершена." << std::endl;
    std::cout << "Нажмите Enter для выхода..." << std::endl;
    std::cin.get();

    return 0;
}
