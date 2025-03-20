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
