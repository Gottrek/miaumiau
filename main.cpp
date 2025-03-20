#include "sorts.h"

int main() {
    vector<int> vec = { 4, 9, 6, 3, 0, 8, 4, 2, 7, 7 };
    cout<<"Poczatkowa tablica:\n";
    print_vector(vec);
    merge_sort(vec, 0, 9);
    cout<<"Posortowana tablica:\n";
    print_vector(vec);
}