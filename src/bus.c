#include <bus.h>
#include <ram.h>
#include <cpu.h>
#include <io.h>
#include <dbg.h>

#include <stdio.h>

unsigned char read_mem(unsigned short addr) {
    if (addr == 0xFFFF) {
        return cpu.ie_register;
    }
    else if (IS_BETWEEN(0xFF00, addr, 0xFF7F)) {
        //IO registers
        return read_io(addr);
    } 
    else {

        return ram.data[addr];
    }


}

void write_mem(unsigned short addr, unsigned char val) {
    if (enable_print) printf("Writing mem : (0x%04X) = %02hhX\n", addr, val);


    if (addr == 0xFFFF) {
        cpu.ie_register = val;
    }
    else if (IS_BETWEEN(0xFF00, addr, 0xFF7F)) {
        //IO registers
        return write_io(addr, val);
    } 
    else {

        ram.data[addr] = val;
    }




}

unsigned short read_mem16(unsigned short addr) {
    return *(unsigned short*)&ram.data[addr];
}

void write_mem16(unsigned short addr, unsigned short val) {
    if (enable_print) printf("Writing mem : (0x%04X) = %02hX\n", addr, val);
    ram.data[addr] = (val & 0x00FF);
    ram.data[addr + 1] = (val & 0xFF00) >> 8;
}