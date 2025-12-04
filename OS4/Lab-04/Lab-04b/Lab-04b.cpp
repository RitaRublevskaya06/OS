#include <windows.h>
#include <stdio.h>

HANDLE g_thread1_handle = NULL;
HANDLE g_thread2_handle = NULL;
DWORD g_thread1_id = 0;
DWORD g_thread2_id = 0;

void Lab_04x(int iterations, const wchar_t* username, int thread_num) {
    if (iterations <= 0) {
        printf("Ошибка: количество итераций должно быть положительным числом\n");
        return;
    }
    if (username == NULL || wcslen(username) == 0) {
        printf("Ошибка: имя пользователя не может быть пустым\n");
        return;
    }

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    if (thread_num == 1) g_thread1_id = tid;
    if (thread_num == 2) g_thread2_id = tid;

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
}

DWORD WINAPI ThreadFunc1(LPVOID param) {
    wchar_t* username = (wchar_t*)param;
    Lab_04x(50, username, 1);
    return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID param) {
    wchar_t* username = (wchar_t*)param;
    Lab_04x(125, username, 2);
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);

    wchar_t username[256];
    DWORD size = 256;
    GetUserNameW(username, &size);

    printf("=== Lab-04b: Управление потоками с приостановкой (WinAPI) ===\n");
    printf("Имя пользователя: %ls\n", username);
    printf("PID главного процесса: %lu\n", GetCurrentProcessId());
    printf("TID главного потока: %lu\n", GetCurrentThreadId());
    printf("=============================================================\n");

    g_thread1_handle = CreateThread(NULL, 0, ThreadFunc1, username, 0, &g_thread1_id);
    g_thread2_handle = CreateThread(NULL, 0, ThreadFunc2, username, 0, &g_thread2_id);

    if (g_thread1_handle == NULL || g_thread2_handle == NULL) {
        printf("Ошибка: не удалось создать потоки!\n");
        return 1;
    }

    printf("Созданы потоки:\n");
    printf("- Поток 1: 50 итераций (TID: %lu)\n", g_thread1_id);
    printf("- Поток 2: 125 итераций (TID: %lu)\n", g_thread2_id);
    printf("- Главный поток: 100 итераций с управлением\n");
    printf("=============================================================\n");

    DWORD main_pid = GetCurrentProcessId();
    DWORD main_tid = GetCurrentThreadId();

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

    for (int i = 0; i < 100; ++i) {
        wchar_t current_letter = clean_username[i % wcslen(clean_username)];
        printf("%lu - %lu - %d - %lc\n", main_pid, main_tid, i + 1, current_letter);

        if (i == 20) {
            printf("!!! Приостанавливаю Поток 1 на 20 итерации !!!\n");
            SuspendThread(g_thread1_handle);
        }
        if (i == 40) {
            printf("!!! Приостанавливаю Поток 2 на 40 итерации !!!\n");
            SuspendThread(g_thread2_handle);
        }
        if (i == 60) {
            printf("!!! Возобновляю Поток 1 на 60 итерации !!!\n");
            ResumeThread(g_thread1_handle);
        }
        Sleep(350);
    }

    printf("=============================================================\n");
    printf("Главный поток завершил цикл. Возобновляю Поток 2...\n");
    ResumeThread(g_thread2_handle);

    HANDLE threads[2] = { g_thread1_handle, g_thread2_handle };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(g_thread1_handle);
    CloseHandle(g_thread2_handle);

    printf("=============================================================\n");
    printf("Все потоки завершены. Программа завершена.\n");
    printf("Нажмите Enter для выхода...\n");
    getchar();

    return 0;
}