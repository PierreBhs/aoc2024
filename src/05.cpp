#include <algorithm>
#include <charconv>
#include <fstream>
#include <iterator>
#include <locale>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct comma_separator : std::ctype<char>
{
    comma_separator() : std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table()
    {
        static std::vector<std::ctype_base::mask> table{classic_table(), classic_table() + table_size};
        // Set comma as a space character
        table[','] = std::ctype_base::space;
        return &table[0];
    }
};

auto read_input() -> std::pair<std::unordered_map<int, std::vector<int>>, std::vector<std::vector<int>>>
{
    std::ifstream                             input_file{"input/05-1.txt"};
    std::string                               line;
    std::unordered_map<int, std::vector<int>> map{};
    while (std::getline(input_file, line) && !line.empty()) {
        auto delimiter_pos{line.find('|')};

        auto key{0}, value{0};
        auto key_str{line.substr(0, delimiter_pos)};
        auto value_str{line.substr(delimiter_pos + 1)};

        std::from_chars(key_str.data(), key_str.data() + key_str.size(), key);
        std::from_chars(value_str.data(), value_str.data() + value_str.size(), value);

        if (map.contains(key)) {
            map[key].push_back(value);
        } else {
            map[key] = {value};
        }
    }

    std::ifstream                 input_file2{"input/05-2.txt"};
    std::vector<std::vector<int>> vec{};
    while (std::getline(input_file2, line) && !line.empty()) {
        std::istringstream iss{line};
        iss.imbue(std::locale(iss.getloc(), new comma_separator()));
        vec.emplace_back(std::istream_iterator<int>{iss}, std::istream_iterator<int>{});
    }

    return {map, vec};
}

auto is_correctly_ordered(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<int>& update)
{
    for (auto i{1ul}; i < update.size(); i++) {
        if (std::ranges::find(rules.at(update[i]), update[i - 1]) != rules.at(update[i]).end()) {
            return false;
        }
    }
    return true;
}

auto part1(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<std::vector<int>>& updates)
{
    auto sum{0};
    for (const auto& vec : updates) {
        if (is_correctly_ordered(rules, vec)) {
            sum += vec[vec.size() / 2];
        }
    }
    return sum;
}

/* 
 * Part2
*/

auto reorder_update(const std::unordered_map<int, std::vector<int>>& rules, std::vector<int> update)
{
    std::ranges::sort(update, [&rules](int a, int b) {
        if (std::ranges::find(rules.at(a), b) != rules.at(a).end()) {
            return true;
        }
        if (std::ranges::find(rules.at(b), a) != rules.at(b).end()) {
            return false;
        }
        // numeric order otherwise
        return a < b;
    });
    return update;
}

auto part2(std::unordered_map<int, std::vector<int>>& rules, const std::vector<std::vector<int>>& updates)
{
    auto sum{0};
    for (const auto& vec : updates) {
        if (!is_correctly_ordered(rules, vec)) {
            auto corrected = reorder_update(rules, vec);
            sum += corrected[corrected.size() / 2];
        }
    }
    return sum;
}

int main()
{
    auto [page_rules, updates]{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    std::println(
        "sum: {} | time = {}us",
        part1(page_rules, updates),
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count());

    begin = std::chrono::steady_clock::now();
    std::println(
        "sum reordered: {} | time = {}us",
        part2(page_rules, updates),
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count());
}

// sum: 7198 | time = 41us
// sum reordered: 4230 | time = 193us
