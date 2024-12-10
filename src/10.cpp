#include <fstream>
#include <print>
#include <vector>

#include <chrono>

using map_t = std::vector<std::vector<int>>;
constexpr auto v_size{53};
constexpr auto h_size{53};

auto read_input()
{
    std::ifstream input{"input/10.txt"};
    std::string   line;
    map_t         map{};

    while (std::getline(input, line) && !line.empty()) {
        std::vector<int> row;
        for (char ch : line) {
            row.push_back(ch - '0');
        }
        map.emplace_back(std::move(row));
    }
    return map;
}

struct Pos
{
    int  i, j;
    bool operator==(const Pos& rhs) const { return (j == rhs.j) && (i == rhs.i); }
    bool operator!=(const Pos& rhs) const { return !(*this == rhs); }
};

auto check_bounds(const Pos& pos)
{
    return (pos.i >= 0) && (pos.i < v_size) && (pos.j >= 0) && (pos.j < h_size);
}

auto find_zeros(const map_t& map)
{
    std::vector<Pos> zeroes{};
    for (auto i{0}; i < v_size; ++i) {
        for (auto j{0}; j < h_size; ++j) {
            if (map[i][j] == 0) {
                zeroes.emplace_back(i, j);
            }
        }
    }
    return zeroes;
}

auto solve(int i, int j, int prev_val, std::vector<Pos>& already_visited, const map_t& map)
{
    Pos pos{i, j};
    if (!check_bounds(pos)) {
        return 0;
    }

    auto val{map[i][j]};

    if (val != prev_val + 1) {
        return 0;
    }

    if (val == 9 && (std::find(already_visited.begin(), already_visited.end(), pos) == already_visited.end())) {
        already_visited.emplace_back(i, j);
        return 1;
    }

    return solve(i, j + 1, val, already_visited, map) + solve(i + 1, j, val, already_visited, map) +
           solve(i, j - 1, val, already_visited, map) + solve(i - 1, j, val, already_visited, map);
}

auto part1(const map_t& map)
{
    auto zeroes{find_zeros(map)};

    auto trailheads_score{0};
    for (const auto& zero : zeroes) {
        std::vector<Pos> already_visited{};
        trailheads_score += solve(zero.i, zero.j, -1, already_visited, map);
    }

    return trailheads_score;
}

auto solve(int i, int j, int prev_val, const map_t& map)
{
    Pos pos{i, j};
    if (!check_bounds(pos)) {
        return 0;
    }

    auto val{map[i][j]};

    if (val != prev_val + 1) {
        return 0;
    }

    if (val == 9) {
        return 1;
    }

    return solve(i + 1, j, val, map) + solve(i, j + 1, val, map) + solve(i, j - 1, val, map) +
           solve(i - 1, j, val, map);
}

auto part2(const map_t& map)
{
    auto zeroes{find_zeros(map)};

    auto trailheads_score{0};
    for (const auto& zero : zeroes) {
        trailheads_score += solve(zero.i, zero.j, -1, map);
    }

    return trailheads_score;
}

int main()
{
    auto map{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    auto p1{part1(map)};
    auto end{std::chrono::steady_clock::now()};

    std::println("Unique trailheads: {} | time = {}us",
                 p1,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p2{part2(map)};
    end = std::chrono::steady_clock::now();

    std::println("All trailheads: {} | time = {}us",
                 p2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Unique trailheads: 644 | time = 100us
// All trailheads: 1366 | time = 77us
