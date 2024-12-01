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

auto part_one(std::vector<int>& left, std::vector<int>& right)
{
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::size_t distance{0ul};
    for (size_t i = 0; i < left.size(); ++i) {
        distance += std::abs(left[i] - right[i]);
    }

    return distance;
}

auto part_two(const std::vector<int>& left, const std::vector<int>& right)
{
    std::unordered_map<int, int64_t> right_counts;
    for (int num : right) {
        ++right_counts[num];
    }

    std::size_t similarity_score{0ul};
    for (int num : left) {
        similarity_score += num * right_counts[num];
    }

    return similarity_score;
}

int main()
{
    std::vector<int> left, right;
    read_input(left, right);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    auto dist{part_one(left, right)};

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::println("Total distance: {} | time = {}us",
                 dist,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto similarity{part_two(left, right)};

    end = std::chrono::steady_clock::now();
    std::println("Similarity: {} | time = {}us",
                 similarity,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Total distance: 1388114 | time = 70us
// Similarity: 23529853 | time = 95us
