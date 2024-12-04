#include <chrono>
#include <fstream>
#include <print>
#include <string>

/*
 * With bound checking per way
*/

constexpr std::string_view pattern{"XMAS"};

constexpr auto build_vertical_string = [](int i, int j, const std::vector<std::string>& grid, bool up) -> std::string {
    return std::string{grid[i][j], grid[i + (up ? -1 : 1)][j], grid[i + (up ? -2 : 2)][j], grid[i + (up ? -3 : 3)][j]};
};

constexpr auto build_diagonal_string =
    [](int row, int col, const std::vector<std::string>& grid, int delta_row, int delta_col) -> std::string {
    return std::string{grid[row][col],
                       grid[row + delta_row][col + delta_col],
                       grid[row + 2 * delta_row][col + 2 * delta_col],
                       grid[row + 3 * delta_row][col + 3 * delta_col]};
};

auto check_pattern(std::size_t i, std::size_t j, const std::vector<std::string>& input)
{
    const auto v_size{input.size()}, h_size{input[i].size()};

    auto count{0};
    if (j >= 3) {
        count += (std::string{input[i][j], input[i][j - 1], input[i][j - 2], input[i][j - 3]} == pattern);
    }
    if (j + 3 < h_size) {
        count += (input[i].substr(j, 4) == pattern);
    }
    if (i >= 3) {
        count += (build_vertical_string(i, j, input, true) == pattern);
    }
    if (i + 3 < v_size) {
        count += (build_vertical_string(i, j, input, false) == pattern);
    }

    // Diagonals
    auto is_valid_position = [&](int row, int col, int delta_row, int delta_col) -> bool {
        return (row + 3 * delta_row) >= 0 && (row + 3 * delta_row) < static_cast<int>(v_size) &&
               (col + 3 * delta_col) >= 0 && (col + 3 * delta_col) < static_cast<int>(h_size);
    };

    if (is_valid_position(i, j, 1, 1)) {
        count += (build_diagonal_string(i, j, input, 1, 1) == pattern);
    }
    if (is_valid_position(i, j, 1, -1)) {
        count += (build_diagonal_string(i, j, input, 1, -1) == pattern);
    }
    if (is_valid_position(i, j, -1, 1)) {
        count += (build_diagonal_string(i, j, input, -1, 1) == pattern);
    }
    if (is_valid_position(i, j, -1, -1)) {
        count += (build_diagonal_string(i, j, input, -1, -1) == pattern);
    }

    return count;
}

auto part1(const std::vector<std::string>& input)
{
    auto count{0};
    for (auto i{0ul}; i < input.size(); i++) {
        for (auto j{0ul}; j < input[i].size(); j++) {
            count += check_pattern(i, j, input);
        }
    }
    return count;
}

// Part1 V2

struct Direction
{
    int delta_row;
    int delta_col;
};

auto check_pattern_in_direction(int i, int j, const std::vector<std::string>& input, const Direction& dir)
{
    const int     v_size = input.size();
    const int     h_size = input[0].size();
    constexpr int pattern_length = pattern.size();

    for (auto k{0}; k < pattern_length; ++k) {
        auto ni{i + dir.delta_row * k};
        auto nj{j + dir.delta_col * k};
        if (ni < 0 || ni >= v_size || nj < 0 || nj >= h_size) {
            return 0;
        }
        if (input[ni][nj] != pattern[k]) {
            return 0;
        }
    }
    return 1;
}

auto part1_v2(const std::vector<std::string>& input)
{
    auto       count{0};
    const auto v_size{static_cast<int>(input.size())};
    const auto h_size{static_cast<int>(input[0].size())};

    constexpr std::array<Direction, 8> directions = {
        Direction{0, 1},    // Right
        Direction{0, -1},   // Left
        Direction{1, 0},    // Down
        Direction{-1, 0},   // Up
        Direction{1, 1},    // Down-Right
        Direction{1, -1},   // Down-Left
        Direction{-1, 1},   // Up-Right
        Direction{-1, -1},  // Up-Left
    };

    for (auto i{0}; i < v_size; ++i) {
        for (auto j{0}; j < h_size; ++j) {
            for (const auto& dir : directions) {
                count += check_pattern_in_direction(i, j, input, dir);
            }
        }
    }
    return count;
}

/*
 * Part 2
*/

auto check_x_mas_at_position(int i, int j, const std::vector<std::string>& input)
{
    const int             v_size{static_cast<int>(input.size())};
    const int             h_size{static_cast<int>(input[0].size())};
    constexpr std::size_t pattern2_length{3ul};

    std::array<std::pair<int, int>, 3> diag1_positions{{{i - 1, j - 1}, {i, j}, {i + 1, j + 1}}};
    std::array<std::pair<int, int>, 3> diag2_positions{{{i - 1, j + 1}, {i, j}, {i + 1, j - 1}}};

    auto get_diagonal_chars = [&](const std::array<std::pair<int, int>, 3>& positions) -> std::string {
        char chars[3];
        for (auto k{0ul}; k < positions.size(); ++k) {
            auto ni{positions[k].first};
            auto nj{positions[k].second};
            if (ni < 0 || ni >= v_size || nj < 0 || nj >= h_size) {
                return {};  // Out of bounds
            }
            chars[k] = input[ni][nj];
        }
        return std::string{chars, 3};
    };

    auto diag1_chars{get_diagonal_chars(diag1_positions)};
    auto diag2_chars{get_diagonal_chars(diag2_positions)};

    constexpr auto is_mas = [](const std::string& diag) -> bool {
        if (diag.size() != pattern2_length) {
            return false;
        }
        std::array<char, pattern2_length> diag_chars{diag[0], diag[1], diag[2]};
        std::array<char, pattern2_length> pattern_chars{'M', 'A', 'S'};

        // diag_chars is MAS or SAM
        return (diag_chars == pattern_chars) ||
               (std::array<char, pattern2_length>{diag_chars[2], diag_chars[1], diag_chars[0]} == pattern_chars);
    };

    return is_mas(diag1_chars) && is_mas(diag2_chars);
}

auto part2(const std::vector<std::string>& input) -> int
{

    auto count{0};
    for (auto i{0}; i < static_cast<int>(input.size()); ++i) {
        for (auto j{0}; j < static_cast<int>(input[0].size()); ++j) {
            count += check_x_mas_at_position(i, j, input);
        }
    }
    return count;
}

auto read_input()
{
    std::ifstream            input_file{"input/04.txt"};
    std::string              line;
    std::vector<std::string> input{};
    while (std::getline(input_file, line) && !line.empty()) {
        input.emplace_back(line);
    }
    return input;
}

int main()
{
    auto input{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    std::println(
        "xmas count 1: {} | time = {}us",
        part1(input),
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count());

    begin = std::chrono::steady_clock::now();
    std::println(
        "xmas count 2: {} | time = {}us",
        part1_v2(input),
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count());

    begin = std::chrono::steady_clock::now();
    std::println(
        "mas count 2: {} | time = {}us",
        part2(input),
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count());
}

// xmas count 1: 2571 | time = 397us
// xmas count 2: 2571 | time = 400us
// mas count 2:  1992 | time = 266us
