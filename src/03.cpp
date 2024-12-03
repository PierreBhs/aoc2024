#include <cstdint>
#include <fstream>
#include <print>
#include <regex>

#include <chrono>
#include <string>

auto read_input()
{
    std::ifstream input_file{"input/03.txt"};
    std::string   str{""}, line;
    while (std::getline(input_file, line) && !line.empty()) {
        str += line;
    }
    return str;
}

auto mult(std::string_view str)
{
    int  a{}, b{};
    auto comma_pos{str.find(',')};
    std::from_chars(str.data(), str.data() + comma_pos, a);
    std::from_chars(str.data() + ++comma_pos, str.data() + str.size(), b);
    return a * b;
}

auto part1(const std::string& input)
{

    const std::regex regex("mul\\(\\d+,\\d+\\)");
    std::smatch      match;
    auto             start{input.cbegin()};

    auto sum{0};
    while (std::regex_search(start, input.cend(), match, regex)) {
        sum += mult((match[0].str().c_str() + 4));
        start = match.suffix().first;
    }

    return sum;
}

auto check_valid(char c)
{
    return std::isdigit(c) || c == ',';
}

auto part1_no_regex(const std::string& input)
{
    auto sum{0};
    auto i{0ul};

    while ((i = input.find("mul(", i)) != std::string::npos) {
        i += 4;  // skip mul(

        std::string substr{""};
        bool        do_mult = true;
        while (input[i] != ')') {
            if (check_valid(input[i])) {
                substr += input[i];
                i++;
            } else {
                do_mult = false;
                break;
            }
        }

        if (do_mult && std::count(substr.begin(), substr.end(), ',') == 1) {
            sum += mult(substr);
        }

        i++;
    }
    return sum;
}

auto part2(const std::string& input)
{

    const std::regex regex("mul\\(\\d+,\\d+\\)|do\\(\\)|don\\'t\\(\\)");
    std::smatch      match;
    auto             start{input.cbegin()};

    auto sum{0};
    auto todo{true};
    while (std::regex_search(start, input.cend(), match, regex)) {
        if (match[0].str() == "do()") {
            todo = true;
        } else if (match[0].str() == "don't()") {
            todo = false;
        } else if (todo) {
            sum += mult(match[0].str().c_str() + 4);
        }

        start = match.suffix().first;
    }

    return sum;
}

auto part2_no_regex(const std::string& input)
{
    auto sum{0};
    for (auto i{0ul}; i < input.size(); i++) {
        if ((i = input.find("mul(", i)) == std::string::npos) {
            break;
        }

        auto do_pos{input.rfind("do()", i)};
        auto dont_pos{input.rfind("don't()", i)};

        auto do_dist{static_cast<std::int32_t>(i - do_pos)}, dont_dist{static_cast<std::int32_t>(i - dont_pos)};
        auto do_dist_min = (std::min(do_dist, dont_dist) == do_dist) ? true : false;

        i += 4;

        if (!do_dist_min) {
            // skip to next do() directly if we're in a don't
            i = input.find("do()", i) - 1;
        }

        std::string substr{""};
        bool        do_mult{true};
        while (input[i] != ')') {
            if (check_valid(input[i])) {
                substr += input[i];
                i++;
            } else {
                do_mult = false;
                break;
            }
        }

        if (do_mult && std::count(substr.begin(), substr.end(), ',') == 1) {
            sum += mult(substr);
        }
    }

    return sum;
}

int main()
{
    const std::string input{read_input()};

    auto begin{std::chrono::steady_clock::now()};

    auto mult_regex{part1(input)};

    auto end{std::chrono::steady_clock::now()};
    std::println("Regex - Mult res: {} | time = {}us",
                 mult_regex,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto mult{part1_no_regex(input)};

    end = std::chrono::steady_clock::now();
    std::println(
        "Mult res: {} | time = {}us", mult, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto do_mult_regex{part2(input)};

    end = std::chrono::steady_clock::now();
    std::println("Regex - Mult res with do's: {} | time = {}us",
                 do_mult_regex,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto do_mult{part2_no_regex(input)};

    end = std::chrono::steady_clock::now();
    std::println("Mult res with do's: {} | time = {}us",
                 do_mult,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Regex - Mult res: 173517243 | time = 230us
// Mult res: 173517243 | time = 16us
// Regex - Mult res with do's: 100450138 | time = 305us
// Mult res with do's: 100450138 | time = 125us
