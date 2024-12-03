#include <iostream>
#include <print>
#include <regex>

#include <chrono>
#include <string>

auto read_input()
{
    std::string str{""}, line;
    while (std::getline(std::cin, line) && !line.empty()) {
        str += line;
    }
    return str;
}

auto mult(std::string_view str)
{
    constexpr auto first_num_pos{4ul};
    int            a{}, b{};
    auto           comma_pos{str.find(',', first_num_pos)}, parenthesis_pos{str.find(')', comma_pos + 1)};
    (void)std::from_chars(str.data() + first_num_pos, str.data() + comma_pos, a);
    (void)std::from_chars(str.data() + comma_pos + 1, str.data() + (parenthesis_pos - comma_pos), b);
    return a * b;
}

auto part1(const std::string& input)
{

    const std::regex regex("mul\\(\\d+,\\d+\\)");
    std::smatch      match;
    auto             start{input.cbegin()}, end{input.cend()};

    auto sum{0};
    while (std::regex_search(start, end, match, regex)) {
        sum += mult(match[0].str());
        start = match.suffix().first;
    }

    return sum;
}

auto part2(const std::string& input)
{

    const std::regex regex("mul\\(\\d+,\\d+\\)|do\\(\\)|don\\'t\\(\\)");
    std::smatch      match;
    auto             start{input.cbegin()}, end{input.cend()};

    auto sum{0};
    auto todo{true};
    while (std::regex_search(start, end, match, regex)) {
        if (match[0].str() == "do()") {
            todo = true;
        } else if (match[0].str() == "don't()") {
            todo = false;
        }

        if (todo) {
            sum += mult(match[0].str());
        }
        start = match.suffix().first;
    }

    return sum;
}

int main()
{
    const std::string input{read_input()};

    auto begin{std::chrono::steady_clock::now()};

    auto safe_reports{part1(input)};

    auto end{std::chrono::steady_clock::now()};
    std::println("Mult res: {} | time = {}us",
                 safe_reports,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto safe_reports_dampener{part2(input)};

    end = std::chrono::steady_clock::now();
    std::println("Mult res with do's: {} | time = {}us",
                 safe_reports_dampener,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Mult res: 173517243 | time = 264us
// Mult res with do's: 100450138 | time = 294us
