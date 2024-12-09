#include <chrono>
#include <cmath>
#include <fstream>
#include <print>
#include <string>

auto read_input()
{
    std::ifstream input_file{"input/09.txt"};
    std::string   line;
    while (std::getline(input_file, line) && !line.empty()) {
        return line;
    }

    return line;
}

auto part1(const std::string& disk_map)
{
    auto checksum{0ul};

    std::vector<std::string> defragmented{};

    auto j{0};
    auto num_dots{0};
    for (auto i{0ul}; i < disk_map.size(); ++i) {
        const std::string s{std::string{disk_map[i]}};
        if (i % 2 == 0) {
            char val[5]{};
            std::to_chars(val, val + 5, j++);
            for (auto n{0ul}; n < std::stoul(s); ++n) {
                defragmented.push_back(val);
            }
        } else {
            for (auto n{0ul}; n < std::stoul(s); n++) {
                defragmented.push_back(".");
                num_dots++;
            }
        }
    }

    while (num_dots--) {
        // Find first '.'
        auto pos{std::find(defragmented.cbegin(), defragmented.cend(), ".")};
        auto idx = pos - defragmented.cbegin();
        // Find last non '.'
        auto pos2{std::find_if(
            defragmented.crbegin(), defragmented.crend(), [](const std::string& in) { return in != "."; })};
        auto idx2 = std::distance(pos2, defragmented.crend()) - 1;

        std::swap(defragmented[idx], defragmented[idx2]);
    }

    while (defragmented[++num_dots] != ".") {
        checksum += std::stoul(defragmented[num_dots]) * num_dots;
    }

    return checksum;
}

auto part1_v2(const std::string& disk_map)
{

    auto checksum{0ul};

    std::vector<int> defragmented{};

    auto j{0}, num_vals{0};
    for (auto i{0ul}; i < disk_map.size(); ++i) {
        const std::string s{std::string{disk_map[i]}};
        if (!(i % 2)) {
            for (auto n{0ul}; n < std::stoul(s); ++n) {
                defragmented.push_back(j);
                num_vals++;
            }
            j++;
        } else {
            for (auto n{0ul}; n < std::stoul(s); ++n) {
                defragmented.push_back(-1);
            }
        }
    }

    auto last{defragmented.size() - 1};
    auto id{0ul}, pos{0ul};
    while (num_vals--) {

        if (defragmented[pos] != -1) {
            checksum += defragmented[pos] * id++;
        } else {
            while (defragmented[last] == -1) {
                last--;
            };
            checksum += defragmented[last--] * id++;
        }

        pos++;
    }

    return checksum;
}

auto part2(const std::string& disk_map)
{
    auto checksum{0ul};

    // {val, occurence}, -1 if gap
    std::vector<std::pair<int, int>> defragmented{};

    auto j{0};
    for (auto i{0ul}; i < disk_map.size(); ++i) {
        const std::string s{std::string{disk_map[i]}};
        if (!(i % 2)) {
            defragmented.emplace_back(j++, std::stoi(s));
        } else {
            if (auto val = std::stoul(s)) {
                defragmented.emplace_back(-1, val);
            }
        }
    }

    auto id{0};
    for (auto i{0ul}; i < defragmented.size(); ++i) {
        auto& curr{defragmented[i]};
        if (curr.first >= 0) {
            auto n{curr.second};
            while (n--) {
                checksum += curr.first * id++;
            }
            continue;
        }

        auto& gap{curr.second};
        if (curr.first != -2) {
            for (auto end{defragmented.size() - 1}; end >= i && gap > 0; --end) {
                auto& to_insert{defragmented[end]};
                if (to_insert.first >= 0 && to_insert.second <= gap) {
                    auto n{to_insert.second};
                    while (n--) {
                        checksum += to_insert.first * id++;
                    }

                    gap -= to_insert.second;
                    to_insert.first = -2;  // dont touch anymore
                }
            }
        }

        // Increase ID for gaps that couldn't be filled
        for (auto gaps{0}; gaps < gap; ++gaps, ++id)
            ;  // stupid formatting
    }

    return checksum;
}

int main()
{
    auto disk_map{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    auto p1{part1(disk_map)};
    auto end{std::chrono::steady_clock::now()};

    std::println(
        "checksum: {} | time = {}ms", p1, std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p1_2{part1_v2(disk_map)};
    end = std::chrono::steady_clock::now();

    std::println("checksumv2: {} | time = {}us",
                 p1_2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p2{part2(disk_map)};
    end = std::chrono::steady_clock::now();

    std::println("checksumv part2: {} | time = {}us",
                 p2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// checksum: 6283170117911 | time = 786ms
// checksumv2: 6283170117911 | time = 675us
// checksumv part2: 6307653242596 | time = 8580us
