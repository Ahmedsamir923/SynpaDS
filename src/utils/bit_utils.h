#pragma once
#include <cstdint>

inline int popcount(uint32_t v) {
    int count = 0;
    while (v) {
        v &= v - 1;
        count++;
    }
    return count;
}

