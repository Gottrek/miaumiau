#include "sorts.h"

using namespace std;

void selection_sort(vector<int>& arr)
{
    int n = arr.size();
    int max_index = 0;
    int temp;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            liczba_porownan++;
            if (arr[j] > arr[max_index]) {
                max_index = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[max_index];
        arr[max_index] = temp;
        liczba_zamian++;
    }
}