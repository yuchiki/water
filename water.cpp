#include <stdio.h>
#include <unistd.h>
#include <vector>

using namespace std;

#define SIZE 20

double map[SIZE + 2][SIZE + 2];
double next_map[SIZE + 2][SIZE + 2];

class board {
    using column_t = vector<double>;
    using field_t  = vector<column_t>;

   private:
    vector<field_t> fields;
    int active_field = 0;

   public:
    const int Size = 20;
    board() {
        fields = vector<field_t>(2, field_t(Size + 2, column_t(Size + 2, 0)));
    }
    ~board() {}

    void flip() {
        active_field = 1 - active_field;
    }

    field_t &field() {
        return fields[active_field];
    }
};

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
        printf("**");
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

int count_tonarikabe(int i, int j) {
    if (i == 1 && j == 1 || i == 1 && j == SIZE || i == SIZE && j == 1 ||
        i == SIZE && j == SIZE)
        return 2;

    if (i == 1 || i == SIZE || j == 1 || j == SIZE) return 1;
    return 0;
}

int count_nanamekabe(int i, int j) {
    if (i == 1 && j == 1 || i == 1 && j == SIZE || i == SIZE && j == 1 ||
        i == SIZE && j == SIZE)
        return 3;

    if (i == 1 || i == SIZE || j == 1 || j == SIZE) return 2;
    return 0;
}

void update() {
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            next_map[i][j] = map[i][j];
        }
    }
    const double ratio = 0.05;
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            next_map[i - 1][j] += map[i][j] * ratio;
            next_map[i + 1][j] += map[i][j] * ratio;
            next_map[i][j - 1] += map[i][j] * ratio;
            next_map[i][j + 1] += map[i][j] * ratio;
            next_map[i - 1][j - 1] += map[i][j] * ratio / 1.4142;
            next_map[i - 1][j + 1] += map[i][j] * ratio / 1.4142;
            next_map[i + 1][j - 1] += map[i][j] * ratio / 1.4142;
            next_map[i + 1][j + 1] += map[i][j] * ratio / 1.4142;
            next_map[i][j] -= map[i][j] * ratio * (4 - count_tonarikabe(i, j));
            next_map[i][j] -=
                map[i][j] * ratio / 1.4142 * (4 - count_nanamekabe(i, j));
        }
    }

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            map[i][j] = next_map[i][j];
        }
    }
}

int main() {
    vector<int> a(10);
    map[10][10] = 338;
    while (1) {
        show();
        update();
        msleep(50);
    }
    return 0;
}