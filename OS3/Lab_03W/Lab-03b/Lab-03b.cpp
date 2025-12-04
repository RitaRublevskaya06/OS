#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    STARTUPINFO si[3];
    PROCESS_INFORMATION pi[3];
    TCHAR cmdLineForFirst[] = _T("Lab-03x.exe");
    TCHAR cmdLineWithArgs[] = _T("Lab-03x.exe 3");
    TCHAR appName[] = _T("Lab-03x.exe");

    printf("Создание 3 процессов с разными параметрами...\n");
    printf("Изменение локальной переменной ITER_NUM=2\n");

    _tputenv(_T("ITER_NUM=2"));

    for (int i = 0; i < 3; i++) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));
    }

    BOOL success[3];

    printf("\nСоздание процесса 1 (только 1 параметр)...\n");

    success[0] = CreateProcess(
        cmdLineForFirst,
        NULL,
        NULL, NULL, FALSE, 0, NULL, NULL, &si[0], &pi[0]
    );
    Sleep(2500);

    printf("Создание процесса 2 (только 2 параметр)...\n");

    success[1] = CreateProcess(
        NULL, cmdLineWithArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si[1], &pi[1]
    );
    Sleep(2500);

    printf("Создание процесса 3 (имя в первом,параметры во втором)...\n");

    success[2] = CreateProcess(
        appName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si[2], &pi[2]
    );
    Sleep(2500);

    for (int i = 0; i < 3; i++) {
        if (success[i]) {
            printf("Процесс %d создан успешно. PID: %d\n", (i + 1), pi[i].dwProcessId);
        }
        else {
            printf("ошибка при создании процесса %d. ошибка: %d\n", (i + 1), GetLastError());
        }
    }

    HANDLE hProcesses[3];
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (success[i]) {
            hProcesses[count++] = pi[i].hProcess;
        }
    }

    if (count > 0) {
        WaitForMultipleObjects(count, hProcesses, TRUE, INFINITE);
    }

    for (int i = 0; i < 3; i++) {
        if (success[i]) {
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
    }

    return 0;
}