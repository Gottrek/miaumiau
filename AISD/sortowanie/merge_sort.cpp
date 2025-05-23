#include "sorts.h"

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    // kopiowanie liczb do 2 tymczasowych wektorow
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0;
    int k = left;

    // ponowne laczenie wektorow
    while (i < n1 && j < n2) {
        liczba_porownan++;
        if (L[i] >= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // kopiowanie reszty z L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // kopiowanie reszty z R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(vector<int>& arr, int left, int right)
{
    if (left >= right)
        return;
    
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}