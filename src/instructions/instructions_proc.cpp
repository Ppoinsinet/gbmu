#include <instructions.h>
#include <cpu.h>
#include <cartridge.h>
#include <bus.h>
#include <dbg.h>
#include <map>

static std::map<INSTR_TYPE, INSTR_FUNCTION> map_process = {
    {INSTR_NOP, process_NOP},
    {INSTR_JP, process_JP},
    {INSTR_XOR, process_XOR},
    {INSTR_LD, process_LD},
    {INSTR_INC, process_INC},
    {INSTR_DEC, process_DEC},
    {INSTR_LDD, process_LDD},
    {INSTR_LDI, process_LDI},
    {INSTR_JR, process_JR},
    {INSTR_EI, process_EI},
    {INSTR_DI, process_DI},
    {INSTR_LDH, process_LDH},
    {INSTR_CP, process_CP},
    {INSTR_CALL, process_CALL},
    {INSTR_OR, process_OR},
    {INSTR_RET, process_RET},
    {INSTR_RETI, process_RETI},
    {INSTR_POP, process_POP},
    {INSTR_PUSH, process_PUSH},
    {INSTR_CPL, process_CPL},
    {INSTR_AND, process_AND},
    {INSTR_CB, process_CB},
    {INSTR_RST, process_RST},
    {INSTR_ADD, process_ADD},
    {INSTR_HALT, process_HALT},
    {INSTR_RLA, process_RLA},
    {INSTR_RLCA, process_RLCA},
    {INSTR_CCF, process_CCF},
    {INSTR_SCF, process_SCF},
    {INSTR_ADC, process_ADC},
    {INSTR_SUB, process_SUB},
    {INSTR_STOP, process_STOP},
    {INSTR_DAA, process_DAA},
    {INSTR_RRA, process_RRA},
    {INSTR_SBC, process_SBC}
};

const INSTR_FUNCTION map_cb_process[] = {
    [INSTR_RLC] = process_RLC,
    [INSTR_RRC] = process_RRC,
    [INSTR_RL] = process_RL,
    [INSTR_RR] = process_RR,
    [INSTR_SLA] = process_SLA,
    [INSTR_SRA] = process_SRA,
    [INSTR_SWAP] = process_SWAP,
    [INSTR_SRL] = process_SRL,
    [INSTR_BIT] = process_BIT,
    [INSTR_RES] = process_RES,
    [INSTR_RST] = process_RST,
    [INSTR_SET] = process_SET
};

void execute_process(INSTRUCTION instruction, unsigned char &arg1) {
    map_process[instruction.instruction](instruction, arg1);
}

void process_RST(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RST process\n");

    emu_cycle(1);
    push_stack(read_reg(REG_PC) + instruction.size);

    set_reg(REG_PC, instruction.param);

}


void process_CB(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB prefix process : %hhX\n", read_mem(read_reg(REG_PC) + 1));

    GO_NEXT_INSTR
    const INSTRUCTION next_instr = getCBInstruction(read_mem(read_reg(REG_PC)));
    emu_cycle(1);
    if (map_cb_process[next_instr.instruction]) {
        map_cb_process[next_instr.instruction](next_instr, getFirstArg(next_instr));
        emu_cycle(1);
    }
}

void process_PUSH(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("PUSH process\n");

    switch (instruction.opcode & 0xF0)
    {
    case 0xC0:
        push_stack(read_reg(REG_BC));
        break;
    case 0xD0:
        push_stack(read_reg(REG_DE));
        break;
    case 0xE0:
        push_stack(read_reg(REG_HL));
        break;
    case 0xF0:
        push_stack(read_reg(REG_AF));
        break;
    
    default:
        ERROR("Error on PUSH")
    }

    GO_NEXT_INSTR
}

void process_POP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("POP process\n");

    unsigned short ret = pop_stack();
    
    switch (instruction.r1)
    {
    case REG_AF:
        set_reg(REG_AF, ret);
        break;
    case REG_BC:
        set_reg(REG_BC, ret);
        break;
    case REG_DE:
        set_reg(REG_DE, ret);
        break;
    case REG_HL:
        set_reg(REG_HL, ret);
        break;
    
    default:
        ERROR("Wrong POP parameter");
    }

    GO_NEXT_INSTR
}


void process_RETI(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RETI process\n");

    unsigned short nn = pop_stack();
    set_reg(REG_PC, nn);
    emu_cycle(1);
    cpu.IME = 1;
}

void process_RET(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RET process\n");

    switch (instruction.opcode)
    {
    case 0xC9: {

        unsigned short nn = pop_stack();
        set_reg(REG_PC, nn);
        emu_cycle(1);
    } break;
    case 0xC0:
    case 0xD0:
    case 0xC8:
    case 0xD8: {

        emu_cycle(1);
        if (!valid_conditions(instruction)) {
            GO_NEXT_INSTR
            return ;
        }
        unsigned short nn = pop_stack();
        set_reg(REG_PC, nn);
        emu_cycle(1);
    } break;
    default:
        ERROR("Error on RET")
    }
}

void process_STOP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("TODO: STOP process\n");

    NO_IMPL
    GO_NEXT_INSTR
}

void process_CALL(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CALL process\n");

    switch (instruction.opcode)
    {
    case 0xCD: {

        unsigned short nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
        emu_cycle(2);

        push_stack(read_reg(REG_PC) + instruction.size);

        set_reg(REG_PC, nn);
    } break;
    case 0xC4:
    case 0xD4:
    case 0xCC:
    case 0xDC: {
        unsigned short nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
        emu_cycle(1);
        printf("lol1\n");
        if (!valid_conditions(instruction)) {
            GO_NEXT_INSTR
            return;
        }
        printf("lol\n");
        push_stack(read_reg(REG_PC) + instruction.size);
        set_reg(REG_PC, nn);
    } break;
    default:
        ERROR("Error avec CALL")
    }

}

void process_CP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CP process\n");

    unsigned char tmp = 0;
    switch (instruction.r1)
    {
    case REG_A:
        tmp = read_reg(REG_A);
        break;
    case REG_B:
        tmp = read_reg(REG_B);
        break;
    case REG_C:
        tmp = read_reg(REG_C);
        break;
    case REG_D:
        tmp = read_reg(REG_D);
        break;
    case REG_E:
        tmp = read_reg(REG_E);
        break;
    case REG_H:
        tmp = read_reg(REG_H);
        break;
    case REG_L:
        tmp = read_reg(REG_L);
        break;
    case REG_HL:
        tmp = read_mem(read_reg(REG_HL));
        break;
    case MEM_D8:
        tmp = read_mem(read_reg(REG_PC) + 1);
        break;
    default:
        ERROR("Wrong call to CP");
    }

    int val = read_reg(REG_A) - 90;
    cpu_set_flag(val == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((tmp & 0x0F) - (read_reg(REG_A) & 0x0F) < 0, FLAG_H);
    cpu_set_flag(val < 0, FLAG_C);
    
    

    //TODO finir implement flags

    GO_NEXT_INSTR

}

void process_LDH(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("LDH process\n");

    switch (instruction.opcode)
    {
    case 0xF2: { // LDH A, (C)
        unsigned char nn = read_mem(0xFF00 + read_reg(REG_C));
        emu_cycle(1);
        set_reg(REG_A, nn);
    } break;
    case 0xE2: { // LDH (C), A
        unsigned char nn = read_reg(REG_A);
        emu_cycle(1);
        write_mem(0xFF00 + read_reg(REG_C), nn);
    } break;
    case 0xF0: {// LDH A, (n)
        unsigned char nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        set_reg(REG_A, read_mem(0xFF00 + nn));
    } break;
    case 0xE0: { // LDH (n), A
        unsigned char nn = read_mem(read_reg(REG_PC) + 1);
        emu_cycle(1);
        write_mem(0xFF00 + nn, read_reg(REG_A));
    } break;
    
    default:
        ERROR("Error on LDH")
    }
    GO_NEXT_INSTR
}

void process_EI(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("EI process\n");

    cpu.enabling_IME = 1;
    GO_NEXT_INSTR
}

void process_DI(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("DI process\n");

    cpu.IME = 0;
    GO_NEXT_INSTR
}

void process_JR(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("JR process\n");
    
    switch (instruction.opcode)
    {
    case 0x18: {

        char offset = (char)read_mem(read_reg(REG_PC) + 1);
        fprintf(stderr, "allo %hhd pour %hhu\n", offset, read_mem(read_reg(REG_PC) + 1));
        emu_cycle(1);
        set_reg(REG_PC, read_reg(REG_PC) + instruction.size + offset);
        emu_cycle(1);
    } break;
    case 0x20:
    case 0x30:
    case 0x28:
    case 0x38: {

        char offset = (char)read_mem(read_reg(REG_PC) + 1);
        fprintf(stderr, "allo %hhd pour %hhu\n", offset, read_mem(read_reg(REG_PC) + 1));
        emu_cycle(1);
        if (!valid_conditions(instruction)) {
            GO_NEXT_INSTR
            return;
        }
        set_reg(REG_PC, read_reg(REG_PC) + instruction.size + offset);
        emu_cycle(1);
    } break;
    
    default:
        ERROR("Probleme avec JR");
        break;
    }
}

void process_LDD(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("LDD process\n");

    switch (instruction.opcode)
    {
    case 0x32: { // LD (HL-), A
        unsigned short nn = read_reg(REG_A);
        emu_cycle(1);
        write_mem(read_reg(REG_HL), nn);
        set_reg(REG_HL, read_reg(REG_HL) - 1);
    } break;
    case 0x3A: { // LD A, (HL-)
        unsigned short nn = read_reg(REG_HL);
        emu_cycle(1);
        set_reg(REG_A, read_mem(nn));
        set_reg(REG_HL, read_reg(REG_HL) - 1);
    } break;
    
    default:
        ERROR("Error on LDD")
    }
    GO_NEXT_INSTR

}

void process_LDI(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("LDI process\n");

    switch (instruction.opcode)
    {
    case 0x22: { // LD (HL+), A
        unsigned short nn = read_reg(REG_A);
        emu_cycle(1);
        write_mem(read_reg(REG_HL), nn);
        set_reg(REG_HL, read_reg(REG_HL) + 1);
    } break;
    case 0x2A: { // LD A, (HL+)
        unsigned short nn = read_reg(REG_HL);
        emu_cycle(1);
        set_reg(REG_A, read_mem(nn));
        set_reg(REG_HL, read_reg(REG_HL) + 1);
    } break;
    
    default:
        ERROR("Error on LDI")
    }
    GO_NEXT_INSTR
}

void process_ADC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("ADC process\n");

    int a = read_reg(REG_A);
    int flagc = cpu_get_flag(FLAG_C);
    int val = a + flagc + arg1;
    int half = (a & 0x0F) + (arg1 & 0x0F) + (flagc ? 1 : 0);

    cpu_set_flag((val & 0xFF) != val, FLAG_C);
    cpu_set_flag(val == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(half > 0x0F, FLAG_H);

    set_reg(REG_A, val);
    GO_NEXT_INSTR
}

void process_SUB(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("SUB process\n");

    unsigned char old = read_reg(REG_A);
    unsigned char newVal;

    switch (instruction.opcode)
    {
    case 0x90:
        newVal = read_reg(REG_B);
        break;
    case 0x91:
        newVal = read_reg(REG_C);
        break;
    case 0x92:
        newVal = read_reg(REG_D);
        break;
    case 0x93:
        newVal = read_reg(REG_E);
        break;
    case 0x94:
        newVal = read_reg(REG_H);
        break;
    case 0x95:
        newVal = read_reg(REG_L);
        break;
    case 0x96:
        newVal = read_mem(read_reg(REG_HL));
        break;
    case 0x97:
        newVal = old;
        break;
    case 0xD6:
        newVal = read_mem(read_reg(REG_PC) + 1);
        break;
    
    default:
        ERROR("Error on SUB")
    }

    set_reg(REG_A, old - newVal);

    //todo check flags
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((old & 0xF) + (newVal & 0xF) > 0xF, FLAG_H);
    cpu_set_flag((old & 0xFF) + (newVal & 0xFF) > 0xFF, FLAG_C);
    GO_NEXT_INSTR
}

void process_SBC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("SBC process\n");

    unsigned char old = read_reg(REG_A);
    unsigned char newVal = arg1;
    set_reg(REG_A, read_reg(REG_A) - arg1 + cpu_get_flag(FLAG_C));
    

    //todo check flags
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((old & 0xF) - (newVal & 0xF) > 0xF, FLAG_H);
    cpu_set_flag((old & 0xFF) - (newVal & 0xFF) > 0xFF, FLAG_C);
    GO_NEXT_INSTR
}



void process_ADD(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("ADD process\n");

    //TODO set flags

    if (instruction.opcode == 0x09 || instruction.opcode == 0x19 || instruction.opcode == 0x29 || instruction.opcode == 0x39) {
        unsigned short old = read_reg(REG_HL);
        unsigned short newVal;

        switch (instruction.opcode)
        {
        case 0x09:
            newVal = read_reg(REG_BC);
            set_reg(REG_HL, newVal + old);
            break;
        case 0x19:
            newVal = read_reg(REG_DE);
            set_reg(REG_HL, newVal + old);
            break;
        case 0x29:
            newVal = read_reg(REG_HL);
            set_reg(REG_HL, newVal + old);
            break;
        case 0x39:
            newVal = read_reg(REG_SP);
            set_reg(REG_HL, newVal + old);
            break;
        default:
            ERROR("Error on ADD")
        }
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (newVal & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (newVal & 0xFF) > 0xFF, FLAG_C);

    } else if (instruction.opcode == 0xE8) {
        unsigned short old = read_reg(REG_SP);
        char newVal = (char)read_mem(read_reg(REG_PC) + 1);

        set_reg(REG_SP, old + newVal);
        cpu_set_flag(0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (newVal & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (newVal & 0xFF) > 0xFF, FLAG_C);
    } else {
        unsigned char old = read_reg(REG_A);
        unsigned char newVal;

        switch (instruction.opcode)
        {
        case 0x80:
            newVal = read_reg(REG_B);
            break;
        case 0x81:
            newVal = read_reg(REG_C);
            break;
        case 0x82:
            newVal = read_reg(REG_D);
            break;
        case 0x83:
            newVal = read_reg(REG_E);
            break;
        case 0x84:
            newVal = read_reg(REG_H);
            break;
        case 0x85:
            newVal = read_reg(REG_L);
            break;
        case 0x86:
            newVal = read_mem(read_reg(REG_HL));
            break;
        case 0x87:
            newVal = old;
            break;
        case 0xC6:
            newVal = read_mem(read_reg(REG_PC) + 1);
            break;
        default:
            ERROR("Error on ADD (2)")
        }

        set_reg(REG_A, old + newVal);

        cpu_set_flag(old + newVal, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (newVal & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (newVal & 0xFF) > 0xFF, FLAG_C);
    }

    GO_NEXT_INSTR
}

void process_INC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("INC process : %p\n", arg1);
    
    switch (instruction.opcode)
    {
    case 0x03: {
        unsigned short tmp = read_reg(REG_BC) + 1;
        set_reg(REG_BC, tmp);
    } break;
    case 0x13: {
        unsigned short tmp = read_reg(REG_DE) + 1;
        set_reg(REG_DE, tmp);
    } break;
    case 0x23: {
        unsigned short tmp = read_reg(REG_HL) + 1;
        set_reg(REG_HL, tmp);
    } break;
    case 0x33: {
        unsigned short tmp = read_reg(REG_SP) + 1;
        set_reg(REG_SP, tmp);
    } break;

    case 0x04: {
        unsigned char tmp = ((unsigned char)read_reg(REG_B)) + 1;
        set_reg(REG_B, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x14: {
        unsigned char tmp = ((unsigned char)read_reg(REG_D)) + 1;
        set_reg(REG_D, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x24: {
        unsigned char tmp = ((unsigned char)read_reg(REG_H)) + 1;
        set_reg(REG_H, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x34: {
        unsigned char tmp = read_mem(read_reg(REG_HL)) + 1;
        write_mem(read_reg(REG_HL), tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;

    case 0x0C: {
        unsigned char tmp = ((unsigned char)read_reg(REG_C)) + 1;
        set_reg(REG_C, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x1C: {
        unsigned char tmp = ((unsigned char)read_reg(REG_E)) + 1;
        set_reg(REG_E, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x2C: {
        unsigned char tmp = ((unsigned char)read_reg(REG_L)) + 1;
        set_reg(REG_L, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    case 0x3C: {
        unsigned char tmp = ((unsigned char)read_reg(REG_A)) + 1;
        set_reg(REG_A, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0, FLAG_H);
    } break;
    
    default:
        ERROR("Error on INC")
    }

    GO_NEXT_INSTR
}

void process_DAA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("TODO: DAA process\n");
    unsigned char to_add = 0;

    if (cpu_get_flag(FLAG_N)) {
        //if substraction
        if (!cpu_get_flag(FLAG_C)) {
            //C == 0
            if (!cpu_get_flag(FLAG_H)) {
                //H == 0
                if ((read_reg(REG_A) & 0xF0) <= 0x90 && (read_reg(REG_A) & 0x0F) <= 0x09) {
                    to_add = 0x00;
                    cpu_set_flag(0, FLAG_C);
                }
            } else {
                //H == 1
                if ((read_reg(REG_A) & 0xF0) <= 0x80 && (read_reg(REG_A) & 0x0F) >= 0x06) {
                    to_add = 0xFA;
                    cpu_set_flag(0, FLAG_C);
                }
            }
        } else {
            //C == 1
            if (!cpu_get_flag(FLAG_H)) {
                //H == 0
                if ((read_reg(REG_A) & 0xF0) >= 0x70 && (read_reg(REG_A)& 0x0F) <= 0x09) {
                    to_add = 0xA0;
                    cpu_set_flag(1, FLAG_C);
                }
            } else {
                //H == 1
                if ((read_reg(REG_A) & 0xF0) >= 0x60 && (read_reg(REG_A) & 0x0F) >= 0x06) {
                    to_add = 0x9A;
                    cpu_set_flag(1, FLAG_C);
                }
            }
        }

    } else {
        //if addition
        if (cpu_get_flag(FLAG_C)) {
            //C == 1
            if (cpu_get_flag(FLAG_H)) {
                //H == 1
                if ((read_reg(REG_A) & 0xF0) <= 0x30 && (read_reg(REG_A) & 0x0F) <= 0x03) {
                    to_add = 0x66;
                    cpu_set_flag(1, FLAG_C);
                }
            } else {
                //H == 0
                if ((read_reg(REG_A) & 0xF0) <= 0x20 && (read_reg(REG_A) & 0x0F) <= 0x09) {
                    to_add = 0x60;
                    cpu_set_flag(1, FLAG_C);
                } else if ((read_reg(REG_A) & 0xF0) <= 0x20 && (read_reg(REG_A) & 0x0F) >= 0x0A) {
                    to_add = 0x66;
                    cpu_set_flag(1, FLAG_C);
                }
            }

        } else {
            //C == 0
            if (cpu_get_flag(FLAG_H)) {
                //H == 1
                if ((read_reg(REG_A) & 0xF0) <= 0x90 && (read_reg(REG_A) & 0x0F) <= 0x03) {
                    to_add = 0x06;
                    cpu_set_flag(0, FLAG_C);
                } else if ((read_reg(REG_A) & 0xF0) <= 0xA0 && (read_reg(REG_A) & 0x0F) >= 0x03) {
                    to_add = 0x66;
                    cpu_set_flag(1, FLAG_C);
                }


            } else {
                //H == 0
                if ((read_reg(REG_A) & 0xF0) <= 0x90 && (read_reg(REG_A) & 0x0F) <= 0x09) {
                    to_add = 0x00;
                    cpu_set_flag(0, FLAG_C);
                } else if ((read_reg(REG_A) & 0xF0) <= 0x80 && (read_reg(REG_A) & 0x0F) >= 0x0A) {
                    to_add = 0x06;
                    cpu_set_flag(0, FLAG_C);
                } else if ((read_reg(REG_A) & 0xF0) >= 0xA0 && (read_reg(REG_A) & 0x0F) >= 0x09) {
                    to_add = 0x60;
                    cpu_set_flag(1, FLAG_C);
                } else if ((read_reg(REG_A) & 0xF0) >= 0x90 && (read_reg(REG_A) & 0x0F) >= 0x0A) {
                    to_add = 0x66;
                    cpu_set_flag(1, FLAG_C);
                }
            }
        }
    }

    set_reg(REG_A, read_reg(REG_A) + to_add);

    GO_NEXT_INSTR
}

void process_HALT(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("HALT process\n");
    cpu.halt = 1;
    // exit(2);
    GO_NEXT_INSTR
}

void process_DEC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("DEC process\n");

    switch (instruction.opcode)
    {
    case 0x05: {
        unsigned char tmp = ((unsigned char)read_reg(REG_B)) - 1;
        set_reg(REG_B, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x15: {
        unsigned char tmp = ((unsigned char)read_reg(REG_D)) - 1;
        set_reg(REG_D, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x25: {
        unsigned char tmp = ((unsigned char)read_reg(REG_H)) - 1;
        set_reg(REG_H, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x35: {
        unsigned char tmp = ((unsigned char)read_mem(read_reg(REG_HL))) - 1;
        write_mem(read_reg(REG_HL), tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;

    case 0x0B: {
        unsigned short tmp = read_reg(REG_BC) - 1;
        set_reg(REG_BC, tmp);
    } break;
    case 0x1B: {
        unsigned short tmp = read_reg(REG_DE) - 1;
        set_reg(REG_DE, tmp);
    } break;
    case 0x2B: {
        unsigned short tmp = read_reg(REG_HL) - 1;
        set_reg(REG_HL, tmp);
    } break;
    case 0x3B: {
        unsigned short tmp = read_reg(REG_SP) - 1;
        set_reg(REG_SP, tmp);
    } break;

    case 0x0D: {
        unsigned char tmp = ((unsigned char)read_reg(REG_C)) - 1;
        set_reg(REG_C, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x1D: {
        unsigned char tmp = ((unsigned char)read_reg(REG_E)) - 1;
        set_reg(REG_E, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x2D: {
        unsigned char tmp = ((unsigned char)read_reg(REG_L)) - 1;
        set_reg(REG_L, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    case 0x3D: {
        unsigned char tmp = ((unsigned char)read_reg(REG_A)) - 1;
        set_reg(REG_A, tmp);
        cpu_set_flag(tmp == 0, FLAG_Z);
        cpu_set_flag(1, FLAG_N);
        cpu_set_flag((tmp & 0x0F) == 0x0F, FLAG_H);
    } break;
    default:
        ERROR("Error on DEC (1)")
    }

    GO_NEXT_INSTR
}

void process_OR(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("OR process : %02X\n", arg1);
    
    set_reg(REG_A, read_reg(REG_A) | arg1);

    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    GO_NEXT_INSTR
}

void process_XOR(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("XOR process\n");

    //TODO set les bits pour toutes les instructions
    switch (instruction.mode)
    {
    case MODE_R:
        switch (instruction.r1)
        {
        case REG_A:
        case REG_B:
        case REG_C:
        case REG_D:
        case REG_E:
        case REG_H:
        case REG_L:
            set_reg(REG_A, read_reg(REG_A) ^ arg1);
            break;
        default:
            ERROR("Incorrect param1 for XOR")
        }
        break;
    case MODE_MR:
        switch (instruction.r1)
        {
        case REG_HL: {
            unsigned char offset = read_mem(read_reg(REG_HL));
            set_reg(REG_A, read_reg(REG_A) ^ read_mem(offset));
            break;
        }
        default:
            ERROR("Incorrect param2 for XOR")
        }
        break;
    case MODE_D8:
        set_reg(REG_A, read_reg(REG_A) ^ (char)arg1);
        break;
    default:
        ERROR("Incorrect param3 for XOR")
    }
    
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);

    GO_NEXT_INSTR
}

void process_NOP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("NOP process\n");
    GO_NEXT_INSTR
}

void process_JP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("JP process\n");
    
    unsigned short val = 0;
    switch (instruction.opcode)
    {
        case 0xC3: {

            unsigned short nn = read_mem(read_reg(REG_PC) + 1);
            emu_cycle(1);
            nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
            emu_cycle(2);
            set_reg(REG_PC, nn);
        } break;
        case 0xE9:
            set_reg(REG_PC, read_reg(REG_HL));
            emu_cycle(1);
            break;
        default: {
            unsigned short nn = read_mem(read_reg(REG_PC) + 1);
            emu_cycle(1);
            nn |= (read_mem(read_reg(REG_PC) + 2) << 8);
            emu_cycle(1);
            if (!valid_conditions(instruction)) {
                GO_NEXT_INSTR
                return ;
            }
            set_reg(REG_PC, nn);
            emu_cycle(1);
        } break;
    }
}