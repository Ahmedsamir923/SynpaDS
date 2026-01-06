#include "irq.h"
#include "../arm9/cpu.h"

void IRQ::init(CPU* c) {
    cpu = c;
}

void IRQ::reset() {
    IME = IE = IF = 0;
}

void IRQ::request(uint32_t bit) {
    IF |= bit;
}

void IRQ::step() {
    if (!IME) return;

    uint32_t pending = IE & IF;
    if (!pending) return;

    // limpa flag
    IF &= ~pending;

    // salva retorno
    cpu->R[14] = cpu->PC() + (cpu->getFlag(CPU::T) ? 2 : 4);

    // desabilita IRQ
    IME = 0;

    // força ARM mode
    cpu->setFlag(CPU::T, false);

    // salta para handler
    cpu->PC() = 0x00000018;

    printf("[IRQ] Interrupt triggered\n");
}

uint32_t IRQ::read(uint32_t addr) {
    switch (addr) {
    case 0x04000208: return IME;
    case 0x04000210: return IE;
    case 0x04000214: return IF;
    }
    return 0;
}

void IRQ::write(uint32_t addr, uint32_t v) {
    switch (addr) {
    case 0x04000208: IME = v & 1; break;
    case 0x04000210: IE = v; break;
    case 0x04000214: IF &= ~v; break; // write-1-to-clear
    }
}
