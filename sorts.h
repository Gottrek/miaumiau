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

int liczba_porownan = 0, liczba_zamian = 0;

void print_vector(vector<int>& arr);
vector<int> generate_random_vector(int n, int min, int max);

void merge_sort(vector<int>& arr, int left, int right);
void bubble_sort(vector<int>& arr);

#endif