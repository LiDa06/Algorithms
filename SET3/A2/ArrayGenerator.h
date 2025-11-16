#include <bits/stdc++.h>

using namespace std;

class ArrayGenerator {
private:
    mt19937_64 rnd;

public:
    ArrayGenerator() {
        random_device rd;
        rnd.seed(rd());
    }

    vector<int> randomArray(int n, int maxValue = 100000) {
        uniform_int_distribution<int> dist(0, maxValue);
        vector<int> a(n);
        for (auto& x : a) x = dist(rnd);
        return a;
    }

    vector<int> reversedArray(int n) {
        auto a = randomArray(n);
        sort(a.begin(), a.end());
        reverse(a.begin(), a.end());
        return a;
    }

    vector<int> almostSortedArray(int n, int k = 100) {
        auto a = randomArray(n);
        sort(a.begin(), a.end());
        while (k > 0){
            swap(a[rnd() % n], a[rnd() % n]);
            --k;
        }
        return a;
    }

    void saveToFile(const vector<int>& arr, const string& filename) {
        ofstream out(filename);
        for (int i = 0; i < arr.size(); i++) {
            out << arr[i];
            if (i + 1 < arr.size()) out << ' ';
        }
    }
};
