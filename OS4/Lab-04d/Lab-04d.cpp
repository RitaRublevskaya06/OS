#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
int main() {
    setlocale(LC_ALL, "RUS");
    std::cout << "Lab-04d: Список потоков процесса" << std::endl;
    DWORD currentPID = GetCurrentProcessId();
    std::cout << "PID текущего процесса: " << currentPID << std::endl;
    std::cout << "TID главного потока: " << GetCurrentThreadId() << std::endl;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Ошибка: не удалось создать снимок потоков" << std::endl;
        return 1;
    }
    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    int threadCount = 0;
    std::cout << "Список потоков текущего процесса:" << std::endl;
    if (Thread32First(hSnapshot, &te32)) {
        do {
            if (te32.th32OwnerProcessID == currentPID) {
                threadCount++;
                std::cout << "Поток " << threadCount << ": TID = " << te32.th32ThreadID << std::endl;
            }
        } while (Thread32Next(hSnapshot, &te32));
    }
    CloseHandle(hSnapshot);
    std::cout << "Всего потоков в процессе: " << threadCount << std::endl;
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();
    return 0;
}
