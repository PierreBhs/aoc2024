#include <iostream>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include <chrono>

using stones_t = std::vector<std::string>;

auto part1_dumb_string(stones_t& stones, int blinks)
{
    while (blinks--) {
        stones_t vec{};
        for (const auto& stone : stones) {
            if (stone == "0") {
                vec.emplace_back("1");
            } else if (stone.size() % 2 == 0) {
                auto size{stone.size()};
                vec.emplace_back(std::to_string(std::stoul(stone.substr(0, size / 2))));
                vec.emplace_back(std::to_string(std::stoul(stone.substr(size / 2, size / 2))));
            } else {
                vec.emplace_back(std::to_string(std::stoul(stone) * 2024));
            }
        }
        stones = std::move(vec);
    }
}

auto get_num_digits(std::size_t n) -> std::size_t
{
    if (n > 0)
        return 1ul + get_num_digits(n / 10);
    return 0ul;
}

constexpr auto split_digits(std::size_t num, std::size_t digits) -> std::pair<std::size_t, std::size_t>
{
    auto divisor{1ul};
    for (auto i{0ul}; i < digits / 2; ++i) {
        divisor *= 10;
    }

    // Split the number into two parts
    auto first_half{num / divisor};
    auto second_half{num % divisor};

    return {first_half, second_half};
}

auto part1(std::vector<std::size_t>& stones, int blinks)
{
    stones.reserve(5000);
    while (blinks--) {
        auto size{stones.size()};
        for (auto i{0ul}; i < size; ++i) {
            auto& stone{stones[i]};
            auto  num_digits{get_num_digits(stone)};
            if (stone == 0ul) {
                stone = 1ul;
            } else if (num_digits % 2 == 0) {
                auto split{split_digits(stone, num_digits)};
                stone = split.first;
                stones.push_back(split.second);
            } else {
                stone *= 2024ul;
            }
        }
    }
}

// Somehow faster than the version above ? Fastest brute force I could find
// Faster than hash map version for small blinking values
auto part1_fastest(std::vector<std::size_t>& stones, int blinks)
{
    while (blinks--) {
        std::vector<std::size_t> vec{};
        vec.reserve(5000);
        for (const auto stone : stones) {
            auto num_digits{get_num_digits(stone)};
            if (stone == 0ul) {
                vec.push_back(1);
            } else if (num_digits % 2 == 0) {
                auto split{split_digits(stone, num_digits)};
                vec.push_back(split.first);
                vec.push_back(split.second);
            } else {
                vec.push_back(stone * 2024ul);
            }
        }
        stones = std::move(vec);
    }
}

auto part2(std::vector<std::size_t>& stones_vec, int blinks)
{
    std::unordered_map<std::size_t, std::uint64_t> stones;
    stones.reserve(stones_vec.size() * 2);
    for (const auto stone : stones_vec) {
        stones[stone]++;
    }

    while (blinks--) {
        std::unordered_map<std::size_t, std::uint64_t> new_stones;
        new_stones.reserve(stones.size() * 2);

        for (const auto& [k, v] : stones) {
            if (k == 0) {
                new_stones[1] += v;
            } else {
                auto digits{get_num_digits(k)};
                if (digits % 2 == 0) {
                    const auto [left_part, right_part]{split_digits(k, digits)};
                    new_stones[left_part] += v;
                    new_stones[right_part] += v;
                } else {
                    new_stones[k * 2024ul] += v;
                }
            }
        }

        stones = std::move(new_stones);
    }

    auto total{0ul};
    for (const auto& kv : stones) {
        total += kv.second;
    }

    return total;
}

int main()
{
    stones_t                 stones{"64599", "31", "674832", "2659361", "1", "0", "8867", "321"};
    std::vector<std::size_t> stones_int{64599ul, 31ul, 674832ul, 2659361ul, 1ul, 0ul, 8867ul, 321ul},
        stones_int2{stones_int}, stones_int_part2{stones_int};

    auto begin{std::chrono::steady_clock::now()};
    part1_dumb_string(stones, 25);
    auto end{std::chrono::steady_clock::now()};

    std::cout << stones.size()
              << " | time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << std::endl;

    begin = std::chrono::steady_clock::now();
    part1(stones_int, 25);
    end = std::chrono::steady_clock::now();

    std::cout << stones_int.size()
              << " | time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << std::endl;

    begin = std::chrono::steady_clock::now();
    part1_fastest(stones_int2, 25);
    end = std::chrono::steady_clock::now();

    std::cout << stones_int2.size()
              << " | time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << std::endl;

    begin = std::chrono::steady_clock::now();
    auto p2{part2(stones_int_part2, 75)};
    end = std::chrono::steady_clock::now();

    std::println("Optimized stone coutning: {} | time = {}us",
                 p2,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// 199986 | time = 13511
// 199986 | time = 1923
// 199986 | time = 1208
// Optimized stone coutning: 236804088748754 | time = 5680us
