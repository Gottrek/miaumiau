#include "sorts.h"

using namespace std;

int partition(vector<int>& arr, int low, int high)
{
    // ostatni element jako pivot
    int pivot = arr[high];

    // przedostatni element (do swapa)
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        liczba_porownan++;
        if (arr[j] >= pivot) {
            i++;
            swap_ints(arr[i], arr[j]);
            liczba_zamian++;
        }
    }

    // ustawienie pivota
    swap_ints(arr[i + 1], arr[high]);

    // tam dzielimy liste
    return (i + 1);
}


void quick_sort(vector<int>& arr, int low, int high)
{
    if (low < high) {
        int part_index = partition(arr, low, high);

        quick_sort(arr, low, part_index - 1);
        quick_sort(arr, part_index + 1, high);
    }
}