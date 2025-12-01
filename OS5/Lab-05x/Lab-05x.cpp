#include <windows.h>
#include <iostream>
#include <ctime>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "RUS");
    clock_t start = clock();
    const int TOTAL_ITERATIONS = 1000000;
    const int REPORT_INTERVAL = 1000;
    const int DELAY_MS = 200;

    std::cout << "================================================" << std::endl;
    std::cout << "           Lab-05x - Выполнение цикла" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Параметры выполнения:" << std::endl;
    std::cout << "Всего итераций: " << TOTAL_ITERATIONS << std::endl;
    std::cout << "Отчет каждые:   " << REPORT_INTERVAL << " итераций" << std::endl;
    std::cout << "Задержка:       " << DELAY_MS << " мс" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (int i = 1; i <= TOTAL_ITERATIONS; i++) {
        if (i % REPORT_INTERVAL == 0) {
            DWORD processId = GetCurrentProcessId();
            DWORD threadId = GetCurrentThreadId();
            DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
            int threadPriority = GetThreadPriority(GetCurrentThread());
            DWORD currentProcessor = GetCurrentProcessorNumber();

            std::cout << "Итерация " << i
                << "  ||  PID:   " << processId
                << "  ||  TID:   " << threadId
                << "  ||  Класс:   " << priorityClass
                << "  ||  Приоритет:   " << threadPriority
                << "  ||  Процессор:   " << currentProcessor << std::endl;
            Sleep(DELAY_MS);
        }
    }

    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Результаты выполнения:" << std::endl;
    std::cout << "Время выполнения: " << std::fixed << std::setprecision(3)
        << elapsed << " секунд" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();

    return 0;
}