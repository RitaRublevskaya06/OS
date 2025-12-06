#include <windows.h>
#include <cstdio>
#include <cstdlib>

void LaunchChildProcess(DWORD affinity, DWORD priorityClass, const char* name) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char commandLine[] = "Lab-05x.exe";

    if (CreateProcessA(
        NULL,
        commandLine,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi)) {

        SetProcessAffinityMask(pi.hProcess, affinity);
        SetPriorityClass(pi.hProcess, priorityClass);

        printf("Успешно запущен %s  ||  PID:   %lu  ||  Маска родственности:   %lu  ||  Приоритет:   %lu\n",
            name, pi.dwProcessId, affinity, priorityClass);

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else {
        printf("Ошибка запуска! %s\n", name);
    }
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc != 4) {
        printf("================================================\n");
        printf("           Lab-05b - Запуск процессов\n");
        printf("================================================\n");
        printf("Использование:  Lab-05b.exe <маска_родственности> <приоритет1> <приоритет2>\n");
        printf("Пример:  Lab-05b.exe 15 32 128\n");
        printf("================================================\n");
        return 1;
    }

    DWORD affinity = strtoul(argv[1], NULL, 10);
    DWORD priority1 = strtoul(argv[2], NULL, 10);
    DWORD priority2 = strtoul(argv[3], NULL, 10);

    printf("================================================\n");
    printf("           Lab-05b - Запуск процессов\n");
    printf("================================================\n");
    printf("Параметры запуска:\n");
    printf("Маска родственности: %lu\n", affinity);
    printf("Приоритет процесса 1: %lu\n", priority1);
    printf("Приоритет процесса 2: %lu\n", priority2);
    printf("------------------------------------------------\n");

    printf("Запуск процессов:\n");
    LaunchChildProcess(affinity, priority1, "Процесс 1");
    Sleep(1000);
    LaunchChildProcess(affinity, priority2, "Процесс 2");

    printf("------------------------------------------------\n");
    printf("Оба процесса запущены.\n");
    printf("================================================\n");
    printf("Нажмите Enter для выхода...");
    getchar();

    return 0;
}