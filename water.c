#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 20

double map[SIZE + 2][SIZE + 2];
double next_map[SIZE + 2][SIZE + 2];

void msleep(int millisecond) {
    usleep(millisecond * 1000);
}

void colorntf(double percent) {
    const double part = 15;
    if (percent < 1 / part)
        printf("%3.3lf ", percent);
    else if (percent < 2 / part)
        printf("\x1b[37m%3.3lf\x1b[39m ", percent);
    else if (percent < 3 / part)
        printf("\x1b[36m%3.3lf\x1b[39m ", percent);
    else if (percent < 4 / part)
        printf("\x1b[35m%3.3lf\x1b[39m ", percent);
    else if (percent < 5 / part)
        printf("\x1b[34m%3.3lf\x1b[39m ", percent);
    else if (percent < 6 / part)
        printf("\x1b[33m%3.3lf\x1b[39m ", percent);
    else if (percent < 7 / part)
        printf("\x1b[32m%3.3lf\x1b[39m ", percent);
    else if (percent < 8 / part)
        printf("\x1b[31m%3.3lf\x1b[39m ", percent);
    else if (percent < 9 / part)
        printf("\x1b[47m%3.3lf\x1b[49m ", percent);
    else if (percent < 10 / part)
        printf("\x1b[46m%3.3lf\x1b[49m ", percent);
    else if (percent < 11 / part)
        printf("\x1b[45m%3.3lf\x1b[49m ", percent);
    else if (percent < 12 / part)
        printf("\x1b[44m%3.3lf\x1b[49m ", percent);
    else if (percent < 13 / part)
        printf("\x1b[43m%3.3lf\x1b[49m ", percent);
    else if (percent < 14 / part)
        printf("\x1b[42m%3.3lf\x1b[49m ", percent);
    else
        printf("\x1b[41m%3.3lf\x1b[49m ", percent);
}

void colorabstf(double percent) {
    const double part = 15;
    if (percent < 1 / part)
        printf("**", percent);
    else if (percent < 2 / part)
        printf("\x1b[37m**\x1b[39m");
    else if (percent < 3 / part)
        printf("\x1b[36m**\x1b[39m");
    else if (percent < 4 / part)
        printf("\x1b[35m**\x1b[39m");
    else if (percent < 5 / part)
        printf("\x1b[34m**\x1b[39m");
    else if (percent < 6 / part)
        printf("\x1b[33m**\x1b[39m");
    else if (percent < 7 / part)
        printf("\x1b[32m**\x1b[39m");
    else if (percent < 8 / part)
        printf("\x1b[31m**\x1b[39m");
    else if (percent < 9 / part)
        printf("\x1b[47m**\x1b[49m");
    else if (percent < 10 / part)
        printf("\x1b[46m**\x1b[49m");
    else if (percent < 11 / part)
        printf("\x1b[45m**\x1b[49m");
    else if (percent < 12 / part)
        printf("\x1b[44m**\x1b[49m");
    else if (percent < 13 / part)
        printf("\x1b[43m**\x1b[49m");
    else if (percent < 14 / part)
        printf("\x1b[42m**\x1b[49m");
    else
        printf("\x1b[41m**\x1b[49m");
}

void show() {
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            colorabstf(map[i][j]);
        }
        puts("");
    }
    puts("");
}

int count_kabe(int i, int j) {
    if (i == 1 && j == 1 || i == 1 && j == SIZE || i == SIZE && j == 1 ||
        i == SIZE && j == SIZE)
        return 5;

    if (i == 1 || i == SIZE || j == 1 || j == SIZE) return 3;
    return 0;
}

void update() {
    map[20][20] = 1;
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            next_map[i][j] = map[i][j];
        }
    }
    const double ratio = 0.05;
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    next_map[i + k][j + l] += map[i][j] * ratio;
                }
            }
            next_map[i][j] -= map[i][j] * ratio * (9 - count_kabe(i, j));
        }
    }

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            map[i][j] = next_map[i][j];
        }
    }
}
int main() {
    map[7][4] = 338;
    while (1) {
        show();
        update();
        msleep(50);
    }
    return 0;
}