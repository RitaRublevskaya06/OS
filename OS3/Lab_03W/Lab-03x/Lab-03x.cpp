#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int iterations = 0;

    if (argc > 1) {
        iterations = atoi(argv[1]);
    }
    else {
        char* env_iter = getenv("ITER_NUM");
        if (env_iter != NULL) {
            iterations = atoi(env_iter);
        }
        else {
            printf("Ошибка: не задано количество итераций!\n");
            ExitProcess(1);
        }
    }

    if (iterations <= 0) {
        printf("Ошибка: некорректное количество итераций!\n");
        ExitProcess(1);
    }

    printf("создан PID: %d, Итераций: %d\n", GetCurrentProcessId(), iterations);

    for (int i = 0; i < iterations; i++) {
        printf("PID: %d, Итерация: %d\n", GetCurrentProcessId(), (i + 1));
        Sleep(500);
    }

    return 0;
}