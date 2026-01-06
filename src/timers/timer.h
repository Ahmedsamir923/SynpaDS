#pragma once
#include <cstdint>

struct IRQ;

struct Timer {
    uint16_t reload = 0;
    uint16_t counter = 0;
    uint16_t control = 0;
    uint32_t prescale = 0;

    void reset();
    void writeCNT_L(uint16_t v);
    void writeCNT_H(uint16_t v);
    uint16_t readCNT_L() const;
    uint16_t readCNT_H() const;

    void step(IRQ* irq, int id);
};
