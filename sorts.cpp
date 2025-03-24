#include "sorts.h"

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

vector<int> generate_random_vector(int n, int min, int max) {
    random_device rd;   // Seed for random number generator
    mt19937 gen(rd());  // Mersenne Twister PRNG
    uniform_int_distribution<int> dist(min, max);  // Uniform distribution in range [min, max]

    vector<int> vec(n);
    for (int &num : vec) {
        num = dist(gen);  // Generate random number
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
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    file << fixed << setprecision(3);  // Set decimal precision (optional)

    for (size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i < data.size() - 1) file << " ";  // Separate numbers with spaces
    }

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