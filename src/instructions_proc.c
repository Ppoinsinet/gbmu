#include <instructions.h>
#include <cpu.h>
#include <cartridge.h>
#include <bus.h>
#include <dbg.h>

const INSTR_FUNCTION map_process[] = {
    [INSTR_NOP] = process_NOP,
    [INSTR_JP] = process_JP,
    [INSTR_XOR] = process_XOR,
    [INSTR_LD] = process_LD,
    [INSTR_INC] = process_INC,
    [INSTR_DEC] = process_DEC,
    [INSTR_LDD] = process_LDD,
    [INSTR_LDI] = process_LDI,
    [INSTR_JR] = process_JR,
    [INSTR_EI] = process_EI,
    [INSTR_DI] = process_DI,
    [INSTR_LDH] = process_LDH,
    [INSTR_CP] = process_CP,
    [INSTR_CALL] = process_CALL,
    [INSTR_OR] = process_OR,
    [INSTR_RET] = process_RET,
    [INSTR_RETI] = process_RETI,
    [INSTR_POP] = process_POP,
    [INSTR_PUSH] = process_PUSH,
    [INSTR_CPL] = process_CPL,
    [INSTR_AND] = process_AND,
    [INSTR_CB] = process_CB,
    [INSTR_RST] = process_RST,
    [INSTR_ADD] = process_ADD,
    [INSTR_HALT] = process_HALT,
    [INSTR_RLA] = process_RLA,
    [INSTR_RLCA] = process_RLCA,
    [INSTR_CCF] = process_CCF,
    [INSTR_SCF] = process_SCF,
    [INSTR_ADC] = process_ADC,
    [INSTR_SUB] = process_SUB,
    [INSTR_STOP] = process_STOP,
    [INSTR_DAA] = process_DAA,
    [INSTR_RRA] = process_RRA,
    [INSTR_SBC] = process_SBC
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

void process_RST(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RST process\n");


    set_reg(REG_SP, read_reg(REG_SP) - 2);
    write_mem16(read_reg(REG_SP), read_reg(REG_PC) + instruction.size);

    set_reg(REG_PC, instruction.param);
}

void process_SWAP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB SWAP process\n");

    unsigned short tmp = *((unsigned short *)arg1.data);
    *((unsigned short *)arg1.data) = ((tmp & 0xFF) << 4) | ((tmp & 0xFF00) >> 4);

    cpu_set_flag(*((unsigned short *)arg1.data) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);

    GO_NEXT_INSTR
}

void process_SLA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB SLA process\n");

    cpu_set_flag((*((unsigned short *)arg1.data) << 1) == 0, FLAG_Z);
    cpu_set_flag((*((unsigned short *)arg1.data) & (1 << 7)), FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    
    *(unsigned char*)arg1.data = ((*(unsigned char*)arg1.data) << 1) & ~(0x1);
    GO_NEXT_INSTR
}

void process_SRA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB SRA process\n");

    cpu_set_flag((*((unsigned short *)arg1.data) >> 1) == 0, FLAG_Z);
    cpu_set_flag(*((unsigned short *)arg1.data) & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    
    *(unsigned char*)arg1.data = ((*(unsigned char*)arg1.data) >> 1) | ((*(unsigned char*)arg1.data) & (0x80));
    GO_NEXT_INSTR
}

void process_SRL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB SRL process\n");

    cpu_set_flag((*((unsigned short *)arg1.data) >> 1) == 0, FLAG_Z);
    cpu_set_flag(*((unsigned short *)arg1.data) & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);


    
    *(unsigned char*)arg1.data = ((*(unsigned char*)arg1.data) >> 1) & ~(0x80);
    GO_NEXT_INSTR
}

void process_RLA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RLA process\n");

    int cy = cpu_get_flag(FLAG_C);

    cpu_set_flag(read_reg(REG_A) & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) << 1) + (cy ? 1 : 0));
    
    GO_NEXT_INSTR
}

void process_RL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB RL process\n");

    unsigned char tmp = *(unsigned char*)arg1.data;
    unsigned char cy = cpu_get_flag(FLAG_C);

    *(unsigned char*)arg1.data = (tmp << 1) + (cy ? 1 : 0);

    cpu_set_flag(((int)tmp << 1) & ~(0xFF), FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(*(unsigned char*)arg1.data == 0, FLAG_Z);
    
    GO_NEXT_INSTR
}

void process_RRA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RRA process\n");

    unsigned char cy = cpu_get_flag(FLAG_C);


    cpu_set_flag(read_reg(REG_A) & 0x1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    
    set_reg(REG_A, (read_reg(REG_A) >> 1) + (cy ? 0x80 : 0));
    
    GO_NEXT_INSTR
}

void process_RR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB RR process\n");

    unsigned char tmp = *(unsigned char*)arg1.data;
    unsigned char cy = cpu_get_flag(FLAG_C);

    *(unsigned char*)arg1.data = (tmp >> 1) + (cy ? 0x80 : 0);

    cpu_set_flag(tmp & 0x1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(*(unsigned char*)arg1.data == 0, FLAG_Z);
    
    GO_NEXT_INSTR
}

void process_RLCA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RLCA process\n");


    cpu_set_flag(read_reg(REG_A) & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) << 1) + (cpu_get_flag(FLAG_C) ? 1 : 0));
    
    GO_NEXT_INSTR
}

void process_RLC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB RLC process\n");

    unsigned char tmp = *(unsigned char*)arg1.data;

    cpu_set_flag(tmp & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(*(unsigned char*)arg1.data == 0, FLAG_Z);

    *(unsigned char*)arg1.data = (tmp << 1) + (cpu_get_flag(FLAG_C) ? 1 : 0);
    
    GO_NEXT_INSTR
}

void process_RRCA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RRCA process\n");


    cpu_set_flag(read_reg(REG_A) & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) >> 1) + (cpu_get_flag(FLAG_C) ? 0x80 : 0));
    
    GO_NEXT_INSTR
}

void process_RRC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB RRC process\n");

    unsigned char tmp = *(unsigned char*)arg1.data;

    cpu_set_flag(tmp & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(*(unsigned char*)arg1.data == 0, FLAG_Z);

    *(unsigned char*)arg1.data = (tmp >> 1) + (cpu_get_flag(FLAG_C) ? 0x80 : 0);
    
    GO_NEXT_INSTR
}

void process_BIT(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB BIT process\n");

    unsigned char tmp = *(unsigned char *)arg1.data;
    cpu_set_flag((tmp & (1 << instruction.param)) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(1, FLAG_H);

    GO_NEXT_INSTR
}

void process_SET(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB SET process\n");

    *(unsigned char *)arg1.data |= (1 << instruction.param);
    GO_NEXT_INSTR
}

void process_RES(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB RES process\n");

    *(unsigned char *)arg1.data &= ~(1 << instruction.param) ;
    GO_NEXT_INSTR
}

void process_CB(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CB process\n");

    const INSTRUCTION next_instr = getCBInstruction(*(unsigned char *)arg1.data);
    if (map_cb_process[next_instr.instruction])
        map_cb_process[next_instr.instruction](next_instr, getFirstArg(instruction), getSecondArg(instruction));
    GO_NEXT_INSTR
}

void process_PUSH(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("PUSH process\n");

    push_stack(*(unsigned short *)arg1.data);

    GO_NEXT_INSTR
}

void process_POP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
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

void process_AND(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("AND process\n");

    set_reg(REG_A, read_reg(REG_A) & *(unsigned char *)arg1.data);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(1, FLAG_H);
    cpu_set_flag(0, FLAG_C);
    GO_NEXT_INSTR
}

void process_SCF(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(1, FLAG_C);

    GO_NEXT_INSTR
}

void process_CCF(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(cpu_get_flag(FLAG_C) ^ 1, FLAG_C);

    GO_NEXT_INSTR
}

void process_CPL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CPL process\n");
    set_reg(REG_A, ~read_reg(REG_A));

    cpu_set_flag(1, FLAG_N);
    cpu_set_flag(1, FLAG_H);
    GO_NEXT_INSTR
}

void process_RETI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("RETI process\n");

    cpu.IME = 1;
    unsigned short ret = pop_stack();
    
    set_reg(REG_PC, ret);
}

void process_RET(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    unsigned short ret = pop_stack();
    if (enable_print) printf("RET process : 0x%04X\n", ret);

    
    set_reg(REG_PC, ret);
}

void process_STOP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("TODO: STOP process\n");

    NO_IMPL
    GO_NEXT_INSTR
}

void process_CALL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("CALL process\n");
    //TODO push ret address
    if (enable_print) printf("CALL - Jumping to 0x%hX\n", *(unsigned short *)arg1.data);
    if (enable_print) printf("CALL - Pushing : 0x%04X\n", read_reg(REG_PC) + instruction.size);

    push_stack(read_reg(REG_PC) + instruction.size);

    set_reg(REG_PC, *(unsigned short *)arg1.data);
}

void process_CP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
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
        tmp = *((unsigned char *)arg1.data);
        break;
    default:
        ERROR("Wrong call to CP");
    }

    int val = tmp - read_reg(REG_A);
    cpu_set_flag(val == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((tmp & 0x0F) - (read_reg(REG_A) & 0x0F) < 0, FLAG_H);
    cpu_set_flag(val < 0, FLAG_C);
    
    

    //TODO finir implement flags

    GO_NEXT_INSTR

}

void process_LDH(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("LDH process\n");

    if (instruction.r2 == MEM_A8) {
        //F0
        if (enable_print) printf("LDH - Reading val at mem : (0x%04hX) == %02hX\n", 0xFF00 + read_mem(read_reg(REG_PC) + 1), read_mem(0xFF00 + read_mem(read_reg(REG_PC) + 1)));
        set_reg(REG_A, read_mem(0xFF00 + read_mem(read_reg(REG_PC) + 1)));
    } else {
        //E0
        if (enable_print) printf("LDH - Writing val at REG_A : %02hX\n", read_mem(0xFF00 + read_mem(read_reg(REG_PC) + 1)));
        write_mem(0xFF00 + read_mem(read_reg(REG_PC) + 1), read_reg(REG_A));
    }
    GO_NEXT_INSTR
}

void process_EI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("EI process\n");

    cpu.enabling_IME = 1;
    GO_NEXT_INSTR
}

void process_DI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("DI process\n");

    cpu.IME = 0;
    GO_NEXT_INSTR
}

void process_JR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("JR process\n");
    
    set_reg(REG_PC, read_reg(REG_PC) + instruction.size + *(char *)arg1.data);
}

void process_LDD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("LDD process\n");

    if (instruction.r1 == REG_HL) {
        write_mem(read_reg(REG_HL), read_reg(REG_A));
    } else {
        set_reg(REG_A, read_mem(read_reg(REG_HL)));
    }
    set_reg(REG_HL, read_reg(REG_HL) - 1);
    GO_NEXT_INSTR

}

void process_LDI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("LDI process\n");

    if (instruction.r1 == REG_HL) {
        write_mem(read_reg(REG_HL), read_reg(REG_A));
    } else {
        set_reg(REG_A, read_mem(read_reg(REG_HL)));
    }
    set_reg(REG_HL, read_reg(REG_HL) + 1);
    GO_NEXT_INSTR
}

void process_ADC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("ADC process\n");

    int reg_a = read_reg(REG_A);
    int flagc = cpu_get_flag(FLAG_C);
    int val = reg_a + flagc + *(unsigned char *)arg1.data;
    int half = (reg_a & 0x0F) + (*(unsigned char *)arg1.data & 0x0F) + (flagc ? 1 : 0);

    cpu_set_flag((val & 0xFF) != val, FLAG_C);
    cpu_set_flag(val == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(half > 0x0F, FLAG_H);

    set_reg(REG_A, val);
    GO_NEXT_INSTR
}

void process_SUB(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("SUB process\n");

    unsigned char old = read_reg(REG_A);
    unsigned char new = *(unsigned char *)arg1.data;
    set_reg(REG_A, read_reg(REG_A) - *(unsigned char *)arg1.data);

    //todo check flags
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((old & 0xF) + (new & 0xF) > 0xF, FLAG_H);
    cpu_set_flag((old & 0xFF) + (new & 0xFF) > 0xFF, FLAG_C);
    GO_NEXT_INSTR
}

void process_SBC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("SBC process\n");

    unsigned char old = read_reg(REG_A);
    unsigned char new = *(unsigned char *)arg1.data;
    set_reg(REG_A, read_reg(REG_A) - *(unsigned char *)arg1.data + cpu_get_flag(FLAG_C));
    

    //todo check flags
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(1, FLAG_N);
    cpu_set_flag((old & 0xF) - (new & 0xF) > 0xF, FLAG_H);
    cpu_set_flag((old & 0xFF) - (new & 0xFF) > 0xFF, FLAG_C);
    GO_NEXT_INSTR
}



void process_ADD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("ADD process\n");

    //TODO set flags

    if (instruction.r1 == REG_SP) {
        unsigned short old = read_reg(REG_SP);
        char new = *(char *)arg2.data;

        set_reg(REG_SP, old + new);
        cpu_set_flag(0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (new & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (new & 0xFF) > 0xFF, FLAG_C);
    } else if (arg1.size == 1) {
        unsigned char old = *(unsigned char *)arg1.data;
        unsigned char new = *(unsigned char *)arg2.data;

        *(unsigned char *)arg1.data = *(unsigned char *)arg2.data;
        cpu_set_flag(*(unsigned char *)arg1.data == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (new & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (new & 0xFF) > 0xFF, FLAG_C);
    } else {
        unsigned short old = *(unsigned short *)arg1.data;
        unsigned short new = *(unsigned short *)arg2.data;

        *(unsigned short *)arg1.data = *(unsigned short *)arg2.data;
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((old & 0xF) + (new & 0xF) > 0xF, FLAG_H);
        cpu_set_flag((old & 0xFF) + (new & 0xFF) > 0xFF, FLAG_C);
    }
    GO_NEXT_INSTR
}

void process_INC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("INC process : %p\n", arg1.data);
    (*(unsigned char *)arg1.data)++;

    switch (read_mem(read_reg(REG_PC)) & 0x0F)
    {
    case 0x03:
        break;
    case 0x04:
    case 0x0C:
        cpu_set_flag((*(char *)arg1.data) == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag(0, FLAG_H);
        break;
    default:
        ERROR("Error on INC instruction");
    }

    GO_NEXT_INSTR
}

void process_DAA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
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

void process_HALT(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    cpu.halt = 1;
    GO_NEXT_INSTR
}

void process_DEC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("DEC process\n");
    (*(unsigned char *)arg1.data)--;

    switch (read_mem(read_reg(REG_PC)) & 0x0F)
    {
    case 0x0B:
        break;
    case 0x05:
    case 0x0D:
        cpu_set_flag((*(unsigned char *)arg1.data) == 0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag(0, FLAG_H);
        break;
    default:
        ERROR("Error on DEC instruction");
    }
    GO_NEXT_INSTR
}

void process_LD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("LD process\n");

    if (instruction.r1 == REG_HL && instruction.r2 == REG_SP) {
        if (enable_print) printf("TODO: LD HL to SP+r8 : %p et %p\n", arg1.data, arg2.data);
        char offset = (char)read_mem(read_reg(REG_PC) + 1);
        *(unsigned short *)arg1.data = *(unsigned short *)arg2.data + offset;

        cpu_set_flag(0, FLAG_Z);
        cpu_set_flag(0, FLAG_N);
        cpu_set_flag((*(unsigned short *)arg2.data & 0xFFF) + (offset & 0xFFF) > 0xFFF, FLAG_H);
        cpu_set_flag((*(unsigned short *)arg2.data) + (offset) > 0xFFFF, FLAG_C);

        GO_NEXT_INSTR
        return ;
    } else if (instruction.r1 == REG_SP && instruction.r2 == REG_HL) {
        set_reg(REG_SP, read_reg(REG_HL));
        GO_NEXT_INSTR
        return ;
    }

    unsigned short fetched = 0;
    if (instruction.r2 >= REG_AF && instruction.r2 <= REG_PC)
        fetched = read_mem(read_reg(instruction.r2));
    else if (instruction.r2 == MEM_D8)
        fetched = read_mem(read_reg(REG_PC) + 1);
    else if (instruction.r2 == MEM_D16)
        fetched = read_mem16(read_reg(REG_PC) + 1);
    else if (instruction.r2 == MEM_A16)
        fetched = read_mem(read_mem16(read_reg(REG_PC) + 1));
    else if (instruction.r2 >= REG_A && instruction.r2 <= REG_L)
        fetched = read_reg(instruction.r2);
    else
        ERROR("LD - Incorrect fetched value");

    if (instruction.r1 == MEM_A16)
        write_mem(read_mem16(read_reg(REG_PC) + 1), fetched);
    else if (instruction.r1 >= REG_A && instruction.r1 <= REG_L && instruction.mode >= MODE_R && instruction.mode <= MODE_R_MR)
        set_reg(instruction.r1, fetched);
    else if (instruction.r1 >= REG_A && instruction.r1 <= REG_L && instruction.mode >= MODE_MR && instruction.mode <= MODE_MR_R8)
        write_mem(0xFF + read_reg(instruction.r1), fetched);
    else if (instruction.r1 >= REG_AF && instruction.r1 <= REG_PC && instruction.mode >= MODE_R && instruction.mode <= MODE_R_MR)
        set_reg(instruction.r1, fetched);
    else if (instruction.r1 >= REG_AF && instruction.r1 <= REG_PC && instruction.mode >= MODE_MR && instruction.mode <= MODE_MR_R8)
        write_mem(read_reg(instruction.r1), fetched);
    else
        ERROR("LD - Incorrect setup register");

    GO_NEXT_INSTR
}

void process_OR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("OR process : %02X\n", arg1.data);
    
    set_reg(REG_A, read_reg(REG_A) | *(unsigned char *)arg1.data);

    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    GO_NEXT_INSTR
}

void process_XOR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
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
            set_reg(REG_A, read_reg(REG_A) ^ *(unsigned char *)arg1.data);
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
        set_reg(REG_A, read_reg(REG_A) ^ *(char*)arg1.data);
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

void process_NOP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("NOP process\n");
    GO_NEXT_INSTR
}

void process_JP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2) {
    if (enable_print) printf("JP process\n");

    //TODO implement conditions
    
    unsigned short val = 0;
    switch (instruction.mode)
    {
    case MODE_A16:
        if (enable_print) printf("JP - Jumping to 0x%hX\n", *((unsigned short *)arg1.data));
        set_reg(REG_PC, *((unsigned short *)arg1.data));
        break;
    case MODE_MR:
        switch (instruction.r1)
        {
        case REG_HL:
            if (enable_print) printf("JP - Jumping to 0x%hX\n", *((unsigned short *)arg1.data));
            set_reg(REG_PC, *((unsigned short *)arg1.data));
            break;
        default:
            ERROR("Incorrect register was entered for JP call");
        }
        break;
    default:
        ERROR("Incorrect mode for JP operation");
    }
}