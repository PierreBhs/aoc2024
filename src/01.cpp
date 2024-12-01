#include <iostream>
#include <print>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <chrono>

void read_input(std::vector<int>& left, std::vector<int>& right)
{
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::istringstream iss(line);
        int                l, r;
        if (iss >> l >> r) {
            left.push_back(l);
            right.push_back(r);
        }
    }
}

auto part1(std::vector<int>& left, std::vector<int>& right)
{
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    auto distance{0ul};
    for (size_t i = 0; i < left.size(); ++i) {
        distance += std::abs(left[i] - right[i]);
    }

    return distance;
}

auto part2(const std::vector<int>& left, const std::vector<int>& right)
{
    std::unordered_map<int, int64_t> right_counts;
    for (int num : right) {
        ++right_counts[num];
    }

    auto similarity{0ul};
    for (int num : left) {
        similarity += num * right_counts[num];
    }

    return similarity;
}

int main()
{
    std::vector<int> left, right;
    read_input(left, right);

    auto begin{std::chrono::steady_clock::now()};

    auto dist{part1(left, right)};

    auto end{std::chrono::steady_clock::now()};
    std::println("Total distance: {} | time = {}us",
                 dist,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto similarity{part2(left, right)};

    end = std::chrono::steady_clock::now();
    std::println("Similarity: {} | time = {}us",
                 similarity,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Total distance: 1388114 | time = 70us
// Similarity: 23529853 | time = 95us
