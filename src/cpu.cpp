#include <cpu.h>
#include <timer.h>
#include <dbg.h>

CPU cpu;

void init_cpu() {
    cpu.registers.pc = 0x0100;
    cpu.registers.sp = 0xFFFE;
    cpu.registers.reg_a = 0x01;
    cpu.registers.reg_flags = 0xB0;

    cpu.registers.reg_b = 0x00;
    cpu.registers.reg_c = 0x13;

    cpu.registers.reg_d = 0x00;
    cpu.registers.reg_e = 0xD8;

    cpu.registers.reg_h = 0x01;
    cpu.registers.reg_l = 0x4D;
    

    cpu.ie_register = 0;
    cpu.int_flags = 0;
    cpu.IME = 0;
    cpu.enabling_IME = 0;  
}

void cpu_set_flag(char condition, int flag) {
    if (flag != FLAG_C && flag != FLAG_H && flag != FLAG_N && flag != FLAG_Z) {
        ERROR("Wrong flag was modified");
    }
    if (condition)
        cpu.registers.reg_flags |= (1 << flag);
    else
        cpu.registers.reg_flags &= ~(1 << flag);
}

int cpu_get_flag(unsigned char flag) {
    if (flag > 7) {
        fprintf(stderr, "Wrong flag read\n");
    }
    return (cpu.registers.reg_flags & (1 << flag)) != 0 ? 1 : 0;
}

unsigned short read_reg(int reg) {
    switch (reg)
    {
    case REG_A:
        return (unsigned short)cpu.registers.reg_a;
    case REG_F:
        return (unsigned short)cpu.registers.reg_flags;
    case REG_AF:
        return *(unsigned short*)cpu.registers.af;
    case REG_B:
        return (unsigned short)cpu.registers.reg_b;
    case REG_C:
        return (unsigned short)cpu.registers.reg_c;
    case REG_BC:
        return *(unsigned short*)cpu.registers.bc;
    case REG_D:
        return (unsigned short)cpu.registers.reg_d;
    case REG_E:
        return (unsigned short)cpu.registers.reg_e;
    case REG_DE:
        return *(unsigned short*)cpu.registers.de;
    case REG_H:
        return (unsigned short)cpu.registers.reg_h;
    case REG_L:
        return (unsigned short)cpu.registers.reg_l;
    case REG_HL:
        return *(unsigned short*)cpu.registers.hl;
    case REG_SP:
        return cpu.registers.sp;
    case REG_PC:
        return cpu.registers.pc;
    
    default:
        fprintf(stderr, "Error on read_reg : %d\n", reg);
        exit(2);
    }
}

void set_reg(int reg, unsigned short val) {
    switch (reg)
    {
    case REG_A:
        // if (enable_print) printf("Setting REG_A = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_a = (unsigned char)(val & 0x00FF);
        break;
    case REG_F:
        // if (enable_print) printf("Setting REG_F = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_flags = (unsigned char)(val & 0x00FF);
        break;
    case REG_AF:
        // if (enable_print) printf("Setting REG_AF = %02hX\n", val);
        cpu.registers.af[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.af[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_B:
        // if (enable_print) printf("Setting REG_B = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_b = (unsigned char)(val & 0x00FF);
        break;
    case REG_C:
        // if (enable_print) printf("Setting REG_C = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_c = (unsigned char)(val & 0x00FF);
        break;
    case REG_BC:
        // if (enable_print) printf("Setting REG_BC = %02hX\n", val);
        cpu.registers.bc[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.bc[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_D:
        // if (enable_print) printf("Setting REG_D = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_d = (unsigned char)(val & 0x00FF);
        break;
    case REG_E:
        // if (enable_print) printf("Setting REG_E = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_e = (unsigned char)(val & 0x00FF);
        break;
    case REG_DE:
        // if (enable_print) printf("Setting REG_DE = %02hX\n", val);
        cpu.registers.de[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.de[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_H:
        // if (enable_print) printf("Setting REG_H = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_h = (unsigned char)(val & 0x00FF);
        break;
    case REG_L:
        // if (enable_print) printf("Setting REG_L = %02hhX\n", (unsigned char)(val & 0x00FF));
        cpu.registers.reg_l = (unsigned char)(val & 0x00FF);
        break;
    case REG_HL:
        // if (enable_print) printf("Setting REG_HL = %02hX\n", val);
        cpu.registers.hl[0] = (unsigned char)(val & 0x00FF);
        cpu.registers.hl[1] = (unsigned char)((val & 0xFF00) >> 8);
        break;
    case REG_SP:
        // if (enable_print) printf("Setting REG_SP = %02hX\n", val);
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

    for (int i = 0 ; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            cpu.ticks++;
            timer.tick();

        }
    }
}