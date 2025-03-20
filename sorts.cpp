#include <iostream>
#include "sorts.h"

using namespace std;

void print_vector(vector<int>& v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}