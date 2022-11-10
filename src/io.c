#include <io.h>
#include <timer.h>
#include <cpu.h>
#include <ram.h>

static unsigned char serial[2];

unsigned char io_data[0xFF] = {
    [0x44] = 0x90
};

unsigned char read_io(unsigned short addr) {
    if (addr == 0xFF01)
        return serial[0];
    if (addr == 0xFF02)
        return serial[1];
    if (IS_BETWEEN(0xFF04, addr, 0xFF07))
        return timer_read(addr);
    if (addr == 0xFF0F)
        return cpu.int_flags;
    if (addr == 0xFF44)
        return io_data[0x44]++;
    return io_data[addr - 0xFF00];
    ERROR("Error on read_io");
}

void write_io(unsigned short addr, unsigned char val) {
    if (addr == 0xFF01) {
        serial[0] = val;
        return;
    }
    else if (addr == 0xFF02) {
        serial[1] = val;
        return ;
    }
    else if (IS_BETWEEN(0xFF04, addr, 0xFF07)) {
        timer_write(addr, val);
        return;
    }
    else if (addr == 0xFF0F) {
        cpu.int_flags = val;
        return ;
    }

    io_data[addr - 0xFF00] = val;
    return ;
}