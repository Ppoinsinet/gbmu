#include <ram.h>
#include <cpu.h>
#include <bus.h>

RAM ram;

//todo emu cycles
void push_stack(unsigned short data) {
    set_reg(REG_SP, read_reg(REG_SP) - 2);

    write_mem16(read_reg(REG_SP), data);
}


//todo emu cycles
unsigned short pop_stack() {
    if (read_reg(REG_SP) >= 0xDFFE)
        ERROR("Stack pointer greater than 0xDFFE");
    unsigned short ret = read_mem16(read_reg(REG_SP));
    set_reg(REG_SP, read_reg(REG_SP) + 2);
    return ret;
}