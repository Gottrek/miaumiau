#include "sorts.h"

using namespace std;

void bubble_sort(vector<int>& arr)
{
    int n = arr.size();
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                liczba_zamian++;
            }
            liczba_porownan++;
        }
    }
}