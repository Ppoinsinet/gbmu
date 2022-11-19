#include <ram.h>
#include <cpu.h>
#include <bus.h>

RAM ram;

void push_stack(unsigned short data) {
    set_reg(REG_SP, read_reg(REG_SP) - 1);
    write_mem(read_reg(REG_SP), (data & 0xFF00) >> 8);
    emu_cycle(1);

    set_reg(REG_SP, read_reg(REG_SP) - 1);
    write_mem(read_reg(REG_SP), data & 0x00FF);
    emu_cycle(1);

}


//todo emu cycles
unsigned short pop_stack() {
    if (read_reg(REG_SP) >= 0xDFFE)
        ERROR("Stack pointer greater than 0xDFFE");

    unsigned short ret = read_mem(read_reg(REG_SP));
    set_reg(REG_SP, read_reg(REG_SP) + 1);
    emu_cycle(1);

    ret |= (read_mem(read_reg(REG_SP)) << 8);
    set_reg(REG_SP, read_reg(REG_SP) + 1);
    emu_cycle(1);
    

    return ret;
}