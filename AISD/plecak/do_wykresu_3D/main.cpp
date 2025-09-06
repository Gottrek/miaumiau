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

double measureTime(std::function<Solution()> func, int runs = 1) {
    // Warm-up run
    func();
    
    auto start = std::chrono::high_resolution_clock::now();
    Solution result = func();
    auto end = std::chrono::high_resolution_clock::now();
    
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
}

void generate3DComparisonData() {
    std::cout << "Generating 3D comparison data for all algorithms..." << std::endl;
    
    std::ofstream dp_file("3d_dp_time.csv");
    std::ofstream greedy_file("3d_greedy_time.csv");
    std::ofstream brute_file("3d_brute_time.csv");
    
    dp_file << std::fixed << std::setprecision(6);
    greedy_file << std::fixed << std::setprecision(6);
    brute_file << std::fixed << std::setprecision(6);
    
    // Smaller range for brute-force
    const int min_n = 1;
    const int max_n = 25;  // Brute-force becomes impractical above this
    const int n_step = 1;
    
    const int min_capacity = 10;
    const int max_capacity = 200;
    const int capacity_step = 10;
    
    // Generate headers
    dp_file << "n\\b";
    greedy_file << "n\\b";
    brute_file << "n\\b";
    
    for (int capacity = min_capacity; capacity <= max_capacity; capacity += capacity_step) {
        dp_file << "," << capacity;
        greedy_file << "," << capacity;
        brute_file << "," << capacity;
    }
    dp_file << "\n";
    greedy_file << "\n";
    brute_file << "\n";
    
    const int max_size = 30;
    const int max_value = 100;
    
    for (int n = min_n; n <= max_n; n += n_step) {
        dp_file << n;
        greedy_file << n;
        brute_file << n;
        
        for (int capacity = min_capacity; capacity <= max_capacity; capacity += capacity_step) {
            generateKnapsackData(n, capacity, max_size, max_value, "temp_test.txt");
            
            std::vector<Item> items;
            int cap;
            loadFromFile("temp_test.txt", items, cap);
            
            double dp_time = measureTime([&]() { return solveDynamicProgramming(items, capacity); });
            double greedy_time = measureTime([&]() { return solveGreedy(items, capacity); });
            double brute_time = measureTime([&]() { return solveBruteForce(items, capacity); });
            
            dp_file << "," << dp_time;
            greedy_file << "," << greedy_time;
            brute_file << "," << brute_time;
            
            std::cout << "n=" << n << ", b=" << capacity 
                      << " | DP: " << dp_time << "ms"
                      << " | Greedy: " << greedy_time << "ms"
                      << " | Brute: " << brute_time << "ms"
                      << std::endl;
        }
        dp_file << "\n";
        greedy_file << "\n";
        brute_file << "\n";
    }
    
    dp_file.close();
    greedy_file.close();
    brute_file.close();
    
    std::cout << "\nData generation complete. Files created:" << std::endl;
    std::cout << "- 3d_dp_time.csv" << std::endl;
    std::cout << "- 3d_greedy_time.csv" << std::endl;
    std::cout << "- 3d_brute_time.csv" << std::endl;
}

int main() {
    std::cout << "=== KNAPSACK 3D ALGORITHM COMPARISON ===" << std::endl;
    std::cout << "Generating data for DP, Greedy and Brute-force algorithms..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    generate3DComparisonData();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end - start);
    std::cout << "\nTotal execution time: " << duration.count() << " minutes" << std::endl;
    
    std::cout << "\n=== EXCEL INSTRUCTIONS ===" << std::endl;
    std::cout << "1. Open any of the CSV files in Excel" << std::endl;
    std::cout << "2. Select all data (including headers)" << std::endl;
    std::cout << "3. Go to Insert > Charts > Surface > 3D Surface" << std::endl;
    std::cout << "4. Customize the chart:" << std::endl;
    std::cout << "   - X-axis: Capacity (b)" << std::endl;
    std::cout << "   - Y-axis: Items (n)" << std::endl;
    std::cout << "   - Z-axis: Time (ms)" << std::endl;
    std::cout << "5. Repeat for each algorithm file" << std::endl;
    
    return 0;
}