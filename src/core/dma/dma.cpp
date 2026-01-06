#include "dma.h"
#include "../memory/memory.h"

void DMA::init(Memory* memory) {
    mem = memory;
}

void DMA::reset() {
    for (int i = 0; i < 4; i++)
        ch[i] = {};
}

uint32_t DMA::read(int id, int reg) {
    DMAChannel& d = ch[id];
    switch (reg) {
    case 0: return d.src;
    case 1: return d.dst;
    case 2: return d.cnt;
    }
    return 0;
}

void DMA::write(int id, int reg, uint32_t v) {
    DMAChannel& d = ch[id];

    switch (reg) {
    case 0: d.src = v; break;
    case 1: d.dst = v; break;
    case 2:
        d.cnt = v;
        if (v & (1u << 31))
            d.active = true;
        break;
    }
}

void DMA::step() {
    for (int i = 0; i < 4; i++) {
        if (ch[i].active)
            execute(i);
    }
}

void DMA::execute(int id) {
    DMAChannel& d = ch[id];

    uint32_t count = d.cnt & 0xFFFF;
    if (count == 0)
        count = 0x10000;

    bool word = d.cnt & (1 << 26);

    uint32_t src = d.src;
    uint32_t dst = d.dst;

    for (uint32_t i = 0; i < count; i++) {
        if (word) {
            uint32_t v = mem->read32(src);
            mem->write32(dst, v);
            src += 4;
            dst += 4;
        }
        else {
            uint16_t v = mem->read16(src);
            mem->write16(dst, v);
            src += 2;
            dst += 2;
        }
    }

    d.src = src;
    d.dst = dst;

    d.active = false;
    d.cnt &= ~(1u << 31);
}
