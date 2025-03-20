#include "sorts.h"

int main() {
    vector<int> vec10 = { 4, 9, 6, 3, 0, 8, 4, 2, 7, 7 };
    vector<int> vec = generate_random_vector(100000, 0, 100);

    //cout<<"Poczatkowa tablica:\n";
    //print_vector(vec);

    auto start = high_resolution_clock::now();
    //merge_sort(vec, 0, vec.size() - 1);
    bubble_sort(vec);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    //cout << "Posortowana tablica:\n";
    //print_vector(vec);
    cout << "Czas sortowania: " << duration.count() << " milisekund\n";
    cout << "Liczba porownan: " << liczba_porownan << endl;
    cout << "Liczba zamian: " << liczba_zamian << endl;
}