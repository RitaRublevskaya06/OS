#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc != 3) {
        printf("Использование: %s <нижняя_граница> <верхняя_граница>\n", argv[0]);
        return 1;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);

    if (lower >= upper || lower < 0) {
        printf("Ошибка: Неверный диапазон\n");
        return 1;
    }

    HANDLE hPipe = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Ошибка: неверный дескриптор канала\n");
        return 1;
    }

    char buffer[32];
    DWORD written = 0;

    for (int i = lower; i <= upper; i++) {
        if (is_prime(i)) {
            sprintf(buffer, "%d\n", i);
            WriteFile(hPipe, buffer, strlen(buffer), &written, NULL);
        }
    }

    CloseHandle(hPipe);
    return 0;
}