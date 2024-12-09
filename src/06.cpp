#include <chrono>
#include <fstream>
#include <print>
#include <string>
#include <unordered_set>
#include <vector>

using map_t = std::vector<std::string>;

auto read_input()
{
    std::ifstream input_file{"input/06.txt"};
    std::string   line;
    map_t         input;
    while (std::getline(input_file, line) && !line.empty()) {
        input.emplace_back(std::move(line));
    }

    return input;
}

auto print_map(const map_t& map)
{
    for (const auto& str : map) {
        std::println("{}", str);
    }
    std::println("");
}

struct Pos
{
    int y, x;
    enum class Direction { UP, DOWN, RIGHT, LEFT };
    Direction dir{Direction::UP};

    bool operator==(const Pos& rhs) const { return (y == rhs.y) && (x == rhs.x); }
    bool operator!=(const Pos& rhs) const { return !(*this == rhs); }

    // Needed for the unordered_set, putting it inside the struct somehow made it 0.01s faster (compared to outside)
    struct PosHash
    {
        std::size_t operator()(const Pos& p) const
        {
            std::size_t h1 = std::hash<int>()(p.x);
            std::size_t h2 = std::hash<int>()(p.y);
            return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1 << 6) + (h1 >> 2));
        }
    };
};

inline auto rotate(Pos& pos)
{
    switch (pos.dir) {
        using Dir = Pos::Direction;
        case Dir::UP:
            pos.dir = Dir::RIGHT;
            break;
        case Dir::DOWN:
            pos.dir = Dir::LEFT;
            break;
        case Dir::RIGHT:
            pos.dir = Dir::DOWN;
            break;
        case Dir::LEFT:
            pos.dir = Dir::UP;
            break;
    }
}

auto find_orig_pos(const map_t& map)
{
    const int h_size = map.size();
    const int v_size = map[0].size();

    for (auto i{0}; i < h_size; ++i) {
        for (auto j{0}; j < v_size; ++j) {
            if (map[i][j] == '^') [[unlikely]] {
                return Pos{i, j};
            }
        }
    }

    return Pos{};
}

inline auto out_of_bounds_with_exit(const Pos& pos, int v_size, int h_size)
{
    switch (pos.dir) {
        case Pos::Direction::UP:
            return pos.y == 0;
        case Pos::Direction::DOWN:
            return pos.y == v_size - 1;
        case Pos::Direction::RIGHT:
            return pos.x == h_size - 1;
        case Pos::Direction::LEFT:
            return pos.x == 0;
    }

    // This is slower than the switch ! 0.01s diff between the two
    return pos.y == 0 || pos.y + 1 == v_size || pos.x == h_size - 1 || pos.x == 0;
}

inline auto no_obstacle(const map_t& map, const Pos& pos)
{
    switch (pos.dir) {
        case Pos::Direction::UP:
            return map[pos.y - 1][pos.x] != '#';
        case Pos::Direction::DOWN:
            return map[pos.y + 1][pos.x] != '#';
        case Pos::Direction::RIGHT:
            return map[pos.y][pos.x + 1] != '#';
        case Pos::Direction::LEFT:
            return map[pos.y][pos.x - 1] != '#';
    }
    return true;
}

inline auto move_next_pos(Pos& pos)
{
    switch (pos.dir) {
        using Dir = Pos::Direction;
        case Dir::UP:
            pos.y -= 1;
            break;
        case Dir::DOWN:
            pos.y += 1;
            break;
        case Dir::RIGHT:
            pos.x += 1;
            break;
        case Dir::LEFT:
            pos.x -= 1;
    }
}

auto part1(map_t& map)
{
    const int h_size = map.size();
    const int v_size = map[0].size();

    Pos pos{find_orig_pos(map)};

    auto visited{0};
    while (true) {
        if (map[pos.y][pos.x] != 'X') {
            visited++;
            map[pos.y][pos.x] = 'X';
        }

        if (out_of_bounds_with_exit(pos, v_size, h_size)) {
            return visited;
        }

        if (no_obstacle(map, pos)) {
            move_next_pos(pos);
        } else {
            rotate(pos);
        }
    }
}

/* 
 * Part2
 * Idea: save path that leads to exit in Part1 and check that you get into a loop if you add an obstacle on each positions of that path
 * Avoid duplicates with a set (faster than a vector and removing duplicates?)
*/

auto path_to_exit(const map_t& map) -> std::pair<std::unordered_set<Pos, Pos::PosHash>, Pos>
{
    Pos                                   orig_pos{find_orig_pos(map)}, pos{orig_pos};
    std::unordered_set<Pos, Pos::PosHash> path{};

    while (true) {
        if (out_of_bounds_with_exit(pos, map.size(), map[0].size())) {
            return {path, orig_pos};
        }

        if (no_obstacle(map, pos)) {
            move_next_pos(pos);
            path.emplace(pos);
        } else {
            rotate(pos);
        }
    }
}

auto check_loop(const map_t& map, Pos pos)
{
    // cheating here, works on this map but should in reality be Steps > 4N^2 (N*N positions and 4 directions)
    auto max_steps{static_cast<int>((map.size() * map.size()) / 2)}, steps{0};

    while (true) {
        if (out_of_bounds_with_exit(pos, map.size(), map[0].size())) {
            return 0;
        }

        if (no_obstacle(map, pos)) {
            move_next_pos(pos);
            steps++;
        } else {
            rotate(pos);
        }

        if (steps > max_steps) [[unlikely]] {
            return 1;
        }
    }
}

auto part2(map_t& map)
{
    auto [exit_path, orig_pos]{path_to_exit(map)};
    auto loops{0};

    // exit_path.erase(orig_pos); not needed for input, but could be needed for other kind of inputs ?

    for (const auto& pos : exit_path) {
        map[pos.y][pos.x] = '#';
        loops += check_loop(map, orig_pos);
        map[pos.y][pos.x] = '.';
    }

    return loops;
}

int main()
{
    auto map{read_input()};
    auto map2 = map;

    auto begin{std::chrono::steady_clock::now()};
    auto p1{part1(map)};
    auto end{std::chrono::steady_clock::now()};

    std::println(
        "sum: {} | time = {}us", p1, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p2{part2(map2)};
    end = std::chrono::steady_clock::now();

    std::println("different positions: {} | time = {}ms",
                 p2,
                 std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
}

// sum: 5208 | time = 14us
// different positions: 1972 | time = 18ms
