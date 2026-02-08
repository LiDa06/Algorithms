#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include "random_stream.h"
#include "hash.h"
#include "hyperloglog.h"

struct Stats {
    double sum = 0.0;
    double sum_sq = 0.0;
    int cnt = 0;

    void add(double x) {
        sum += x;
        sum_sq += x * x;
        cnt++;
    }

    double mean() const {
        return sum / cnt;
    }

    double stddev() const {
        double m = mean();
        return std::sqrt(sum_sq / cnt - m * m);
    }
};

int main() {
    constexpr size_t N = 1'000'000;
    constexpr uint32_t P = 14;
    constexpr int R = 10;
    constexpr int STEPS = 20;

    size_t step_size = N / STEPS;

    std::vector<Stats> stats(STEPS);
    std::vector<size_t> real_vals(STEPS);

    for (int run = 0; run < R; ++run) {
        RandomStreamGen gen(100 + run);
        HyperLogLog hll(P);
        std::unordered_set<uint64_t> exact;
        exact.reserve(N);

        for (int i = 0; i < STEPS; ++i) {
            for (size_t j = 0; j < step_size; ++j) {
                std::string s = gen.generate_string();
                uint64_t h = HashFunc::hash(s);
                hll.add(h);
                exact.insert(h);
            }

            double est = hll.estimate();
            size_t real = exact.size();

            stats[i].add(est);
            real_vals[i] = real;
        }

        std::cerr << "Run " << run + 1 << " done\n";
    }

    std::cout << "step,real,mean_est,stddev,rel_error\n";
    for (int i = 0; i < STEPS; ++i) {
        double mean = stats[i].mean();
        double sd = stats[i].stddev();
        double rel_err = std::abs(mean - real_vals[i]) / real_vals[i];

        std::cout
            << (i + 1) * step_size << ","
            << real_vals[i] << ","
            << mean << ","
            << sd << ","
            << rel_err << "\n";
    }
}
