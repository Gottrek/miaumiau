#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

struct Item {
    int size;
    int value;
    int index;
    double ratio;
    
    Item(int s, int v, int i) : size(s), value(v), index(i) {
        ratio = (double)value / size;
    }
};

struct Solution {
    std::vector<bool> selected;
    int totalValue;
    int totalSize;
    
    Solution(int n) : selected(n, false), totalValue(0), totalSize(0) {}
};

// Wczytywanie danych z pliku
bool loadFromFile(const std::string& filename, std::vector<Item>& items, int& capacity) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Błąd: Nie można otworzyć pliku " << filename << std::endl;
        return false;
    }
    
    int n;
    file >> n >> capacity;
    items.clear();
    
    for (int i = 0; i < n; i++) {
        int size, value;
        file >> size >> value;
        items.emplace_back(size, value, i);
    }
    
    file.close();
    return true;
}

// Wyświetlanie rozwiązania
void printSolution(const Solution& sol, const std::vector<Item>& items, int capacity, const std::string& algorithmName) {
    std::cout << "\n=== " << algorithmName << " ===" << std::endl;
    std::cout << "Wybrane przedmioty:" << std::endl;
    
    for (int i = 0; i < items.size(); i++) {
        if (sol.selected[i]) {
            std::cout << "Przedmiot " << (i + 1) 
                     << " (rozmiar: " << items[i].size 
                     << ", wartosc: " << items[i].value 
                     << ", wspolczynnik: " << std::fixed << std::setprecision(2) 
                     << items[i].ratio << ")" << std::endl;
        }
    }
    
    std::cout << "Sumaryczny rozmiar: " << sol.totalSize << "/" << capacity << std::endl;
    std::cout << "Sumaryczna wartosc: " << sol.totalValue << std::endl;
}

// Algorytm Programowania Dynamicznego (AD)
Solution solveDynamicProgramming(const std::vector<Item>& items, int capacity) {
    int n = items.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    
    // Wypełnianie tabeli DP
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i-1].size <= w) {
                dp[i][w] = std::max(dp[i-1][w], 
                                   dp[i-1][w - items[i-1].size] + items[i-1].value);
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    // Odtwarzanie rozwiązania
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

// Algorytm Zachłanny (AZ)
Solution solveGreedy(const std::vector<Item>& items, int capacity) {
    int n = items.size();
    
    // Sortowanie według współczynnika opłacalności (wartość/rozmiar)
    std::vector<Item> sortedItems = items;
    std::sort(sortedItems.begin(), sortedItems.end(), 
             [](const Item& a, const Item& b) {
                 return a.ratio > b.ratio;
             });
    
    Solution solution(n);
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

// Algorytm Siłowy (AB)
Solution solveBruteForce(const std::vector<Item>& items, int capacity) {
    int n = items.size();
    int fmax = 0;
    int bestMask = 0;
    
    // Dla każdej liczby X od 1 do 2^n-1
    for (int X = 1; X < (1 << n); X++) {
        
        // W(X) = suma wag przedmiotów w plecaku
        int W = 0;
        for (int i = 0; i < n; i++) {
            if (X & (1 << i)) {
                W += items[i].size;
            }
        }
        
        // jeśli W(X) <= c (rozwiązanie dopuszczalne)
        if (W <= capacity) {
            // f(X) = suma wartości przedmiotów w plecaku
            int f = 0;
            for (int i = 0; i < n; i++) {
                if (X & (1 << i)) {
                    f += items[i].value;
                }
            }
            
            // jeśli f(X) > fmax
            if (f > fmax) {
                fmax = f;
                bestMask = X;
            }
        }
    }
    
    // Budowanie rozwiązania z najlepszej maski
    Solution solution(n);
    solution.totalValue = fmax;
    for (int i = 0; i < n; i++) {
        if (bestMask & (1 << i)) {
            solution.selected[i] = true;
            solution.totalSize += items[i].size;
        }
    }
    
    return solution;
}

int main() {
    std::string filename;
    std::vector<Item> items;
    int capacity;
    
    std::cout << "Podaj nazwe pliku z danymi: ";
    std::cin >> filename;
    
    if (!loadFromFile(filename, items, capacity)) {
        return 1;
    }
    
    // Algorytm Programowania Dynamicznego
    Solution dpSolution = solveDynamicProgramming(items, capacity);
    printSolution(dpSolution, items, capacity, "Algorytm Programowania Dynamicznego (AD)");
    
    // Algorytm Zachłanny
    Solution greedySolution = solveGreedy(items, capacity);
    printSolution(greedySolution, items, capacity, "Algorytm Zachlanny (AZ)");
    
    // Algorytm Siłowy
    Solution bruteForceSolution = solveBruteForce(items, capacity);
    printSolution(bruteForceSolution, items, capacity, "Algorytm Silowy (AB)");
    
    return 0;
}