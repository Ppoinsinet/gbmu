#include <bus.h>
#include <ram.h>
#include <cpu.h>
#include <io.h>
#include <dbg.h>

#include <stdio.h>

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

unsigned char &read_mem(unsigned short addr) {
    // if (enable_print) printf("Reading : 0x%hhX\n", addr);

    if (addr < 0x8000) {
        //ROM data
        return ram.data[addr];
    } else if (addr < 0x9800) {
        //CHR RAM
        return ram.data[addr];
    } else if (addr < 0x9C00) {
        //BG Map 1
        return ram.data[addr];
    } else if (addr < 0xA000) {
        //BG Map 2
        return ram.data[addr];
    } else if (addr < 0xC000) {
        //Cartridge RAM
        return ram.data[addr];
    } else if (addr < 0xD000) {
        //RAM bank 0
        return ram.data[addr];
    } else if (addr < 0xE000) {
        //RAM bank 1-7
        return ram.data[addr];
    } else if (addr < 0xFE00) {
        //Reserved - Echo RAM
        return ram.data[addr - 0x2000];
    } else if (addr < 0xFEA0) {
        //OAM (Object Attribute Memory)
        return ram.data[addr];
    } else if (addr < 0xFF00) {
        //Reserved - Unusable
        NO_IMPL
    } else if (addr < 0xFF80) {
        //I/O Registers
        return read_io(addr);
    } else if (addr < 0xFFFF) {
        //Zero page -- HRAM
        return ram.data[addr];
    }
    else if (addr == 0xFFFF) {
        return cpu.ie_register;
    }
    else
        ERROR("Error on read_mem")
}

void write_mem(unsigned short addr, unsigned char val) {
    // if (enable_print) printf("Writing mem : (0x%04X) = %02hhX\n", addr, val);

    if (addr < 0x8000) {
        //ROM data
        printf("Cannot write on ROM cartridge\n");
        // ERROR("Cannot write on ROM cartridge")
    } else if (addr < 0x9800) {
        //CHR RAM
        ram.data[addr] = val;
    } else if (addr < 0x9C00) {
        //BG Map 1
        ram.data[addr] = val;
    } else if (addr < 0xA000) {
        //BG Map 2
        ram.data[addr] = val;
    } else if (addr < 0xC000) {
        //Cartridge RAM
        ram.data[addr] = val;
    } else if (addr < 0xD000) {
        //RAM bank 0
        ram.data[addr] = val;
    } else if (addr < 0xE000) {
        //RAM bank 1-7
        ram.data[addr] = val;
    } else if (addr < 0xFE00) {
        //Reserved - Echo RAM
        NO_IMPL
    } else if (addr < 0xFEA0) {
        //OAM (Object Attribute Memory)
        // NO_IMPL
    } else if (addr < 0xFF00) {
        //Reserved - Unusable
        // NO_IMPL
    } else if (addr < 0xFF80) {
        //I/O Registers
        write_io(addr, val);
    } else if (addr < 0xFFFF) {
        //Zero page -- High RAM
        ram.data[addr] = val;
    }
    else if (addr == 0xFFFF) {
        cpu.ie_register = val;
    }
    else
        ERROR("Error on write_mem")





}

unsigned short read_mem16(unsigned short addr) {
    return *(unsigned short*)&ram.data[addr];
}

void write_mem16(unsigned short addr, unsigned short val) {
    // if (enable_print) printf("Writing mem : (0x%04X) = %02hX\n", addr, val);
    ram.data[addr] = (val & 0x00FF);
    ram.data[addr + 1] = (val & 0xFF00) >> 8;
}