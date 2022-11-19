#include <instructions.h>
#include <dbg.h>

void process_SWAP(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB SWAP process\n");

    unsigned short tmp = arg1;
    arg1 = ((tmp & 0xFF) << 4) | ((tmp & 0xFF00) >> 4);

    cpu_set_flag(arg1 == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);

    GO_NEXT_INSTR
}

void process_SLA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB SLA process\n");

    cpu_set_flag((arg1 << 1) == 0, FLAG_Z);
    cpu_set_flag((arg1 & (1 << 7)), FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    
    arg1 = (arg1 << 1) & ~(0x1);
    GO_NEXT_INSTR
}

void process_SRA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB SRA process\n");

    cpu_set_flag((arg1 >> 1) == 0, FLAG_Z);
    cpu_set_flag(arg1 & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    
    arg1 = (arg1 >> 1) | (arg1 & (0x80));
    GO_NEXT_INSTR
}

void process_SRL(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB SRL process\n");

    cpu_set_flag((arg1 >> 1) == 0, FLAG_Z);
    cpu_set_flag(arg1 & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);


    
    arg1 = (arg1 >> 1) & ~(0x80);
    GO_NEXT_INSTR
}

void process_RLA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RLA process\n");

    int cy = cpu_get_flag(FLAG_C);

    cpu_set_flag(read_reg(REG_A) & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) << 1) + (cy ? 1 : 0));
    
    GO_NEXT_INSTR
}

void process_RL(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB RL process\n");

    unsigned char tmp = arg1;
    unsigned char cy = cpu_get_flag(FLAG_C);

    arg1 = (tmp << 1) + (cy ? 1 : 0);

    cpu_set_flag(((int)tmp << 1) & ~(0xFF), FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(arg1 == 0, FLAG_Z);
    
    GO_NEXT_INSTR
}

void process_RRA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RRA process\n");

    unsigned char cy = cpu_get_flag(FLAG_C);


    cpu_set_flag(read_reg(REG_A) & 0x1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    
    set_reg(REG_A, (read_reg(REG_A) >> 1) + (cy ? 0x80 : 0));
    
    GO_NEXT_INSTR
}

void process_RR(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB RR process\n");

    unsigned char tmp = arg1;
    unsigned char cy = cpu_get_flag(FLAG_C);

    arg1 = (tmp >> 1) + (cy ? 0x80 : 0);

    cpu_set_flag(tmp & 0x1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(arg1 == 0, FLAG_Z);
    
    GO_NEXT_INSTR
}

void process_RLCA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RLCA process\n");


    cpu_set_flag(read_reg(REG_A) & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) << 1) + (cpu_get_flag(FLAG_C) ? 1 : 0));
    
    GO_NEXT_INSTR
}

void process_RLC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB RLC process\n");

    unsigned char tmp = arg1;

    cpu_set_flag(tmp & 0x80, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(arg1 == 0, FLAG_Z);

    arg1 = (tmp << 1) + (cpu_get_flag(FLAG_C) ? 1 : 0);
    
    GO_NEXT_INSTR
}

void process_RRCA(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("RRCA process\n");


    cpu_set_flag(read_reg(REG_A) & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);

    set_reg(REG_A, (read_reg(REG_A) >> 1) + (cpu_get_flag(FLAG_C) ? 0x80 : 0));
    
    GO_NEXT_INSTR
}

void process_RRC(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB RRC process\n");

    unsigned char tmp = arg1;

    cpu_set_flag(tmp & 1, FLAG_C);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(arg1 == 0, FLAG_Z);

    arg1 = (tmp >> 1) + (cpu_get_flag(FLAG_C) ? 0x80 : 0);
    
    GO_NEXT_INSTR
}

void process_BIT(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB BIT process : %d\n", instruction.param);
    unsigned char tmp = arg1;
    cpu_set_flag((tmp & (1 << instruction.param)) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(1, FLAG_H);

    GO_NEXT_INSTR
}

void process_SET(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB SET process\n");

    arg1 |= (1 << instruction.param);
    GO_NEXT_INSTR
}

void process_RES(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CB RES process : %hhX\n", arg1);

    arg1 &= ~(1 << instruction.param) ;
    GO_NEXT_INSTR
}


void process_AND(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("AND process\n");

    set_reg(REG_A, read_reg(REG_A) & arg1);
    cpu_set_flag(read_reg(REG_A) == 0, FLAG_Z);
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(1, FLAG_H);
    cpu_set_flag(0, FLAG_C);
    GO_NEXT_INSTR
}

void process_SCF(INSTRUCTION instruction, unsigned char &arg1) {
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(1, FLAG_C);

    GO_NEXT_INSTR
}

void process_CCF(INSTRUCTION instruction, unsigned char &arg1) {
    cpu_set_flag(0, FLAG_N);
    cpu_set_flag(0, FLAG_H);
    cpu_set_flag(!cpu_get_flag(FLAG_C), FLAG_C);

    GO_NEXT_INSTR
}

void process_CPL(INSTRUCTION instruction, unsigned char &arg1) {
    if (enable_print) printf("CPL process\n");
    set_reg(REG_A, ~read_reg(REG_A));

    cpu_set_flag(1, FLAG_N);
    cpu_set_flag(1, FLAG_H);
    GO_NEXT_INSTR
}