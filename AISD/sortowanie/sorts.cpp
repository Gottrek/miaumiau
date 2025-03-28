#include "sorts.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

long long int liczba_porownan = 0;
long long int liczba_zamian = 0;

void print_vector(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

vector<int> generate_random_vector(int n, int min, int max, int mode = 0) {
    random_device rd;   // Seed for random number generator
    mt19937 gen(rd());  // Mersenne Twister PRNG
    uniform_int_distribution<int> dist(min, max);  // Uniform distribution in range [min, max]

    vector<int> vec(n);
    for (int &num : vec) {
        num = dist(gen);  // Generate random number
    }

    switch (mode)
    {
    case 1: // Sorted in ascending order
        sort(vec.begin(), vec.end());
        break;
    case 2: // Sorted in descending order
        sort(vec.begin(), vec.end(), greater<int>());
        break;
    case 3: // A-shaped (mountain shape, increasing then decreasing)
        sort(vec.begin(), vec.end());  
        for (int i = 0; i < n / 2; i++) {
            swap(vec[i], vec[n - i - 1]);  // Swap first half to reverse it
        }
        break;
    case 4: // V-shaped (valley shape, decreasing then increasing)
        sort(vec.begin(), vec.end(), greater<int>());
        for (int i = 0; i < n / 2; i++) {
            swap(vec[i], vec[n - i - 1]);  // Swap first half to reverse it
        }
        break;
    default:
        break;
    }

    return vec;
}


void swap_ints(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

void save_to_file(const vector<double>& data, const string& filename) {
    ofstream file(filename, ios::app);
    
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    file << fixed << setprecision(2);  // Set decimal precision (optional)

    for (size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i < data.size() - 1) file << " ";  // Separate numbers with spaces
    }

    file << endl;
    file.close();
    cout << "Data saved to " << filename << endl;
}

vector<double> calculate_average(const vector<vector<int>>& data) {
    if (data.empty()) return {0.0, 0.0, 0.0};  // Handle empty input

    int num_measurements = data.size();
    vector<double> averages(3, 0.0);  // To store averages for each column

    // Sum each column separately
    for (const auto& row : data) {
        for (size_t i = 0; i < 3; i++) {
            averages[i] += row[i];
        }
    }

    // Divide sums by the number of measurements to get averages
    for (double& avg : averages) {
        avg /= num_measurements;
    }

    return averages;
}