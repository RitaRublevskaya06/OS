#include <windows.h>
#include <stdio.h>

HANDLE g_thread1_handle = NULL;
HANDLE g_thread2_handle = NULL;
DWORD g_thread1_id = 0;
DWORD g_thread2_id = 0;
BOOL g_thread2_terminated = FALSE;

typedef struct {
    int iterations;
    wchar_t* username;
    int thread_num;
} ThreadParams;

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    ThreadParams* params = (ThreadParams*)lpParam;
    int iterations = params->iterations;
    wchar_t* username = params->username;
    int thread_num = params->thread_num;

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
        printf("PID: %lu - TID: %lu - %d - %lc\n", pid, tid, i + 1, current_letter);
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

    printf("=== Lab-04c: Управление потоками с завершением ===\n");
    printf("Имя пользователя: %ls\n", username);
    printf("PID главного процесса: %lu\n", GetCurrentProcessId());
    printf("TID главного потока: %lu\n", GetCurrentThreadId());
    printf("==================================================\n");

    ThreadParams params1 = { 50, username, 1 };
    ThreadParams params2 = { 125, username, 2 };

    g_thread1_handle = CreateThread(NULL, 0, ThreadProc, &params1, 0, &g_thread1_id);
    g_thread2_handle = CreateThread(NULL, 0, ThreadProc, &params2, 0, &g_thread2_id);

    Sleep(100);

    printf("Созданы потоки:\n");
    printf("- Поток 1: 50 итераций (TID: %lu)\n", g_thread1_id);
    printf("- Поток 2: 125 итераций (TID: %lu)\n", g_thread2_id);
    printf("- Главный поток: 100 итераций с управлением\n");

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
        printf("PID: %lu - TID: %lu - %d - %lc\n", GetCurrentProcessId(), GetCurrentThreadId(), i + 1, current_letter);

        if (i == 40 && !g_thread2_terminated && g_thread2_handle != NULL) {
            printf("!!! Завершение потока 2 на 40 итерации\n");
            if (TerminateThread(g_thread2_handle, 0)) {
                printf("Поток 2 успешно завершен!\n");
                g_thread2_terminated = TRUE;
                CloseHandle(g_thread2_handle);
                g_thread2_handle = NULL;
            }
            else {
                printf("Ошибка завершения потока! Код: %lu\n", GetLastError());
            }
        }
        Sleep(350);
    }

    printf("Главный поток завершил работу. Ожидание дочерних потоков...\n");

    if (g_thread1_handle != NULL) {
        WaitForSingleObject(g_thread1_handle, INFINITE);
        CloseHandle(g_thread1_handle);
        printf("Поток 1 завершился\n");
    }

    if (!g_thread2_terminated && g_thread2_handle != NULL) {
        WaitForSingleObject(g_thread2_handle, INFINITE);
        CloseHandle(g_thread2_handle);
        printf("Поток 2 завершился\n");
    }

    printf("Все потоки завершены. Программа завершена.\n");
    printf("Нажмите Enter для выхода...\n");
    getchar();

    return 0;
}