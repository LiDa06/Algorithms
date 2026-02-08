#pragma once
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>

class HyperLogLog {
private:
    uint32_t p;
    uint32_t m;
    std::vector<uint8_t> registers;

    static double alpha(uint32_t m) {
        if (m == 16) return 0.673;
        if (m == 32) return 0.697;
        if (m == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / m);
    }

    static uint8_t rho(uint64_t x, uint32_t max_bits) {
        uint8_t r = 1;
        while (r <= max_bits && ((x & (1ULL << (max_bits - r))) == 0))
            ++r;
        return r;
    }

public:
    explicit HyperLogLog(uint32_t precision)
        : p(precision), m(1u << p), registers(m, 0) {}

    void add(uint64_t hash) {
        uint32_t idx = hash >> (64 - p);
        uint64_t w = (hash << p);
        uint8_t r = rho(w, 64 - p);
        registers[idx] = std::max(registers[idx], r);
    }

    double estimate() const {
        double sum = 0.0;
        for (uint8_t r : registers)
            sum += std::pow(2.0, -static_cast<int>(r));

        double raw = alpha(m) * m * m / sum;

        // Small range correction
        if (raw <= 2.5 * m) {
            int zeros = std::count(registers.begin(), registers.end(), 0);
            if (zeros > 0)
                return m * std::log(static_cast<double>(m) / zeros);
        }

        return raw;
    }
};
