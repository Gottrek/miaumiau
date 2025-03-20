#include <iostream>
#include "sorts.h"

using namespace std;

void print_vector(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}