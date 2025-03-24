#include "sorts.h"

int main() {
    //vector<int> vec10 = { 4, 9, 6, 3, 0, 8, 4, 2, 7, 7 };
    vector<vector<int>> dane(10, vector<int>(3, 0)); // czas, porownania i zamiany do zrobienia sredniej

    //cout << "Poczatkowa tablica:\n";
    //print_vector(vec10);

    for (int i = 0; i < 10; i++) {
        vector<int> vec = generate_random_vector(10000, 0, 100);
        auto start = high_resolution_clock::now();

        //bubble_sort(vec);
        //selection_sort(vec10);
        //insertion_sort(vec10);
        //quick_sort(vec10, 0, vec.size() - 1);
        //merge_sort(vec10, 0, vec10.size() - 1);
        heap_sort(vec);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        dane[i][0] = duration.count();
        dane[i][1] = liczba_porownan;
        dane[i][2] = liczba_zamian;
        liczba_porownan = 0;
        liczba_zamian = 0;
    }

    for (int i = 0; i < 10; i++) {
        cout << i + 1 << ") czas: " << dane[i][0] << " milisekund, liczba porownan: " << dane[i][1] << ", liczba zamian: " << dane[i][2] << endl;
    }

    vector<double> dot_on_a_graph = calculate_average(dane);
    for (int i = 0; i < 3; i++) {
        cout << dot_on_a_graph[i] << endl;
    }

    save_to_file(dot_on_a_graph, "sortowe_pomiary.txt");
    //cout << "Posortowana tablica:\n";
    //print_vector(vec10);
    //cout << "Czas sortowania: " << dane[5][0] << " milisekund\n";
    //cout << "Liczba porownan: " << liczba_porownan << endl;
    //cout << "Liczba zamian: " << liczba_zamian << endl;
}