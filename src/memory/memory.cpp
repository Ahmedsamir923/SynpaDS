#include "../memory/memory.h"
#include "../core/arm9/irq.h"
#include "../timers/timer.h"

Memory::Memory() {
    memset(bios, 0, sizeof(bios));
    memset(mainRAM, 0, sizeof(mainRAM));
    memset(io, 0, sizeof(io));
    for (auto& t : timers) t.reset();
}

// ---------------- READ ----------------

uint8_t Memory::read8(uint32_t addr) {

    if (addr < BIOS_SIZE)
        return bios[addr];

    if (addr >= 0x02000000 && addr < 0x02000000 + MAIN_RAM_SIZE)
        return mainRAM[addr - 0x02000000];

    if (addr >= 0x04000000 && addr < 0x04001000)
        return io[addr - 0x04000000];

    return 0;
}

uint16_t Memory::read16(uint32_t addr) {

    // TIMERS (0x04000100 - 0x0400010F)
    if (addr >= 0x04000100 && addr <= 0x0400010F) {
        int id = (addr - 0x04000100) / 4;
        bool high = addr & 2;

        if (high)
            return timers.readCNT_H();
        else
            return timers.readCNT_L();
    }
}

uint32_t Memory::read32(uint32_t addr) {

    if (irq && addr >= 0x04000208 && addr <= 0x04000214)
        return irq->read(addr);

    if (addr >= 0x040000B0 && addr <= 0x040000DF) {
        int id = (addr - 0x040000B0) / 12;
        int reg = ((addr - 0x040000B0) % 12) / 4;
        return dma.read(id, reg);
    }

    return read16(addr) | (read16(addr + 2) << 16);
}

// ---------------- WRITE ----------------

void Memory::write8(uint32_t addr, uint8_t v) {

    if (addr >= 0x02000000 && addr < 0x02000000 + MAIN_RAM_SIZE) {
        mainRAM[addr - 0x02000000] = v;
        return;
    }

    if (addr >= 0x04000000 && addr < 0x04001000)
        io[addr - 0x04000000] = v;
}

void Memory::write16(uint32_t addr, uint16_t v) {

    if (addr >= 0x04000100 && addr <= 0x0400010F) {
        int id = (addr - 0x04000100) / 4;
        bool high = addr & 2;
        if (high) timers[id].writeCNT_H(v);
        else timers[id].writeCNT_L(v);
        return;
    }

    write8(addr, v & 0xFF);
    write8(addr + 1, v >> 8);
}

void Memory::write32(uint32_t addr, uint32_t v) {

    if (irq && addr >= 0x04000208 && addr <= 0x04000214) {
        irq->write(addr, v);
        return;
    }

    if (addr >= 0x040000B0 && addr <= 0x040000DF) {
        int id = (addr - 0x040000B0) / 12;
        int reg = ((addr - 0x040000B0) % 12) / 4;
        dma.write(id, reg, v);
        return;
    }

    write16(addr, v & 0xFFFF);
    write16(addr + 2, v >> 16);
}
