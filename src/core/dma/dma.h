#pragma once
#include <cstdint>

struct Memory; // forward declaration

struct DMAChannel {
    uint32_t src = 0;
    uint32_t dst = 0;
    uint32_t cnt = 0;
    bool active = false;
};

struct DMA {
    Memory* mem = nullptr;
    DMAChannel ch[4];

    void init(Memory* memory);
    void reset();

    void write(int id, int reg, uint32_t v);
    uint32_t read(int id, int reg);

    void step();
    void execute(int id);
};
