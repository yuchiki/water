#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

using namespace std;

#define SIZE 20

using column_t = vector<double>;
using field_t  = vector<column_t>;

class board {
   private:
    vector<field_t> fields;
    int active_field = 0;

   public:
    const int Size = SIZE;
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

// following functions are of utility.

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

bool is_corner(int i, int j) {
    return i == 1 && j == 1 || i == 1 && j == SIZE || i == SIZE && j == 1 ||
           i == SIZE && j == SIZE;
}

bool is_on_edge(int i, int j) {
    return i == 1 || i == SIZE || j == 1 || j == SIZE;
}

int count_off_diagonal_walls(int i, int j) {
    return is_corner(i, j) ? 2 : is_on_edge(i, j) ? 1 : 0;
}

int count_diagonal_walls(int i, int j) {
    return is_corner(i, j) ? 3 : is_on_edge(i, j) ? 2 : 0;
}

int count_walls(int i, int j) {
    return count_off_diagonal_walls(i, j) + count_diagonal_walls(i, j);
}

// following functions are the core of this program.

field_t next_map(SIZE + 2, column_t(SIZE + 2, 0));

field_t initialize() {
    field_t map(SIZE + 2, column_t(SIZE + 2, 0));
    map[10][10] = 338;
    return map;  // May NRVO work well!
}

void show(const field_t &map) {
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            colorabstf(map[i][j]);
        }
        puts("");
    }
    puts("");
}

void update(field_t &map) {
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            next_map[i][j] = map[i][j];
        }
    }
    constexpr double ratio          = 0.05;
    constexpr double diagonal_ratio = ratio / 1.41421356;
    constexpr pair<int, int> directions[] = {make_pair(0, -1), make_pair(0, 1),
                                             make_pair(-1, 0), make_pair(1, 0)};
    constexpr pair<int, int> diagonal_directions[] = {
        make_pair(-1, -1), make_pair(-1, 1), make_pair(1, -1), make_pair(1, 1)};

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            for (auto dir : directions)
                next_map[i + dir.first][j + dir.second] += map[i][j] * ratio;
            for (auto dir : diagonal_directions)
                next_map[i + dir.first][j + dir.second] +=
                    map[i][j] * diagonal_ratio;
            // cancel flows to wall-cells.
            next_map[i][j] -=
                map[i][j] * ratio * (4 - count_off_diagonal_walls(i, j));
            next_map[i][j] -=
                map[i][j] * diagonal_ratio * (4 - count_diagonal_walls(i, j));
        }
    }

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            map[i][j] = next_map[i][j];
        }
    }
}

int main() {
    field_t map = initialize();

    while (true) {
        show(map);
        update(map);
        msleep(50);
    }
    return 0;
}