#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

HANDLE hConsoleMutex;

DWORD WINAPI ThreadFunction(LPVOID lpParameter) {
    int id = (int)(intptr_t)lpParameter;
    const int TOTAL_ITERATIONS = 1000000;
    const int REPORT_INTERVAL = 1000;
    const int DELAY_MS = 200;

    WaitForSingleObject(hConsoleMutex, INFINITE);
    std::cout << "Поток " << id << " начал выполнение" << std::endl;
    ReleaseMutex(hConsoleMutex);

    for (int i = 1; i <= TOTAL_ITERATIONS; i++) {
        if (i % REPORT_INTERVAL == 0) {
            DWORD processId = GetCurrentProcessId();
            DWORD threadId = GetCurrentThreadId();
            DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
            int threadPriority = GetThreadPriority(GetCurrentThread());
            DWORD currentProcessor = GetCurrentProcessorNumber();

            // Синхронизированный вывод
            WaitForSingleObject(hConsoleMutex, INFINITE);
            std::cout << "[Поток " << id << "] Итерация " << std::setw(6) << i
                << " | PID: " << processId
                << " | TID: " << threadId
                << " | Класс: " << priorityClass
                << " | Приоритет: " << threadPriority
                << " | CPU: " << currentProcessor << std::endl;
            ReleaseMutex(hConsoleMutex);

            Sleep(DELAY_MS);
        }
    }

    WaitForSingleObject(hConsoleMutex, INFINITE);
    std::cout << "Поток " << id << " завершил выполнение" << std::endl;
    ReleaseMutex(hConsoleMutex);

    return 0;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");

    // Создаем мьютекс для синхронизации вывода
    hConsoleMutex = CreateMutex(NULL, FALSE, NULL);
    if (hConsoleMutex == NULL) {
        std::cerr << "Ошибка создания мьютекса!" << std::endl;
        return 1;
    }

    if (argc != 5) {
        std::cout << "================================================" << std::endl;
        std::cout << "           Lab-05c - Управление потоками" << std::endl;
        std::cout << "================================================" << std::endl;
        std::cout << "Использование: Lab-05c.exe <маска_родственности> <класс_процесса> <приоритет_потока1> <приоритет_потока2>" << std::endl;
        std::cout << "Пример: Lab-05c.exe 255 32 -2 2" << std::endl;
        std::cout << "================================================" << std::endl;
        CloseHandle(hConsoleMutex);
        return 1;
    }

    DWORD affinity = std::stoul(argv[1]);
    DWORD processPriority = std::stoul(argv[2]);
    int threadPriority1 = std::stoi(argv[3]);
    int threadPriority2 = std::stoi(argv[4]);

    std::cout << "================================================" << std::endl;
    std::cout << "           Lab-05c - Управление потоками" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Параметры запуска:" << std::endl;
    std::cout << "Маска родственности: " << affinity << std::endl;
    std::cout << "Класс процесса:      " << processPriority << std::endl;
    std::cout << "Приоритет потока 1:  " << threadPriority1 << std::endl;
    std::cout << "Приоритет потока 2:  " << threadPriority2 << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    // Установка параметров процесса
    if (!SetProcessAffinityMask(GetCurrentProcess(), affinity)) {
        std::cerr << "Ошибка установки маски процессора!" << std::endl;
    }

    if (!SetPriorityClass(GetCurrentProcess(), processPriority)) {
        std::cerr << "Ошибка установки класса приоритета процесса!" << std::endl;
    }

    std::cout << "Создание потоков:" << std::endl;
    HANDLE hThread1 = CreateThread(NULL, 0, ThreadFunction, (LPVOID)1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ThreadFunction, (LPVOID)2, 0, NULL);

    if (hThread1 == NULL || hThread2 == NULL) {
        std::cerr << "Ошибка создания потоков!" << std::endl;
        CloseHandle(hConsoleMutex);
        return 1;
    }

    // Установка приоритетов потоков
    if (!SetThreadPriority(hThread1, threadPriority1)) {
        std::cerr << "Ошибка установки приоритета потока 1!" << std::endl;
    }

    if (!SetThreadPriority(hThread2, threadPriority2)) {
        std::cerr << "Ошибка установки приоритета потока 2!" << std::endl;
    }

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Ожидание завершения потоков..." << std::endl;

    // Ожидание завершения потоков
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Все потоки завершили работу" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hConsoleMutex);

    return 0;
}