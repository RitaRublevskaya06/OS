#include <windows.h>
#include <stdio.h>
#include <ctype.h>

typedef struct {
    int iterations;
    wchar_t* username;
} ThreadArgs;

DWORD WINAPI Lab_04x(LPVOID lpParam) {
    ThreadArgs* args = (ThreadArgs*)lpParam;
    int iterations = args->iterations;
    wchar_t* username = args->username;

    if (iterations <= 0) {
        printf("Ошибка: количество итераций должно быть положительным числом\n");
        return 1;
    }
    if (username == NULL || wcslen(username) == 0) {
        printf("Ошибка: имя пользователя не может быть пустым\n");
        return 1;
    }

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();
    printf("\nПроцесс - %lu Поток - %lu\n", pid, tid);

    wchar_t clean_username[256] = L"";
    int clean_index = 0;

    for (int i = 0; username[i] != L'\0'; ++i) {
        if (iswalpha(username[i])) {
            clean_username[clean_index++] = username[i];
        }
    }
    clean_username[clean_index] = L'\0';

    if (wcslen(clean_username) == 0) {
        wcscpy_s(clean_username, 256, username);
    }

    for (int i = 0; i < iterations; ++i) {
        wchar_t current_letter = clean_username[i % wcslen(clean_username)];
        printf("%lu - %lu - %d - %lc\n", pid, tid, i + 1, current_letter);
        Sleep(350);
    }

    printf("Поток %lu завершил выполнение (%d итераций)\n", tid, iterations);
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);

    wchar_t username[256];
    DWORD size = 256;
    GetUserNameW(username, &size);

    printf("=== Lab-04a: Управление потоками (WinAPI) ===\n");
    printf("Имя пользователя: %ls\n", username);
    printf("PID главного процесса: %lu\n", GetCurrentProcessId());
    printf("TID главного потока: %lu\n", GetCurrentThreadId());
    printf("Запуск потоков...\n");
    printf("=============================================\n");

    ThreadArgs args1 = { 50, username };
    ThreadArgs args2 = { 125, username };

    HANDLE hThread1 = CreateThread(NULL, 0, Lab_04x, &args1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, Lab_04x, &args2, 0, NULL);

    if (hThread1 == NULL || hThread2 == NULL) {
        printf("Ошибка: не удалось создать потоки!\n");
        return 1;
    }

    printf("Созданы потоки:\n");
    printf("- Поток 1: 50 итераций\n");
    printf("- Поток 2: 125 итераций\n");
    printf("- Главный поток: 100 итераций\n");
    printf("=============================================\n");

    ThreadArgs mainArgs = { 100, username };
    Lab_04x(&mainArgs);

    printf("Главный поток завершил работу. Ожидание дочерних потоков...\n");

    HANDLE threads[2] = { hThread1, hThread2 };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    printf("=============================================\n");
    printf("Все потоки завершены. Программа завершена.\n");
    printf("Нажмите Enter для выхода...\n");
    getchar();

    return 0;
}