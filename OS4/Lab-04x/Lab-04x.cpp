#include <iostream>
#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>

void Lab_04x(int iterations, const std::string& username) {
    if (iterations <= 0) {
        std::cerr << "Ошибка: количество итераций должно быть положительным числом" << std::endl;
        return;
    }
    if (username.empty()) {
        std::cerr << "Ошибка: имя пользователя не может быть пустым" << std::endl;
        return;
    }

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    std::string clean_username;
    for (char c : username) {
        if (isalpha(static_cast<unsigned char>(c))) {
            clean_username += c;
        }
    }
    if (clean_username.empty()) {
        clean_username = username;
    }

    try {
        for (int i = 0; i < iterations; ++i) {
            char current_letter = clean_username[i % clean_username.length()];
            printf("PID - %lu  TID - %lu - %d - %c\n",
                pid, tid, i + 1, current_letter);
            fflush(stdout);
            Sleep(350);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Исключение в потоке: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Неизвестное исключение в потоке" << std::endl;
    }

    printf("Поток %lu завершил выполнение (%d итераций)\n", tid, iterations);
}

DWORD WINAPI ThreadFunc(LPVOID param) {
    std::string* username = static_cast<std::string*>(param);
    Lab_04x(7, *username);
    return 0;
}
int main() {
    setlocale(LC_ALL, "RUS");
    char username[256];
    DWORD size = 256;
    GetUserNameA(username, &size);
    std::cout << username << std::endl;
    std::string my_username = username;
    Lab_04x(21, my_username);
    HANDLE hThread = CreateThread(nullptr,
        0, ThreadFunc, &my_username, 0, nullptr);
    if (hThread == nullptr) {
        std::cerr << "Не удалось создать поток!" << std::endl;
        return 1;
    }
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}
