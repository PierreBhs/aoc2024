#include <chrono>
#include <fstream>
#include <iterator>
#include <print>
#include <string>
#include <vector>

using input_t = std::vector<std::pair<std::size_t, std::vector<std::size_t>>>;

auto read_input()
{
    std::ifstream input_file{"input/07.txt"};
    std::string   line;
    input_t       input;

    while (std::getline(input_file, line) && !line.empty()) {
        auto delimiter_pos = line.find(':');

        auto key_str = line.substr(0, delimiter_pos);
        auto values_str = line.substr(delimiter_pos + 1);

        std::istringstream iss{values_str};
        input.emplace_back(
            std::stoul(key_str.data()),
            std::vector<std::size_t>{std::istream_iterator<std::size_t>{iss}, std::istream_iterator<std::size_t>{}});
    }

    return input;
}

auto part1(const input_t& equations)
{
    auto total_sum{0ul};

    for (const auto& [target, equation] : equations) {
        auto num_operators{equation.size() - 1};
        auto total_combinations{1ul << num_operators};  // 2^(N-1)

        for (auto mask{0ul}; mask < total_combinations; ++mask) {
            auto current_result{equation[0]};

            for (auto i{0ul}; i < num_operators; ++i) {
                if (mask & (1 << i)) {
                    current_result *= equation[i + 1];
                } else {
                    current_result += equation[i + 1];
                }
            }

            if (current_result == target) {
                total_sum += target;
                break;
            }
        }
    }

    return total_sum;
}

auto part1_dp(const input_t& equations)
{
    auto sum{0ul};

    for (const auto& [target, numbers] : equations) {
        std::vector<size_t> dp{numbers[0]};

        for (size_t i = 1; i < numbers.size(); ++i) {
            std::vector<size_t> dp_next{};
            auto                num{numbers[i]};

            for (const auto& val : dp) {
                auto add{val + num}, mult{val * num};
                if (add <= target) {
                    dp_next.push_back(add);
                }
                if (mult <= target) {
                    dp_next.push_back(mult);
                }
            }

            dp = std::move(dp_next);
        }

        if (std::find(dp.crbegin(), dp.crend(), target) != dp.crend()) {
            sum += target;
        }
    }

    return sum;
}

auto part2(const input_t& equations)
{
    auto sum{0ul};

    for (const auto& [target, numbers] : equations) {
        std::vector<size_t> dp{numbers[0]};

        for (size_t i = 1; i < numbers.size(); ++i) {
            std::vector<size_t> dp_next{};
            dp_next.reserve(2500);
            auto num{numbers[i]};

            for (const auto& val : dp) {
                auto mult{val * num}, concat{std::stoul(std::to_string(val) + std::to_string(num))};

                dp_next.push_back(val + num);  // almost never bigger

                if (mult <= target) {
                    dp_next.push_back(mult);
                }
                if (concat <= target) {
                    dp_next.push_back(concat);
                }
            }

            dp = std::move(dp_next);
        }

        if (std::find(dp.crbegin(), dp.crend(), target) != dp.crend()) {
            sum += target;
        }
    }

    return sum;
}

int main()
{
    auto equations{read_input()};

    auto begin{std::chrono::steady_clock::now()};
    auto p1{part1(equations)};
    auto end{std::chrono::steady_clock::now()};

    std::println(
        "sum: {} | time = {}us", p1, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p1_dp{part1_dp(equations)};
    end = std::chrono::steady_clock::now();

    std::println(
        "sum dp: {} | time = {}us", p1_dp, std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();
    auto p2{part2(equations)};
    end = std::chrono::steady_clock::now();

    std::println("sum dp concat: {} | time = {}us",
                 p2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// sum: 2299996598890 | time = 2583us
// sum dp: 2299996598890 | time = 565us
// sum dp concat: 362646859298554 | time = 80815us
