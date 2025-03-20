#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include "sorts.cpp"
#include "merge_sort.cpp"

using namespace std;
using namespace chrono;

void merge_sort(vector<int>& arr, int left, int right);

#endif