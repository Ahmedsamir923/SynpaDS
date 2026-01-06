#pragma once
#include <cstdint>

struct CPU;

struct IRQ {
    CPU* cpu = nullptr;

    uint32_t IME = 0;
    uint32_t IE = 0;
    uint32_t IF = 0;

    void init(CPU* c);
    void reset();

    void request(uint32_t bit);
    void step();

    uint32_t read(uint32_t addr);
    void write(uint32_t addr, uint32_t v);
};

