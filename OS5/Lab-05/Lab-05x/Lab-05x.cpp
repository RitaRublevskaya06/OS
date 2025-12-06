#include <windows.h>
#include <cstdio>
#include <ctime>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    clock_t start = clock();
    const int TOTAL_ITERATIONS = 1000000;
    const int REPORT_INTERVAL = 1000;
    const int DELAY_MS = 200;

    printf("================================================\n");
    printf("           Lab-05x - Выполнение цикла\n");
    printf("================================================\n");
    printf("Параметры выполнения:\n");
    printf("Всего итераций: %d\n", TOTAL_ITERATIONS);
    printf("Отчет каждые:   %d итераций\n", REPORT_INTERVAL);
    printf("Задержка:       %d мс\n", DELAY_MS);
    printf("------------------------------------------------\n");

    for (int i = 1; i <= TOTAL_ITERATIONS; i++) {
        if (i % REPORT_INTERVAL == 0) {
            DWORD processId = GetCurrentProcessId();
            DWORD threadId = GetCurrentThreadId();
            DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
            int threadPriority = GetThreadPriority(GetCurrentThread());
            DWORD currentProcessor = GetCurrentProcessorNumber();

            printf("Итерация %d  ||  PID:   %lu  ||  TID:   %lu  ||  Класс:   %lu  ||  Приоритет:   %d  ||  Процессор:   %lu\n",
                i, processId, threadId, priorityClass, threadPriority, currentProcessor);
            Sleep(DELAY_MS);
        }
    }

    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;

    printf("------------------------------------------------\n");
    printf("Результаты выполнения:\n");
    printf("Время выполнения: %.3f секунд\n", elapsed);
    printf("================================================\n");
    printf("Нажмите Enter для выхода...");
    getchar();

    return 0;
}