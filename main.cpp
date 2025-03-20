#include "sorts.h"

int main() {
    vector<int> vec10 = { 4, 9, 6, 3, 0, 8, 4, 2, 7, 7 };
    vector<int> vec = generate_random_vector(1000000, 0, 100);

    cout << "Poczatkowa tablica:\n";
    print_vector(vec10);

    auto start = high_resolution_clock::now();

    //bubble_sort(vec10);
    //selection_sort(vec10);
    //insertion_sort(vec10);
    //quick_sort(vec10, 0, vec.size() - 1);
    //merge_sort(vec10, 0, vec10.size() - 1);
    //heap_sort(vec10);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Posortowana tablica:\n";
    print_vector(vec10);
    cout << "Czas sortowania: " << duration.count() << " milisekund\n";
    cout << "Liczba porownan: " << liczba_porownan << endl;
    cout << "Liczba zamian: " << liczba_zamian << endl;
}