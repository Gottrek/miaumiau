#include "sorts.h"

using namespace std;

void insertion_sort(vector<int>& arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Przesuwanie elementów w górę, aby znaleźć właściwą pozycję dla key
        while (j >= 0) {
            liczba_porownan++;
            if (arr[j] < key) { // Sortowanie malejące
                arr[j + 1] = arr[j];
                liczba_zamian++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
        liczba_zamian++;
    }
}