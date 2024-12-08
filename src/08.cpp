#include <chrono>
#include <fstream>
#include <print>
#include <string>
#include <unordered_set>
#include <vector>

using map_t = std::vector<std::string>;

auto read_input()
{
    std::ifstream input_file{"input/08.txt"};
    std::string   line;
    map_t         input;
    while (std::getline(input_file, line) && !line.empty()) {
        input.emplace_back(line);
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

    bool operator==(const Pos& rhs) const { return (y == rhs.y) && (x == rhs.x); }
    bool operator!=(const Pos& rhs) const { return !(*this == rhs); }

    struct hash
    {
        std::size_t operator()(const Pos& p) const
        {
            std::size_t h1 = std::hash<int>()(p.x);
            std::size_t h2 = std::hash<int>()(p.y);
            return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1 << 6) + (h1 >> 2));
        }
    };
};

auto all_positions(std::size_t i, char c, const map_t& map)
{
    std::vector<Pos> all_pos{};
    for (; i < map.size(); ++i) {
        for (auto j{0ul}; j < map[i].size(); ++j) {
            if (map[i][j] == c) {
                all_pos.emplace_back(i, j);
            }
        }
    }

    return all_pos;
}

auto all_pairs_positions(std::vector<Pos> positions)
{
    std::vector<std::pair<Pos, Pos>> positions_pairs{};

    for (auto i{0ul}; i < positions.size(); ++i) {
        for (auto j{i + 1}; j < positions.size(); ++j) {
            positions_pairs.emplace_back(positions[i], positions[j]);
        }
    }

    return positions_pairs;
}

auto antenna_pairs(const map_t& map)
{
    std::vector<std::vector<std::pair<Pos, Pos>>> pairs{};
    std::string                                   already{"."};

    for (auto i{0ul}; i < map.size(); ++i) {
        for (auto j{0ul}; j < map[i].size(); ++j) {
            if (!already.contains(map[i][j])) {
                already += map[i][j];
                pairs.emplace_back(all_pairs_positions(all_positions(i, map[i][j], map)));
            }
        }
    }
    return pairs;
}

auto calc_vector(const std::pair<Pos, Pos>& positions) -> Pos
{
    return {positions.second.y - positions.first.y, positions.second.x - positions.first.x};
}

auto add_vector(const Pos& a, const Pos& b) -> Pos
{
    return {a.y + b.y, a.x + b.x};
}

auto sub_vector(const Pos& a, const Pos& b) -> Pos
{
    return {a.y - b.y, a.x - b.x};
}
auto check_bound(const Pos& pos, const map_t& map)
{
    const int ysize = map.size();
    const int xsize = map.size();
    return (pos.y >= 0) && (pos.y < ysize) && (pos.x >= 0) && (pos.x < xsize);
}

auto part1(const map_t& map)
{
    auto all_pairs{antenna_pairs(map)};

    std::unordered_set<Pos, Pos::hash> antinodes{};
    for (const auto& pairs : all_pairs) {
        for (const auto& pair : pairs) {
            auto vec{calc_vector(pair)};
            auto pos1{sub_vector(pair.first, vec)};
            auto pos2{add_vector(pair.second, vec)};

            if (check_bound(pos1, map)) {
                antinodes.insert(pos1);
            }
            if (check_bound(pos2, map)) {
                antinodes.insert(pos2);
            }
        }
    }

    return antinodes.size();
}

auto part2(const map_t& map)
{
    auto all_pairs{antenna_pairs(map)};

    std::unordered_set<Pos, Pos::hash> antinodes{};
    for (const auto& pairs : all_pairs) {
        for (const auto& pair : pairs) {
            const auto vec{calc_vector(pair)};
            auto       pos1{pair.first};
            auto       pos2{pair.second};

            antinodes.insert(pos1);
            antinodes.insert(pos2);

            while (check_bound(sub_vector(pos1, vec), map)) {
                pos1 = sub_vector(pos1, vec);
                antinodes.insert(pos1);
            }
            while (check_bound(add_vector(pos2, vec), map)) {
                pos2 = add_vector(pos2, vec);
                antinodes.insert(pos2);
            }
        }
    }

    return antinodes.size();
}

int main()
{
    auto map{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    auto p1{part1(map)};
    auto end{std::chrono::steady_clock::now()};

    std::println(
        "sum: {} | time = {}us", p1, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p2{part2(map)};
    end = std::chrono::steady_clock::now();

    std::println("different positions: {} | time = {}us",
                 p2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// sum: 364 | time = 217us
// different positions: 1231 | time = 341us
