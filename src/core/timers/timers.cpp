#include "timers.h"

static const uint32_t prescaleTable[4] = { 1, 64, 256, 1024 };

void Timers::reset() {
    for (int i = 0; i < 4; i++) {
        tm[i] = {};
    }
}

void Timers::step(uint32_t cycles) {

    for (int i = 0; i < 4; i++) {
        Timer& t = tm[i];

        if (!(t.control & (1 << 7)))
            continue;

        bool countUp = t.control & (1 << 2);
        if (countUp)
            continue; // TM1+ incrementado por overflow do anterior

        uint32_t prescaler = prescaleTable[t.control & 3];
        t.prescaleCounter += cycles;

        while (t.prescaleCounter >= prescaler) {
            t.prescaleCounter -= prescaler;

            t.counter++;
            if (t.counter == 0) {
                t.counter = t.reload;

                // IRQ será tratado fora (Memory)
            }
        }
    }
}

uint16_t Timers::readCNT_L(int id) {
    return tm[id].counter;
}

uint16_t Timers::readCNT_H(int id) {
    return tm[id].control;
}

void Timers::writeCNT_L(int id, uint16_t v) {
    tm[id].reload = v;
}

void Timers::writeCNT_H(int id, uint16_t v) {
    bool wasEnabled = tm[id].control & (1 << 7);
    tm[id].control = v;

    if (!wasEnabled && (v & (1 << 7))) {
        tm[id].counter = tm[id].reload;
        tm[id].prescaleCounter = 0;
    }
}
