#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

extern long long int liczba_porownan;
extern long long int liczba_zamian;

void print_vector(vector<int>& arr);
vector<int> generate_random_vector(int n, int min, int max, int mode);
void swap_ints(int &a, int &b);
void save_to_file(const vector<double>& data, const string& filename);
vector<double> calculate_average(const vector<vector<long long int>>& data);

void merge_sort(vector<int>& arr, int left, int right);
void bubble_sort(vector<int>& arr);
void selection_sort(vector<int>& arr);
void insertion_sort(vector<int>& arr);
void quick_sort(vector<int>& arr, int low, int high, int depth = 0);
void heap_sort(vector<int>& arr);

#endif