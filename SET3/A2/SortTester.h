#include <bits/stdc++.h>
using namespace std;

class SortTester {
public:
    static vector<int> loadArray(const string& filename) {
        ifstream in(filename);
        vector<int> a;
        int x;
        while (in >> x) a.push_back(x);
        return a;
    }

    template<typename SortFn>
    static long long runAndMeasure(const string& filename, SortFn sorter) {
        auto arr = loadArray(filename);

        auto start = chrono::high_resolution_clock::now();
        sorter(arr);                      
        auto end = chrono::high_resolution_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        return ms;
    }

    template<typename HybridFn>
    static long long runHybrid(const string& filename, HybridFn sorter, int k) {
        auto arr = loadArray(filename);

        auto start = chrono::high_resolution_clock::now();
        sorter(arr, k);
        auto end = chrono::high_resolution_clock::now();

        auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        return ms;
    }
};
