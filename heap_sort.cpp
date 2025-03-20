#include "sorts.h"

using namespace std;

void ukopcuj(vector<int>& arr, int n, int i)
{
    int smallest = i; // korzen
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // sprawdzenie czy lewe dziecko jest wieksze od korzenia
    if (left < n) {
        liczba_porownan++;
        if (arr[left] < arr[smallest])
            smallest = left;
    }

    // to samo dla prawego
    if (right < n) {
        liczba_porownan++;
        if (arr[right] < arr[smallest])
            smallest = right;
    }

    // jak smallest nie jest korzeniem to zamieniamy z impostorem
    if (smallest != i) {
        swap_ints(arr[i], arr[smallest]);
        liczba_zamian++;
        ukopcuj(arr, n, smallest);
    }
}

void heap_sort(vector<int>& arr)
{
    int n = arr.size();

    // kopcowanie
    for (int i = n / 2 - 1; i >= 0; i--)
        ukopcuj(arr, n, i);
    
    // przekopcowywanie
    for (int i = n - 1; i > 0; i--) {
        swap_ints(arr[0], arr[i]);
        liczba_zamian++;
        ukopcuj(arr, i, 0);
    }
}