#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void colorabstf(double percent) {
    percent = min(percent, 0.999999);
    const vector<int> color_format{37, 36, 35, 34, 33, 32, 31,
                                   47, 46, 45, 44, 43, 42, 41};
    printf("\x1b[%dm**\x1b[39m\x1b[49m",
           color_format[percent * color_format.size()]);
}

class board {
    using column_t = vector<double>;
    using field_t  = vector<column_t>;

   private:
    vector<field_t> fields;
    int active_field = 0;

   public:
    int Size;
    board(int size) {
        Size   = size;
        fields = vector<field_t>(2, field_t(Size + 2, column_t(Size + 2, 0)));
    }
    ~board() {}

    void flip() {
        active_field = 1 - active_field;
    }

    field_t &field() {
        return fields[active_field];
    }

    field_t &background_field() {
        return fields[1 - active_field];
    }

    void show() {
        field_t map = field();
        for (int i = 1; i <= Size; i++) {
            for (int j = 1; j <= Size; j++) {
                colorabstf(map[i][j]);
            }
            puts("");
        }
        puts("");
    }

    bool is_corner(int i, int j) {
        return i == 1 && j == 1 || i == 1 && j == Size || i == Size && j == 1 ||
               i == Size && j == Size;
    }

    bool is_on_edge(int i, int j) {
        return i == 1 || i == Size || j == 1 || j == Size;
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

    void update() {
        field_t &map = field();
        static field_t next_map(Size + 2, column_t(Size + 2, 0));
        for (int i = 1; i <= Size; i++) {
            for (int j = 1; j <= Size; j++) {
                next_map[i][j] = map[i][j];
            }
        }
        constexpr double ratio                = 0.05;
        constexpr double diagonal_ratio       = ratio / 1.41421356;
        constexpr pair<int, int> directions[] = {
            make_pair(0, -1), make_pair(0, 1), make_pair(-1, 0),
            make_pair(1, 0)};
        constexpr pair<int, int> diagonal_directions[] = {
            make_pair(-1, -1), make_pair(-1, 1), make_pair(1, -1),
            make_pair(1, 1)};

        for (int i = 1; i <= Size; i++) {
            for (int j = 1; j <= Size; j++) {
                for (auto dir : directions)
                    next_map[i + dir.first][j + dir.second] +=
                        map[i][j] * ratio;
                for (auto dir : diagonal_directions)
                    next_map[i + dir.first][j + dir.second] +=
                        map[i][j] * diagonal_ratio;
                // cancel flows to wall-cells.
                next_map[i][j] -=
                    map[i][j] * ratio * (4 - count_off_diagonal_walls(i, j));
                next_map[i][j] -= map[i][j] * diagonal_ratio *
                                  (4 - count_diagonal_walls(i, j));
            }
        }

        for (int i = 1; i <= Size; i++) {
            for (int j = 1; j <= Size; j++) {
                map[i][j] = next_map[i][j];
            }
        }
    }
};

// following functions are of utility.

void msleep(int millisecond) {
    usleep(millisecond * 1000);
}

// following functions are the core of this program.

board initialize(int size) {
    board b(size);
    b.field()[10][10] = 338;
    return b;  // May NRVO work well!
}

int main() {
    board b = initialize(20);
    while (true) {
        b.show();
        b.update();
        msleep(50);
    }
    return 0;
}