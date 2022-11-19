#include <instructions.h>
#include <dbg.h>
#include <bus.h>

static void process_ld_01(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_02(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_06(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_0A(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_0E(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_40(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_E2(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_F8(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_F9(INSTRUCTION instruction, unsigned char &arg1);
static void process_ld_EA(INSTRUCTION instruction, unsigned char &arg1);

void process_LD(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("LD process\n");

    if (IS_BETWEEN(0x3F, instruction.opcode, 0x80))
        process_ld_40(instruction, arg1);
    else if (instruction.opcode == 0x01 || instruction.opcode == 0x11 || instruction.opcode == 0x21 || instruction.opcode == 0x31)
        process_ld_01(instruction, arg1);
    else if (instruction.opcode == 0x02 || instruction.opcode == 0x12)
        process_ld_02(instruction, arg1);
    else if (instruction.opcode == 0x06 || instruction.opcode == 0x16 || instruction.opcode == 0x26 || instruction.opcode == 0x36)
        process_ld_06(instruction, arg1);
    else if (instruction.opcode == 0x0A || instruction.opcode == 0x1A)
        process_ld_0A(instruction, arg1);
    else if (instruction.opcode == 0x0E || instruction.opcode == 0x1E || instruction.opcode == 0x2E || instruction.opcode == 0x3E)
        process_ld_0E(instruction, arg1);
    else if (instruction.opcode == 0xE2 || instruction.opcode == 0xF2)
        process_ld_E2(instruction, arg1);
    else if (instruction.opcode == 0xEA || instruction.opcode == 0xFA)
        process_ld_EA(instruction, arg1);
    else if (instruction.opcode == 0xF8)
        process_ld_F8(instruction, arg1);
    else if (instruction.opcode == 0xF9) {
        set_reg(REG_SP, read_reg(REG_HL));
        emu_cycle(1);
    }
    else if (instruction.opcode == 0xEA || instruction.opcode == 0xFA)
        process_ld_EA(instruction, arg1);
    else
        ERROR("Error on LD_GLOBAL")


    GO_NEXT_INSTR
}

static void process_ld_EA(INSTRUCTION instruction, unsigned char &arg1) {
    if (instruction.opcode == 0xEA) { // LD (a16),A
        unsigned short nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
        emu_cycle(1);

        write_mem(nn, read_reg(REG_A));
    }
    else if (instruction.opcode == 0xFA) { // LD A,(a16)
        unsigned short nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
        emu_cycle(1);

        set_reg(REG_A, read_mem(nn));
    }
    else
        ERROR("Error on LD_EA")
    emu_cycle(1);
}

static void process_ld_F8(INSTRUCTION instruction, unsigned char &arg1) {
    if (instruction.opcode == 0xF8) { // LD HL,SP+r8
        char nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);

        set_reg(REG_HL, read_reg(REG_SP) + nn);
        emu_cycle(1);
    }
    else
        ERROR("Error on LD_F8")
}

static void process_ld_E2(INSTRUCTION instruction, unsigned char &arg1) {
    if (instruction.opcode == 0xE2) { // LD (C),A
        write_mem(0xFF00 + read_reg(REG_C), read_reg(REG_A));
    }
    else if (instruction.opcode == 0xF2) { // LD A,(C)
        set_reg(REG_A, read_mem(0xFF00 + read_reg(REG_C)));
    }
    else
        ERROR("Error on LD_E2")
    emu_cycle(1);
}

static void process_ld_08(INSTRUCTION instruction, unsigned char &arg1) {
    unsigned short nn = read_mem(read_reg(REG_PC) + 1);
    emu_cycle(1);
    nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
    emu_cycle(1);

    write_mem(nn, read_reg(REG_SP) & 0xFF);
    emu_cycle(1);
    write_mem(nn + 1, ((read_reg(REG_SP) & 0xFF00) >> 8));
    emu_cycle(1);
}

static void process_ld_0E(INSTRUCTION instruction, unsigned char &arg1) {
    unsigned short nn = read_mem(read_reg(REG_PC) + 1);
    emu_cycle(1);
    switch (instruction.opcode & 0xF0)
    {
    case 0x00:
        set_reg(REG_C, nn);
        break;
    case 0x10:
        set_reg(REG_E, nn);
        break;
    case 0x20:
        set_reg(REG_L, nn);
        break;
    case 0x30:
        set_reg(REG_A, nn);
        break;
    
    default:
        ERROR("Error on LD_0E")
    }
}

static void process_ld_0A(INSTRUCTION instruction, unsigned char &arg1) {
    if (instruction.opcode == 0x0A)
        set_reg(REG_A, read_mem(read_reg(REG_BC)));
    else if (instruction.opcode == 0x1A)
        set_reg(REG_A, read_mem(read_reg(REG_DE)));
    emu_cycle(1);
}

static void process_ld_06(INSTRUCTION instruction, unsigned char &arg1) {
    unsigned short nn = read_mem(read_reg(REG_PC) + 1);
    emu_cycle(1);

    switch (instruction.opcode & 0xF0)
    {
    case 0x00:
        set_reg(REG_B, nn);
        break;
    case 0x10:
        set_reg(REG_D, nn);
        break;
    case 0x20:
        set_reg(REG_H, nn);
        break;
    case 0x30:
        write_mem(read_reg(REG_HL), nn);
        break;
    default:
        ERROR("Error on LD_06")
    }
}

static void process_ld_01(INSTRUCTION instruction, unsigned char &arg1) {
    unsigned short nn = read_mem(read_reg(REG_PC) + 1);
    emu_cycle(1);
    nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
    emu_cycle(1);

    switch (instruction.opcode & 0xF0)
    {
    case 0x00:
        set_reg(REG_BC, nn);
        break;
    case 0x10:
        set_reg(REG_DE, nn);
        break;
    case 0x20:
        set_reg(REG_HL, nn);
        break;
    case 0x30:
        set_reg(REG_SP, nn);
        break;
    default:
        ERROR("Error on LD_01")
    }
}

static void process_ld_02(INSTRUCTION instruction, unsigned char &arg1) {
    if (instruction.opcode == 0x02)
        write_mem(read_reg(REG_BC), read_reg(REG_A));
    else if (instruction.opcode == 0x12)
        write_mem(read_reg(REG_DE), read_reg(REG_A));
    else
        ERROR("Error on LD_02")
    emu_cycle(1);
}

static void process_ld_40(INSTRUCTION instruction, unsigned char &arg1) {
    unsigned short fetched;

    switch (instruction.opcode & 0x0F)
    {
    case 0x00:
    case 0x08:
        fetched = read_reg(REG_B);
        break;
    case 0x01:
    case 0x09:
        fetched = read_reg(REG_C);
        break;
    case 0x02:
    case 0x0A:
        fetched = read_reg(REG_D);
        break;
    case 0x03:
    case 0x0B:
        fetched = read_reg(REG_E);
        break;
    case 0x04:
    case 0x0C:
        fetched = read_reg(REG_H);
        break;
    case 0x05:
    case 0x0D:
        fetched = read_reg(REG_L);
        break;
    case 0x06:
    case 0x0E:
        fetched = read_mem(read_reg(REG_HL));
        break;
    case 0x07:
    case 0x0F:
        fetched = read_reg(REG_A);
        break;    
    default:
        break;
    }

    emu_cycle(1);

    if (IS_BETWEEN(0x40, instruction.opcode, 0x47)) // between 0x40 and 0x47
        set_reg(REG_B, fetched);
    else if (IS_BETWEEN(0x48, instruction.opcode, 0x4F)) // between 0x48 and 0x4F
        set_reg(REG_C, fetched);
    
    else if (IS_BETWEEN(0x50, instruction.opcode, 0x57)) // between 0x50 and 0x57
        set_reg(REG_D, fetched);
    else if (IS_BETWEEN(0x58, instruction.opcode, 0x5F)) // between 0x58 and 0x5F
        set_reg(REG_E, fetched);

    else if (IS_BETWEEN(0x60, instruction.opcode, 0x67)) // between 0x60 and 0x67
        set_reg(REG_H, fetched);
    else if (IS_BETWEEN(0x68, instruction.opcode, 0x6F)) // between 0x68 and 0x6F
        set_reg(REG_L, fetched);

    else if (IS_BETWEEN(0x70, instruction.opcode, 0x77)) // between 0x70 and 0x77
        write_mem(read_reg(REG_HL), fetched);
    else if (IS_BETWEEN(0x78, instruction.opcode, 0x7F)) // between 0x78 and 0x7F
        set_reg(REG_A, fetched);
    
}