#pragma once
#include <cstdint>
#include <string>

class HashFunc {
public:
    static uint64_t hash(const std::string& s) {
        uint64_t x = 0xcbf29ce484222325ULL;
        for (char c : s) {
            x ^= static_cast<uint64_t>(c);
            x *= 0x100000001b3ULL;
        }
        return splitmix64(x);
    }

private:
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
};
