#include <iostream>
#include <windows.h>
#include <string>
#include <cstdio>
#include <cctype>
HANDLE g_thread1_handle = nullptr;
HANDLE g_thread2_handle = nullptr;
DWORD g_thread1_id = 0;
DWORD g_thread2_id = 0;
void Lab_04x(int iterations, const std::string& username, int thread_num = 0) {
    if (iterations <= 0) {
        std::fprintf(stderr, "Ошибка: количество итераций должно быть положительным числом\n");
        return;
    }
    if (username.empty()) {
        std::fprintf(stderr, "Ошибка: имя пользователя не может быть пустым\n");
        return;
    }
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    if (thread_num == 1) g_thread1_id = tid;
    if (thread_num == 2) g_thread2_id = tid;

    std::string clean_username;
    for (char c : username) {
        if (isalpha(c)) clean_username += c;
    }
    if (clean_username.empty()) clean_username = username;

    try {
        for (int i = 0; i < iterations; ++i) {
            char current_letter = clean_username[i % clean_username.length()];
            printf("%lu - %lu - %d - %c\n", pid, tid, i + 1, current_letter);
            Sleep(350);
        }
    }
    catch (const std::exception& e) {
        std::fprintf(stderr, "Исключение в потоке %lu: %s\n", tid, e.what());
    }
    catch (...) {
        std::fprintf(stderr, "Неизвестное исключение в потоке %lu\n", tid);
    }
    printf("Поток %lu завершил выполнение (%d итераций)\n", tid, iterations);
}
DWORD WINAPI ThreadFunc1(LPVOID param) {
    std::string* username = static_cast<std::string*>(param);
    Lab_04x(50, *username, 1);
    return 0;
}
DWORD WINAPI ThreadFunc2(LPVOID param) {
    std::string* username = static_cast<std::string*>(param);
    Lab_04x(125, *username, 2);
    return 0;
}
int main() {
    setlocale(LC_ALL, "RUS");
    char username[256];
    DWORD size = 256;
    GetUserNameA(username, &size);
    std::string my_username = username;
    std::cout << "=== Lab-04b: Управление потоками с приостановкой (WinAPI) ===" << std::endl;
    std::cout << "Имя пользователя: " << my_username << std::endl;
    std::cout << "PID главного процесса: " << GetCurrentProcessId() << std::endl;
    std::cout << "TID главного потока: " << GetCurrentThreadId() << std::endl;
    std::cout << "=============================================================" << std::endl;
    g_thread1_handle = CreateThread(NULL, 0, ThreadFunc1, &my_username, 0, &g_thread1_id);
    g_thread2_handle = CreateThread(NULL, 0, ThreadFunc2, &my_username, 0, &g_thread2_id);

    if (!g_thread1_handle || !g_thread2_handle) {
        std::cerr << "Ошибка: не удалось создать потоки!" << std::endl;
        return 1;
    }
    std::cout << "Созданы потоки:" << std::endl;
    std::cout << "- Поток 1: 50 итераций (TID: " << g_thread1_id << ")" << std::endl;
    std::cout << "- Поток 2: 125 итераций (TID: " << g_thread2_id << ")" << std::endl;
    std::cout << "- Главный поток: 100 итераций с управлением" << std::endl;
    std::cout << "=============================================================" << std::endl;
    DWORD main_pid = GetCurrentProcessId();
    DWORD main_tid = GetCurrentThreadId();
    for (int i = 0; i < 100; ++i) {
        char current_letter = my_username[i % my_username.length()];
        printf("%lu - %lu - %d - %c\n", main_pid, main_tid, i + 1, current_letter);

        if (i == 20) {
            std::cout << "!!! Приостанавливаю Поток 1 на 20 итерации !!!" << std::endl;
            SuspendThread(g_thread1_handle);
        }
        if (i == 40) {
            std::cout << "!!! Приостанавливаю Поток 2 на 40 итерации !!!" << std::endl;
            SuspendThread(g_thread2_handle);
        }
        if (i == 60) {
            std::cout << "!!! Возобновляю Поток 1 на 60 итерации !!!" << std::endl;
            ResumeThread(g_thread1_handle);
        }
        Sleep(350);
    }
    std::cout << "=============================================================" << std::endl;
    std::cout << "Главный поток завершил цикл. Возобновляю Поток 2..." << std::endl;
    ResumeThread(g_thread2_handle);
    HANDLE threads[2] = { g_thread1_handle, g_thread2_handle };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    CloseHandle(g_thread1_handle);
    CloseHandle(g_thread2_handle);
    std::cout << "=============================================================" << std::endl;
    std::cout << "Все потоки завершены. Программа завершена." << std::endl;
    std::cout << "Нажмите Enter для выхода..." << std::endl;
    std::cin.get();

    return 0;
}
