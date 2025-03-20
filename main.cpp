#include "sorts.h"

int main() {
    vector<int> vec10 = { 4, 9, 6, 3, 0, 8, 4, 2, 7, 7 };
    vector<int> vec = generate_random_vector(10000000, 0, 100);

    //cout<<"Poczatkowa tablica:\n";
    //print_vector(vec);

    auto start = high_resolution_clock::now();
    merge_sort(vec, 0, vec.size() - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    //cout<<"Posortowana tablica:\n";
    //print_vector(vec);
    cout<<"Czas sortowania:\n" << duration.count() << " milisekund\n";
}