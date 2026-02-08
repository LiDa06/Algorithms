#pragma once
#include <string>
#include <vector>
#include <random>

class RandomStreamGen {
private:
    std::mt19937 rng;
    static constexpr char alphabet[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789-";

public:
    explicit RandomStreamGen(uint32_t seed = 42) : rng(seed) {}

    std::string generate_string(size_t len = 16) {
        std::uniform_int_distribution<size_t> dist(0, sizeof(alphabet) - 2);
        std::string s;
        s.reserve(len);
        for (size_t i = 0; i < len; ++i)
            s.push_back(alphabet[dist(rng)]);
        return s;
    }

    std::vector<std::string> generate_stream(size_t n, size_t strlen = 16) {
        std::vector<std::string> stream;
        stream.reserve(n);
        for (size_t i = 0; i < n; ++i)
            stream.push_back(generate_string(strlen));
        return stream;
    }
};
