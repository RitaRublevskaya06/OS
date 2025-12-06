#include <windows.h>
#include <cstdio>
#include <cstdlib>

HANDLE hConsoleMutex;

DWORD WINAPI ThreadFunction(LPVOID lpParameter) {
    int id = (int)(intptr_t)lpParameter;
    const int TOTAL_ITERATIONS = 1000000;
    const int REPORT_INTERVAL = 1000;
    const int DELAY_MS = 200;

    WaitForSingleObject(hConsoleMutex, INFINITE);
    printf("Поток %d начал выполнение\n", id);
    ReleaseMutex(hConsoleMutex);

    for (int i = 1; i <= TOTAL_ITERATIONS; i++) {
        if (i % REPORT_INTERVAL == 0) {
            DWORD processId = GetCurrentProcessId();
            DWORD threadId = GetCurrentThreadId();
            DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
            int threadPriority = GetThreadPriority(GetCurrentThread());
            DWORD currentProcessor = GetCurrentProcessorNumber();

            WaitForSingleObject(hConsoleMutex, INFINITE);
            printf("[Поток %d] Итерация %6d | PID: %lu | TID: %lu | Класс: %lu | Приоритет: %d | CPU: %lu\n",
                id, i, processId, threadId, priorityClass, threadPriority, currentProcessor);
            ReleaseMutex(hConsoleMutex);

            Sleep(DELAY_MS);
        }
    }

    WaitForSingleObject(hConsoleMutex, INFINITE);
    printf("Поток %d завершил выполнение\n", id);
    ReleaseMutex(hConsoleMutex);

    return 0;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    hConsoleMutex = CreateMutex(NULL, FALSE, NULL);
    if (hConsoleMutex == NULL) {
        printf("Ошибка создания мьютекса!\n");
        return 1;
    }

    if (argc != 5) {
        printf("================================================\n");
        printf("           Lab-05c - Управление потоками\n");
        printf("================================================\n");
        printf("Использование: Lab-05c.exe <маска_родственности> <класс_процесса> <приоритет_потока1> <приоритет_потока2>\n");
        printf("Пример: Lab-05c.exe 255 32 -2 2\n");
        printf("================================================\n");
        CloseHandle(hConsoleMutex);
        return 1;
    }

    DWORD affinity = strtoul(argv[1], NULL, 10);
    DWORD processPriority = strtoul(argv[2], NULL, 10);
    int threadPriority1 = atoi(argv[3]);
    int threadPriority2 = atoi(argv[4]);

    printf("================================================\n");
    printf("           Lab-05c - Управление потоками\n");
    printf("================================================\n");
    printf("Параметры запуска:\n");
    printf("Маска родственности: %lu\n", affinity);
    printf("Класс процесса:      %lu\n", processPriority);
    printf("Приоритет потока 1:  %d\n", threadPriority1);
    printf("Приоритет потока 2:  %d\n", threadPriority2);
    printf("------------------------------------------------\n");

    if (!SetProcessAffinityMask(GetCurrentProcess(), affinity)) {
        printf("Ошибка установки маски процессора!\n");
    }

    if (!SetPriorityClass(GetCurrentProcess(), processPriority)) {
        printf("Ошибка установки класса приоритета процесса!\n");
    }

    printf("Создание потоков:\n");
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, (LPVOID)1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, (LPVOID)2, 0, NULL);

    if (hThread1 == NULL || hThread2 == NULL) {
        printf("Ошибка создания потоков!\n");
        CloseHandle(hConsoleMutex);
        return 1;
    }

    if (!SetThreadPriority(hThread1, threadPriority1)) {
        printf("Ошибка установки приоритета потока 1!\n");
    }

    if (!SetThreadPriority(hThread2, threadPriority2)) {
        printf("Ошибка установки приоритета потока 2!\n");
    }

    printf("------------------------------------------------\n");
    printf("Ожидание завершения потоков...\n");

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    printf("------------------------------------------------\n");
    printf("Все потоки завершили работу\n");
    printf("================================================\n");
    printf("Нажмите Enter для выхода...");
    getchar();

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hConsoleMutex);

    return 0;
}