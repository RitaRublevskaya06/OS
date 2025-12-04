#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

int main() {
    SetConsoleOutputCP(1251);

    printf("Lab-04d: Список потоков процесса\n");

    DWORD currentPID = GetCurrentProcessId();
    printf("PID текущего процесса: %lu\n", currentPID);
    printf("TID главного потока: %lu\n", GetCurrentThreadId());

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Ошибка: не удалось создать снимок потоков\n");
        return 1;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    int threadCount = 0;

    printf("Список потоков текущего процесса:\n");

    if (Thread32First(hSnapshot, &te32)) {
        do {
            if (te32.th32OwnerProcessID == currentPID) {
                threadCount++;
                printf("Поток %d: TID = %lu\n", threadCount, te32.th32ThreadID);
            }
        } while (Thread32Next(hSnapshot, &te32));
    }

    CloseHandle(hSnapshot);

    printf("Всего потоков в процессе: %d\n", threadCount);
    printf("Нажмите Enter для выхода...");
    getchar();

    return 0;
}