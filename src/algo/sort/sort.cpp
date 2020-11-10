// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <string>
#include <iostream>
#include <chrono>
#include <random>

#include "sort.h"
#include "merge_sort.h"
#include "heap_sort.h"

using namespace std;

#ifdef NDEBUG
#define EXPECT(cond)    do { if (!(cond)) {int* p = 0; *p = 1;}} while(0)
#else
#define EXPECT(cond)    assert(cond)
#endif

// now time in milliseconds
static int64_t now_ms()
{
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

template <typename T>
inline void printArray(const T arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}


template <typename T>
void do_sort(const std::string& algo, T arr[], int size)
{
    if (algo == "bubble") {
        bubble_sort(arr, size);
    }
    else if (algo == "selection") {
        selection_sort(arr, size);
    }
    else if (algo == "insertion") {
        insertion_sort_aux(arr, size);
    } 
    else if (algo == "shell") {
        shell_sort(arr, size);
    }
    else if (algo == "quick") {
        quick_sort(arr, size);
    }
    else if (algo == "merge") {
        merge_sort(arr, size);
    }
    else if (algo == "heap") {
        heap_sort(arr, size);
    }
}

const std::string all_sort_algo[] = {"bubble", "selection", "insertion", "shell", "quick", "merge", "heap"};

static void fill_random_array(int N, std::vector<int>& arr)
{
    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<> dist(0, N);
    arr.resize(N);
    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = dist(rnd);
    }
}

static void test_sort_algo()
{
    int N = 1000;
    std::vector<int> arr;
    fill_random_array(N, arr);

    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    EXPECT(std::is_sorted(sorted.begin(), sorted.end()));

    for (const std::string& algo : all_sort_algo)
    {
        std::vector<int> copy = arr;
        do_sort(algo, (int*)copy.data(), copy.size());
        EXPECT(std::is_sorted(copy.begin(), copy.end()));
        EXPECT(copy == sorted);
    }
}

static double bench_random_input(const std::string& algo, int N, int T)
{
    std::vector<int> arr;
    int64_t total = 0;
    for (int i = 0; i < T; i++)
    {
        arr.clear();
        fill_random_array(N, arr);
        int64_t start = now_ms();
        do_sort(algo, (int*)arr.data(), N);
        int64_t end = now_ms();
        total += end - start;
    }
    return double(total) / 1000.0;
}

static void benchmark_sort(const std::string& algo1, const std::string& algo2, int N, int T)
{
    double t1 = bench_random_input(algo1, N, T);
    double t2 = bench_random_input(algo2, N, T);
    printf("for %d random integers\n", N);
    printf("%s(%.2f) is %.2f times faster than %s(%.2f)\n", algo1.data(), t1, t2/t1, algo2.data(), t2);
}

int main(int argc, char* argv[])
{
    test_sort_algo();

    if (argc != 5) {
        printf("Usage: %s N T algo1 algo2\n", argv[0]);
        return 1;
    }

    string algo1 = argv[1];
    string algo2 = argv[2];
    int N = atoi(argv[3]);
    int T = atoi(argv[4]);

    benchmark_sort(algo1, algo2, N, T);

    return 0;
}
