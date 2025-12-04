#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc != 4) {
        printf("Использование: %s <количество_процессов> <нижняя_граница> <верхняя_граница>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    int lower = atoi(argv[2]);
    int upper = atoi(argv[3]);

    if (num_processes <= 0 || lower >= upper || lower < 0) {
        printf("Ошибка: Неверные параметры\n");
        return 1;
    }

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        printf("Ошибка создания канала: %d\n", GetLastError());
        return 1;
    }

    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    printf("Сервер запущен. Создание %d клиентских процессов...\n", num_processes);
    printf("Диапазон поиска: %d до %d\n", lower, upper);

    STARTUPINFO* si = (STARTUPINFO*)malloc(num_processes * sizeof(STARTUPINFO));
    PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)malloc(num_processes * sizeof(PROCESS_INFORMATION));

    if (!si || !pi) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }

    int range_size = (upper - lower + 1) / num_processes;
    int successful_processes = 0;

    for (int i = 0; i < num_processes; i++) {
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);
        si[i].hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si[i].hStdOutput = hWritePipe;
        si[i].hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si[i].dwFlags |= STARTF_USESTDHANDLES;

        ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

        int proc_lower = lower + i * range_size;
        int proc_upper = (i == num_processes - 1) ? upper : proc_lower + range_size - 1;

        // Используем широкие символы для CreateProcess
        wchar_t cmdLine[256];
        swprintf(cmdLine, 256, L"Lab-03d-client.exe %d %d", proc_lower, proc_upper);

        printf("Запуск процесса %d с диапазоном %d до %d\n", i, proc_lower, proc_upper);

        BOOL success = CreateProcess(
            NULL,
            cmdLine,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &si[i],
            &pi[i]
        );

        if (!success) {
            printf("Ошибка создания процесса %d: %d\n", i, GetLastError());
        }
        else {
            printf("Процесс %d успешно создан. PID: %d\n", i, pi[i].dwProcessId);
            successful_processes++;
        }
    }

    CloseHandle(hWritePipe);

    printf("\n=== Найденные простые числа ===\n");

    char buffer[4096];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    if (successful_processes > 0) {
        printf("\nОжидание завершения всех процессов...\n");

        HANDLE* processHandles = (HANDLE*)malloc(successful_processes * sizeof(HANDLE));
        if (processHandles) {
            int handleCount = 0;
            for (int i = 0; i < num_processes; i++) {
                if (pi[i].hProcess != NULL) {
                    processHandles[handleCount++] = pi[i].hProcess;
                }
            }
            WaitForMultipleObjects(handleCount, processHandles, TRUE, INFINITE);
            free(processHandles);
        }
    }

    CloseHandle(hReadPipe);

    for (int i = 0; i < num_processes; i++) {
        if (pi[i].hProcess) CloseHandle(pi[i].hProcess);
        if (pi[i].hThread) CloseHandle(pi[i].hThread);
    }

    free(si);
    free(pi);

    printf("Сервер успешно завершил работу.\n");
    return 0;
}