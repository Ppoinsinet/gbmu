#include <interrupts.h>
#include <cpu.h>
#include <ram.h>

void int_handle(unsigned short addr) {
    push_stack(cpu.registers.pc);
    set_reg(REG_PC, addr);
}

char int_check(unsigned short addr, INT_TYPE type) {
    if (cpu.int_flags & type && cpu.ie_register & type) {
        int_handle(addr);
        cpu.int_flags &= ~type;
        cpu.halt = 0;
        cpu.IME = 0;
        return 1;
    }
    return 0;
}

void cpu_handle_interrupts() {
    printf("Before : %d et %d\n", cpu.int_flags, cpu.ie_register);
    if (int_check(0x40, INT_VBLANK)) {

    } else if (int_check(0x48, INT_LCD_STAT)) {

    } else if (int_check(0x50, INT_TIMER)) {
        
    } else if (int_check(0x58, INT_SERIAL)) {
        
    } else if (int_check(0x60, INT_JOYPAD)) {
        
    }
    printf("After : %d et %d\n", cpu.int_flags, cpu.ie_register);
}