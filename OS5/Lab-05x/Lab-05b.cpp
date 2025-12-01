#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

void LaunchChildProcess(int affinity, int priorityClass, const std::string& name) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string commandLine = "Lab-05x.exe";
    if (CreateProcessA(
        NULL,
        const_cast<LPSTR>(commandLine.c_str()),
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

        std::cout << "Успешно запущен " << name
            << "  ||  PID:   " << pi.dwProcessId
            << "  ||  Маска родственности:   " << affinity
            << "  ||  Приоритет:   " << priorityClass << std::endl;
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else {
        std::cerr << "Ошибка запуска!" << name << std::endl;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");

    if (argc != 4) {
        std::cout << "================================================" << std::endl;
        std::cout << "           Lab-05b - Запуск процессов" << std::endl;
        std::cout << "================================================" << std::endl;
        std::cout << "Использование:  Lab-05b.exe <маска_родственности> <приоритет1> <приоритет2>" << std::endl;
        std::cout << "Пример:  Lab-05b.exe 15 32 128" << std::endl;
        std::cout << "================================================" << std::endl;
        return 1;
    }

    DWORD affinity = std::stoul(argv[1]);
    DWORD priority1 = std::stoul(argv[2]);
    DWORD priority2 = std::stoul(argv[3]);

    std::cout << "================================================" << std::endl;
    std::cout << "           Lab-05b - Запуск процессов" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Параметры запуска:" << std::endl;
    std::cout << "Маска родственности: " << affinity << std::endl;
    std::cout << "Приоритет процесса 1: " << priority1 << std::endl;
    std::cout << "Приоритет процесса 2: " << priority2 << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    std::cout << "Запуск процессов:" << std::endl;
    LaunchChildProcess(affinity, priority1, "Процесс 1");
    Sleep(1000);
    LaunchChildProcess(affinity, priority2, "Процесс 2");

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Оба процесса запущены." << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();

    return 0;
}
