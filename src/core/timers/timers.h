#pragma once
#include <cstdint>

struct Timer {
    uint16_t reload = 0;
    uint16_t counter = 0;
    uint16_t control = 0;

    uint32_t prescaleCounter = 0;
};

struct Timers {
    Timer tm[4];

    void reset();
    void step(uint32_t cycles);

    uint16_t readCNT_L(int id);
    uint16_t readCNT_H(int id);

    void writeCNT_L(int id, uint16_t v);
    void writeCNT_H(int id, uint16_t v);
};
#pragma once
