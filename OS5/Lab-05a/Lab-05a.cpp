#include <windows.h>
#include <iostream>
#include <bitset>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "RUS");

    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
    int threadPriority = GetThreadPriority(GetCurrentThread());
    DWORD_PTR processAffinity, systemAffinity;
    GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity);
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD currentProcessor = GetCurrentProcessorNumber();

    std::cout << "================================================" << std::endl;
    std::cout << "           Lab-05a - Информация системы" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Основная информация:" << std::endl;
    std::cout << "Идентификатор текущего процесса:  " << processId << std::endl;
    std::cout << "Идентификатор текущего потока:    " << threadId << std::endl;
    std::cout << "Класс приоритетов процесса:       " << priorityClass << std::endl;
    std::cout << "Приоритет текущего потока:        " << threadPriority << std::endl;
    std::cout << std::endl;

    std::cout << "Маски родственности:" << std::endl;
    std::cout << "Маска процесса:    " << std::bitset<64>(processAffinity) << std::endl;
    std::cout << "Системная маска:   " << std::bitset<64>(systemAffinity) << std::endl;
    std::cout << std::endl;

    std::cout << "Информация о процессорах:" << std::endl;
    std::cout << "Доступно процессоров:        " << sysInfo.dwNumberOfProcessors << std::endl;
    std::cout << "Текущий процессор потока:    " << currentProcessor << std::endl;
    std::cout << std::endl;

    std::cout << "================================================" << std::endl;
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();

    return 0;
}