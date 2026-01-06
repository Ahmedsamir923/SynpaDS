#include "timer.h"
#include "../arm9/irq.h"

static constexpr int prescalerTable[4] = { 1, 64, 256, 1024 };

void Timer::reset() {
    reload = counter = control = 0;
    prescale = 0;
}

void Timer::writeCNT_L(uint16_t v) {
    reload = v;
}

void Timer::writeCNT_H(uint16_t v) {
    bool wasEnabled = control & (1 << 7);
    control = v;

    if (!wasEnabled && (control & (1 << 7))) {
        counter = reload;
        prescale = 0;
    }
}

uint16_t Timer::readCNT_L() const {
    return counter;
}

uint16_t Timer::readCNT_H() const {
    return control;
}

void Timer::step(IRQ* irq, int id) {
    if (!(control & (1 << 7))) return;

    prescale++;
    if (prescale < prescalerTable[control & 3]) return;
    prescale = 0;

    counter++;
    if (counter == 0) {
        counter = reload;
        if (control & (1 << 6))
            irq->request(1 << (3 + id)); // TIMER0 = bit 3
    }
}
