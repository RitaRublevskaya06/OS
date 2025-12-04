#include <windows.h>
#include <stdio.h>

void Lab_04x(int iterations, const wchar_t* username) {
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

    try {
        for (int i = 0; i < iterations; ++i) {
            wchar_t current_letter = clean_username[i % wcslen(clean_username)];
            printf("PID - %lu  TID - %lu - %d - %lc\n",
                pid, tid, i + 1, current_letter);
            fflush(stdout);
            Sleep(350);
        }
    }
    catch (...) {
        printf("Исключение в потоке\n");
    }

    printf("Поток %lu завершил выполнение (%d итераций)\n", tid, iterations);
}

DWORD WINAPI ThreadFunc(LPVOID param) {
    wchar_t* username = (wchar_t*)param;
    Lab_04x(7, username);
    return 0;
}

int main() {
    SetConsoleOutputCP(1251);

    wchar_t username[256];
    DWORD size = 256;
    GetUserNameW(username, &size);
    printf("%ls\n", username);

    Lab_04x(21, username);

    HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, username, 0, NULL);
    if (hThread == NULL) {
        printf("Не удалось создать поток!\n");
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    return 0;
}