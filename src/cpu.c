#include <cpu.h>
#include <timer.h>
#include <dbg.h>

CPU cpu = { .registers.af = {0x80, 0x11}, .registers.bc = {0x00, 0x00}, .registers.de = {0x56, 0xFF}, .registers.hl = {0x0D, 0x00} , .registers.sp = 0xFFFE, .enabling_IME = 0 , .halt = 0 };

void cpu_set_flag(char condition, int flag) {
    if (flag != FLAG_C && flag != FLAG_H && flag != FLAG_N && flag != FLAG_Z) {
        ERROR("Wrong flag was modified");
    }
    if (condition)
        cpu.registers.flags |= (1 << flag);
    else
        cpu.registers.flags &= ~(1 << flag);
}

int cpu_get_flag(unsigned char flag) {
    if (flag > 7) {
        printf("Wrong flag read\n");
        exit(5);
    }
    return (cpu.registers.flags & (1 << flag)) != 0 ? 1 : 0;
}

unsigned short read_reg(int reg) {
    switch (reg)
    {
    case REG_A:
        return (unsigned short)cpu.registers.a;
    case REG_F:
        return (unsigned short)cpu.registers.flags;
    case REG_AF:
        return *(unsigned short*)cpu.registers.af;
    case REG_B:
        return (unsigned short)cpu.registers.b;
    case REG_C:
        return (unsigned short)cpu.registers.c;
    case REG_BC:
        return *(unsigned short*)cpu.registers.bc;
    case REG_D:
        return (unsigned short)cpu.registers.d;
    case REG_E:
        return (unsigned short)cpu.registers.e;
    case REG_DE:
        return *(unsigned short*)cpu.registers.de;
    case REG_H:
        return (unsigned short)cpu.registers.h;
    case REG_L:
        return (unsigned short)cpu.registers.l;
    case REG_HL:
        return *(unsigned short*)cpu.registers.hl;
    case REG_SP:
        return cpu.registers.sp;
    case REG_PC:
        return cpu.registers.pc;
    
    default:
        printf("Error on read_reg : %d\n", reg);
        exit(4);
    }
}

void set_reg(int reg, unsigned short val) {
    switch (reg)
    {
    case REG_A:
        if (enable_print) printf("Setting REG_A = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.a = (unsigned char)(val & 0x00FF);
        break;
    case REG_F:
        if (enable_print) printf("Setting REG_F = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.flags = (unsigned char)(val & 0x00FF);
        break;
    case REG_AF:
        if (enable_print) printf("Setting REG_AF = %02hX\n", val);
        cpu.registers.af[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.af[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_B:
        if (enable_print) printf("Setting REG_B = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.b = (unsigned char)(val & 0x00FF);
        break;
    case REG_C:
        if (enable_print) printf("Setting REG_C = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.c = (unsigned char)(val & 0x00FF);
        break;
    case REG_BC:
        if (enable_print) printf("Setting REG_A = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.bc[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.bc[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_D:
        if (enable_print) printf("Setting REG_D = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.d = (unsigned char)(val & 0x00FF);
        break;
    case REG_E:
        if (enable_print) printf("Setting REG_E = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.e = (unsigned char)(val & 0x00FF);
        break;
    case REG_DE:
        if (enable_print) printf("Setting REG_DE = %02hX\n", val);
        cpu.registers.de[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.de[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_H:
        if (enable_print) printf("Setting REG_H = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.h = (unsigned char)(val & 0x00FF);
        break;
    case REG_L:
        if (enable_print) printf("Setting REG_L = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.l = (unsigned char)(val & 0x00FF);
        break;
    case REG_HL:
        if (enable_print) printf("Setting REG_HL = %02hX\n", val);
        cpu.registers.hl[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.hl[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_SP:
        if (enable_print) printf("Setting REG_SP = %02hX\n", val);
        cpu.registers.sp = val;
        break;
    case REG_PC:
        // if (enable_print) printf("Setting REG_PC = %02hX\n", val);
        cpu.registers.pc = val;
        break;
    
    default:
        ERROR("Set_reg");
    }
}

void request_interrupt(int type) {
    cpu.int_flags |= type;
}

void emu_cycle(unsigned short n) {

    //todo
    int tmp = n * 4;

    for (int i = 0 ; i < tmp; tmp++) {
        cpu.ticks++;
        timer_tick();
    }
}