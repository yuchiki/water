#include <stdio.h>
#include <unistd.h>
#include <string>
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

void colorabstf(double percent) {
    percent = min(percent, 0.999999);
    vector<string> formatter{"**",
                             "\x1b[37m**\x1b[39m",
                             "\x1b[36m**\x1b[39m",
                             "\x1b[35m**\x1b[39m",
                             "\x1b[34m**\x1b[39m",
                             "\x1b[33m**\x1b[39m",
                             "\x1b[32m**\x1b[39m",
                             "\x1b[31m**\x1b[39m",
                             "\x1b[47m**\x1b[49m",
                             "\x1b[46m**\x1b[49m",
                             "\x1b[45m**\x1b[49m",
                             "\x1b[44m**\x1b[49m",
                             "\x1b[43m**\x1b[49m",
                             "\x1b[42m**\x1b[49m",
                             "\x1b[41m**\x1b[49m"};
    printf(formatter[percent * formatter.size()].c_str());
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

bool is_corner(int i, int j) {
    return i == 1 && j == 1 || i == 1 && j == SIZE || i == SIZE && j == 1 ||
           i == SIZE && j == SIZE;
}

bool is_on_edge(int i, int j) {
    return i == 1 || i == SIZE || j == 1 || j == SIZE;
}

int count_tonarikabe(int i, int j) {
    return is_corner(i, j) ? 2 : is_on_edge(i, j) ? 1 : 0;
}

int count_nanamekabe(int i, int j) {
    return is_corner(i, j) ? 3 : is_on_edge(i, j) ? 2 : 0;
}

int count_kabe(int i, int j) {
    return count_tonarikabe(i, j) + count_nanamekabe(i, j);
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

int initialize() {
    map[10][10] = 338;
}

int main() {
    initialize();

    while (true) {
        show();
        update();
        msleep(50);
    }
    return 0;
}