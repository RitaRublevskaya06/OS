#include <iostream>
#include <windows.h>
#include <cstdio>
#include <cctype>
#include <string>
using namespace std;
HANDLE g_thread1_handle = nullptr;
HANDLE g_thread2_handle = nullptr;
DWORD g_thread1_id = 0;
DWORD g_thread2_id = 0;
bool g_thread2_terminated = false;
struct ThreadParams {
    int iterations;
    std::string* username;
    int thread_num;
};
DWORD WINAPI ThreadProc(LPVOID lpParam) {
    ThreadParams* params = reinterpret_cast<ThreadParams*>(lpParam);
    int iterations = params->iterations;
    std::string username = *(params->username);
    int thread_num = params->thread_num;
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
            printf("PID: %lu - TID: %lu - %d - %c\n", pid, tid, i + 1, current_letter);
            Sleep(350);
        }
    }
    catch (...) {
        fprintf(stderr, "Исключение в потоке %lu\n", tid);
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
    std::cout << "=== Lab-04c: Управление потоками с завершением ===" << std::endl;
    std::cout << "Имя пользователя: " << my_username << std::endl;
    std::cout << "PID главного процесса: " << GetCurrentProcessId() << std::endl;
    std::cout << "TID главного потока: " << GetCurrentThreadId() << std::endl;
    std::cout << "==================================================" << std::endl;
    ThreadParams params1 = { 50, &my_username, 1 };
    ThreadParams params2 = { 125, &my_username, 2 };
    g_thread1_handle = CreateThread(nullptr, 0, ThreadProc, &params1, 0, &g_thread1_id);
    g_thread2_handle = CreateThread(nullptr, 0, ThreadProc, &params2, 0, &g_thread2_id);
    Sleep(100);
    std::cout << "Созданы потоки:" << std::endl;
    std::cout << "- Поток 1: 50 итераций (TID: " << g_thread1_id << ")" << std::endl;
    std::cout << "- Поток 2: 125 итераций (TID: " << g_thread2_id << ")" << std::endl;
    std::cout << "- Главный поток: 100 итераций с управлением" << std::endl;
    std::string clean_username;
    for (char c : my_username) {
        if (isalpha(static_cast<unsigned char>(c))) clean_username += c;
    }
    if (clean_username.empty()) clean_username = my_username;
    for (int i = 0; i < 100; ++i) {
        char current_letter = clean_username[i % clean_username.length()];
        printf("PID: %lu - TID: %lu - %d - %c\n", GetCurrentProcessId(), GetCurrentThreadId(), i + 1, current_letter);
        if (i == 40 && !g_thread2_terminated && g_thread2_handle) {
            std::cout << "!!! Завершение потока 2 на 40 итерации" << std::endl;
            if (TerminateThread(g_thread2_handle, 0)) {
                std::cout << "Поток 2 успешно завершен!" << std::endl;
                g_thread2_terminated = true;
                CloseHandle(g_thread2_handle);
                g_thread2_handle = nullptr;
            }
            else {
                std::cout << "Ошибка завершения потока! Код: " << GetLastError() << std::endl;
            }
        }
        Sleep(350);
    }
    std::cout << "Главный поток завершил работу. Ожидание дочерних потоков..." << std::endl;
    if (g_thread1_handle) {
        WaitForSingleObject(g_thread1_handle, INFINITE);
        CloseHandle(g_thread1_handle);
        std::cout << "Поток 1 завершился" << std::endl;
    }
    if (!g_thread2_terminated && g_thread2_handle) {
        WaitForSingleObject(g_thread2_handle, INFINITE);
        CloseHandle(g_thread2_handle);
        std::cout << "Поток 2 завершился" << std::endl;
    }
    std::cout << "Все потоки завершены. Программа завершена." << std::endl;
    std::cout << "Нажмите Enter для выхода..." << std::endl;
    std::cin.get();
    return 0;
}
