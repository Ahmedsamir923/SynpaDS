#pragma once
#include <cstdint>
#include <cstring>

#include "../dma/dma.h"
#include "../timers/timer.h"

struct IRQ;

struct Memory {

    static constexpr uint32_t BIOS_SIZE = 0x4000;
    static constexpr uint32_t MAIN_RAM_SIZE = 4 * 1024 * 1024;
    static constexpr uint32_t IO_SIZE = 0x1000;

    uint8_t bios[BIOS_SIZE];
    uint8_t mainRAM[MAIN_RAM_SIZE];
    uint8_t io[IO_SIZE];

    Timer timers;
    DMA dma;

    IRQ* irq = nullptr;

    Memory();

    void attachIRQ(IRQ* i) { irq = i; }

    uint8_t  read8(uint32_t addr);
    uint16_t read16(uint32_t addr);
    uint32_t read32(uint32_t addr);

    void write8(uint32_t addr, uint8_t v);
    void write16(uint32_t addr, uint16_t v);
    void write32(uint32_t addr, uint32_t v);
};
