#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Ошибка создания снимка: %d\n", GetLastError());
        return 1;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe)) {
        printf("Ошибка получения информации процесса: %d\n", GetLastError());
        CloseHandle(hSnapshot);
        return 1;
    }

    printf("%-30s%-10s%-10s\n", "Имя", "PID", "Родитель PID");
    printf("------------------------------------------------\n");

    do {
        char processName[MAX_PATH];
        WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, processName, MAX_PATH, NULL, NULL);

        printf("%-30s%-10d%-10d\n", processName, pe.th32ProcessID, pe.th32ParentProcessID);

    } while (Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return 0;
}