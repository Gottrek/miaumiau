#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <iomanip>

struct Item {
    int size, value, index;
    double ratio;
    
    Item(int s, int v, int i) : size(s), value(v), index(i), ratio((double)v/s) {}
};

struct Solution {
    std::vector<bool> selected;
    int totalValue, totalSize;
    
    Solution(int n) : selected(n, false), totalValue(0), totalSize(0) {}
    Solution() : selected(), totalValue(0), totalSize(0) {}
};

void generateKnapsackData(int n, int capacity, int max_size, int max_value, const char* filename) {
    std::ofstream out(filename);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> size_dist(1, max_size);
    std::uniform_int_distribution<int> value_dist(1, max_value);

    out << n << " " << capacity << "\n";
    for (int i = 0; i < n; ++i) {
        out << size_dist(gen) << " " << value_dist(gen) << "\n";
    }
}

bool loadFromFile(const std::string& filename, std::vector<Item>& items, int& capacity) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    int n;
    file >> n >> capacity;
    items.clear();
    
    for (int i = 0; i < n; i++) {
        int size, value;
        file >> size >> value;
        items.emplace_back(size, value, i);
    }
    return true;
}

Solution solveDynamicProgramming(const std::vector<Item>& items, int capacity) {
    int n = items.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i-1].size <= w) {
                dp[i][w] = std::max(dp[i-1][w], dp[i-1][w - items[i-1].size] + items[i-1].value);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    Solution solution(n);
    int w = capacity;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            solution.selected[i-1] = true;
            solution.totalValue += items[i-1].value;
            solution.totalSize += items[i-1].size;
            w -= items[i-1].size;
        }
    }
    return solution;
}

Solution solveGreedy(const std::vector<Item>& items, int capacity) {
    std::vector<Item> sortedItems = items;
    std::sort(sortedItems.begin(), sortedItems.end(), 
             [](const Item& a, const Item& b) { return a.ratio > b.ratio; });
    
    Solution solution(items.size());
    int currentWeight = 0;
    
    for (const auto& item : sortedItems) {
        if (currentWeight + item.size <= capacity) {
            solution.selected[item.index] = true;
            solution.totalValue += item.value;
            solution.totalSize += item.size;
            currentWeight += item.size;
        }
    }
    return solution;
}

Solution solveBruteForce(const std::vector<Item>& items, int capacity, int time_limit_ms = 60000) {
    int n = items.size();
    Solution best_solution(n);
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int X = 1; X < (1 << n); X++) {
        int W = 0, f = 0;
        for (int i = 0; i < n; i++) {
            if (X & (1 << i)) {
                W += items[i].size;
                f += items[i].value;
            }
        }
        
        if (W <= capacity && f > best_solution.totalValue) {
            best_solution.totalValue = f;
            best_solution.totalSize = W;
            for (int i = 0; i < n; i++) {
                best_solution.selected[i] = (X & (1 << i)) != 0;
            }
        }
        
        // Check time limit every 1000 iterations
        if (X % 1000 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            if (elapsed > time_limit_ms) {
                std::cout << "Brute force timeout after " << elapsed << "ms" << std::endl;
                break;
            }
        }
    }
    return best_solution;
}

double measureTime(std::function<Solution()> func, int runs = 3) {
    double total_time = 0;
    for (int i = 0; i < runs; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        Solution result = func();
        auto end = std::chrono::high_resolution_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    }
    return total_time / runs;
}

void generateTimeVsItemsData() {
    std::cout << "Generating t=f(n) data..." << std::endl;
    
    std::ofstream file("time_vs_items.csv");
    file << std::fixed << std::setprecision(6);
    file << "n,DP_time_ms,Greedy_time_ms,BruteForce_time_ms\n";
    
    const int capacity = 100;
    const int max_size = 20;
    const int max_value = 100;
    
    // Even-numbered increments from 2 to 200
    for (int n = 2; n <= 200; n += 2) {
        generateKnapsackData(n, capacity, max_size, max_value, "temp_test.txt");
        
        std::vector<Item> items;
        int cap;
        loadFromFile("temp_test.txt", items, cap);
        
        double dp_time = measureTime([&]() { return solveDynamicProgramming(items, capacity); });
        double greedy_time = measureTime([&]() { return solveGreedy(items, capacity); });
        double bf_time = measureTime([&]() { return solveBruteForce(items, capacity); }, 1); // Single run for brute force
        
        file << n << "," << dp_time << "," << greedy_time << ",";
        file << (bf_time < 60000 ? std::to_string(bf_time) : "TIMEOUT") << "\n";
        
        std::cout << "n=" << n << " completed" << std::endl;
    }
    
    file.close();
    std::cout << "Data saved to: time_vs_items.csv" << std::endl;
}

void generateTimeVsCapacityData() {
    std::cout << "Generating t=f(b) data..." << std::endl;
    
    std::ofstream file("time_vs_capacity.csv");
    file << std::fixed << std::setprecision(6);
    file << "capacity,DP_time_ms,Greedy_time_ms,BruteForce_time_ms\n";
    
    const int n = 20;
    const int max_size = 30;
    const int max_value = 150;
    
    // Even-numbered increments from 20 to 2000
    for (int capacity = 20; capacity <= 2000; capacity += 20) {
        generateKnapsackData(n, capacity, max_size, max_value, "temp_test.txt");
        
        std::vector<Item> items;
        int cap;
        loadFromFile("temp_test.txt", items, cap);
        
        double dp_time = measureTime([&]() { return solveDynamicProgramming(items, capacity); });
        double greedy_time = measureTime([&]() { return solveGreedy(items, capacity); });
        double bf_time = measureTime([&]() { return solveBruteForce(items, capacity); }, 1);
        
        file << capacity << "," << dp_time << "," << greedy_time << ",";
        file << (bf_time < 60000 ? std::to_string(bf_time) : "TIMEOUT") << "\n";
        
        std::cout << "capacity=" << capacity << " completed" << std::endl;
    }
    
    file.close();
    std::cout << "Data saved to: time_vs_capacity.csv" << std::endl;
}

void generateTimeVsNAndBData() {
    std::cout << "Generating t=f(n,b) data..." << std::endl;
    
    std::ofstream file("time_vs_n_and_b.csv");
    file << std::fixed << std::setprecision(6);
    file << "n,capacity,DP_time_ms,Greedy_time_ms,BruteForce_time_ms\n";
    
    const int max_size = 25;
    const int max_value = 150;
    
    // Even-numbered n values from 2 to 30
    std::vector<int> item_counts;
    for (int i = 2; i <= 30; i += 2) {
        item_counts.push_back(i);
    }
    
    // Even-numbered capacities from 20 to 200
    std::vector<int> capacities;
    for (int i = 20; i <= 200; i += 20) {
        capacities.push_back(i);
    }
    
    for (int n : item_counts) {
        for (int capacity : capacities) {
            generateKnapsackData(n, capacity, max_size, max_value, "temp_test.txt");
            
            std::vector<Item> items;
            int cap;
            loadFromFile("temp_test.txt", items, cap);
            
            double dp_time = measureTime([&]() { return solveDynamicProgramming(items, capacity); });
            double greedy_time = measureTime([&]() { return solveGreedy(items, capacity); });
            double bf_time = measureTime([&]() { return solveBruteForce(items, capacity); }, 1);
            
            file << n << "," << capacity << "," << dp_time << "," << greedy_time << ",";
            file << (bf_time < 60000 ? std::to_string(bf_time) : "TIMEOUT") << "\n";
            
            std::cout << "n=" << n << ", capacity=" << capacity << " completed" << std::endl;
        }
    }
    
    file.close();
    std::cout << "Data saved to: time_vs_n_and_b.csv" << std::endl;
}

void analyzeGreedyQuality() {
    std::cout << "Analyzing greedy algorithm quality..." << std::endl;
    
    std::ofstream file("greedy_quality_analysis.csv");
    file << std::fixed << std::setprecision(6);
    file << "test_case,n,capacity,DP_value,Greedy_value,quality_ratio,is_optimal\n";
    
    int test_case = 0;
    
    // Even-numbered test scenarios
    std::vector<std::tuple<int, int, int, int>> scenarios;
    for (int n = 10; n <= 50; n += 2) {
        int capacity = n * 5;
        int max_size = n / 2 + 5;
        int max_value = n * 10;
        scenarios.emplace_back(n, capacity, max_size, max_value);
    }
    
    for (auto& scenario : scenarios) {
        int n, capacity, max_size, max_value;
        std::tie(n, capacity, max_size, max_value) = scenario;
        
        // 5 instances per scenario
        for (int instance = 0; instance < 5; instance++) {
            generateKnapsackData(n, capacity, max_size, max_value, "temp_test.txt");
            
            std::vector<Item> items;
            int cap;
            loadFromFile("temp_test.txt", items, cap);
            
            Solution dp_sol = solveDynamicProgramming(items, capacity);
            Solution greedy_sol = solveGreedy(items, capacity);
            
            double quality_ratio = (dp_sol.totalValue > 0) ? (double)greedy_sol.totalValue / dp_sol.totalValue : 1.0;
            bool is_optimal = (greedy_sol.totalValue == dp_sol.totalValue);
            
            file << test_case << "," << n << "," << capacity << "," 
                 << dp_sol.totalValue << "," << greedy_sol.totalValue << "," 
                 << quality_ratio << "," << (is_optimal ? 1 : 0) << "\n";
            
            test_case++;
        }
        std::cout << "Scenario n=" << n << ", capacity=" << capacity << " completed" << std::endl;
    }
    
    file.close();
    std::cout << "Data saved to: greedy_quality_analysis.csv" << std::endl;
}

int main() {
    std::cout << "=== KNAPSACK ALGORITHMS PERFORMANCE ANALYSIS ===" << std::endl;
    std::cout << "Brute-force timeout set to 1 minute (60000ms)" << std::endl;
    
    int choice;
    std::cout << "\nSelect analysis:" << std::endl;
    std::cout << "1. All analyses" << std::endl;
    std::cout << "2. t=f(n) analysis" << std::endl;
    std::cout << "3. t=f(b) analysis" << std::endl;
    std::cout << "4. t=f(n,b) analysis" << std::endl;
    std::cout << "5. Greedy quality analysis" << std::endl;
    std::cout << "Choice: ";
    std::cin >> choice;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    switch(choice) {
        case 1:
            generateTimeVsItemsData();
            generateTimeVsCapacityData();
            generateTimeVsNAndBData();
            analyzeGreedyQuality();
            break;
        case 2:
            generateTimeVsItemsData();
            break;
        case 3:
            generateTimeVsCapacityData();
            break;
        case 4:
            generateTimeVsNAndBData();
            break;
        case 5:
            analyzeGreedyQuality();
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return 1;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    
    std::cout << "\n=== ANALYSIS COMPLETED ===" << std::endl;
    std::cout << "Total execution time: " << duration.count() << " seconds" << std::endl;
    std::cout << "CSV files generated and ready for plotting." << std::endl;
    
    return 0;
}