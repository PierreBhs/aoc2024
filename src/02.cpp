#include <iostream>
#include <iterator>
#include <print>
#include <sstream>
#include <vector>

#include <chrono>

void read_input(std::vector<std::vector<int>>& reports)
{
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::istringstream iss{line};
        reports.emplace_back(std::istream_iterator<int>{iss}, std::istream_iterator<int>{});
    }
}

auto part1(const std::vector<std::vector<int>>& reports)
{
    auto           num_safe_reports{0};
    constexpr auto max_diff{3};

    for (const auto& report : reports) {
        auto only_decrease{true}, only_increase{true}, adj{true};
        for (auto i{1ul}; i < report.size(); i++) {

            int diff{report[i] - report[i - 1]};
            if (std::abs(diff) > max_diff || diff == 0) {
                adj = false;
                break;
            }

            if (diff > 0) {
                only_decrease = false;
            } else {
                only_increase = false;
            }
        }

        num_safe_reports += (only_decrease ^ only_increase) && adj;
    }
    return num_safe_reports;
}

auto check_report_safe_dampener(const std::vector<int>& report)
{
    constexpr auto max_diff{3};

    for (size_t j = 0; j < report.size(); ++j) {
        std::vector<int> modified_report = report;  // :(
        modified_report.erase(modified_report.begin() + j);

        auto only_decrease{true}, only_increase{true}, adj{true};
        for (auto i{1ul}; i < modified_report.size(); i++) {

            auto diff{modified_report[i] - modified_report[i - 1]};
            if (std::abs(diff) > max_diff || diff == 0) {
                adj = false;
                break;
            }

            if (diff > 0) {
                only_decrease = false;
            } else {
                only_increase = false;
            }
        }

        if ((only_decrease ^ only_increase) && adj) {
            return 1;
        }
    }

    return 0;
}

auto part2(const std::vector<std::vector<int>>& reports)
{
    auto num_safe_reports{0};

    for (const auto& report : reports) {
        num_safe_reports += check_report_safe_dampener(report);
    }
    return num_safe_reports;
}

int main()
{
    std::vector<std::vector<int>> reports;
    read_input(reports);

    auto begin{std::chrono::steady_clock::now()};

    auto safe_reports{part1(reports)};

    auto end{std::chrono::steady_clock::now()};
    std::println("Safe reports: {} | time = {}us",
                 safe_reports,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());

    begin = std::chrono::steady_clock::now();

    auto safe_reports_dampener{part2(reports)};

    end = std::chrono::steady_clock::now();
    std::println("Safe report with dampener: {} | time = {}us",
                 safe_reports_dampener,
                 std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
}

// Safe reports: 213 | time = 12us
// Safe report with dampener: 285 | time = 110us
