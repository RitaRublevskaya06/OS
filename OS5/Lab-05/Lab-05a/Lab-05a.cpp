#include <windows.h>
#include <cstdio>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    DWORD priorityClass = GetPriorityClass(GetCurrentProcess());
    int threadPriority = GetThreadPriority(GetCurrentThread());
    DWORD_PTR processAffinity, systemAffinity;
    GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity);
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    DWORD currentProcessor = GetCurrentProcessorNumber();

    printf("================================================\n");
    printf("           Lab-05a - Информация системы\n");
    printf("================================================\n\n");

    printf("Основная информация:\n");
    printf("Идентификатор текущего процесса:  %lu\n", processId);
    printf("Идентификатор текущего потока:    %lu\n", threadId);
    printf("Класс приоритетов процесса:       %lu\n", priorityClass);
    printf("Приоритет текущего потока:        %d\n\n", threadPriority);

    printf("Маски родственности:\n");
    printf("Маска процесса:    ");
    for (int i = 63; i >= 0; i--) {
        printf("%d", (processAffinity >> i) & 1);
    }
    printf("\nСистемная маска:   ");
    for (int i = 63; i >= 0; i--) {
        printf("%d", (systemAffinity >> i) & 1);
    }
    printf("\n\n");

    printf("Информация о процессорах:\n");
    printf("Доступно процессоров:        %lu\n", sysInfo.dwNumberOfProcessors);
    printf("Текущий процессор потока:    %lu\n\n", currentProcessor);

    printf("================================================\n");
    printf("Нажмите Enter для выхода...");
    getchar();

    return 0;
}