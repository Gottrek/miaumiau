#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <iostream>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

extern long long int liczba_porownan;
extern long long int liczba_zamian;

void print_vector(vector<int>& arr);
vector<int> generate_random_vector(int n, int min, int max);
void swap_ints(int &a, int &b);

void merge_sort(vector<int>& arr, int left, int right);
void bubble_sort(vector<int>& arr);
void selection_sort(vector<int>& arr);
void insertion_sort(vector<int>& arr);
void quick_sort(vector<int>& arr, int low, int high);
void heap_sort(vector<int>& arr);

#endif