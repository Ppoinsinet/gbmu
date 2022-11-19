#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <common.h>
#include <cpu.h>
#include <ram.h>

#define GO_NEXT_INSTR {set_reg(REG_PC, read_reg(REG_PC) + instruction.size);}

enum INSTR_TYPE {
    INSTR_NONE,
    INSTR_NOP,
    INSTR_LD,
    INSTR_LDD,
    INSTR_LDI,
    INSTR_INC,
    INSTR_DEC,
    INSTR_RLCA,
    INSTR_RLA,
    INSTR_ADD,
    INSTR_RRCA,
    INSTR_RRA,
    INSTR_JR,
    INSTR_DAA,
    INSTR_CPL,
    INSTR_CCF,
    INSTR_SCF,
    INSTR_HALT,
    INSTR_ADC,
    INSTR_SUB,
    INSTR_SBC,
    INSTR_AND,
    INSTR_XOR,
    INSTR_OR,
    INSTR_CP,
    INSTR_RET,
    INSTR_POP,
    INSTR_PUSH,
    INSTR_JP,
    INSTR_CALL,
    INSTR_RST,
    INSTR_CB,
    INSTR_RETI,
    INSTR_LDH,
    INSTR_DI,
    INSTR_EI,
    INSTR_STOP,

    INSTR_RLC,
    INSTR_RRC,
    INSTR_RL,
    INSTR_RR,
    INSTR_SLA,
    INSTR_SRA,
    INSTR_SWAP,
    INSTR_SRL,
    INSTR_BIT,
    INSTR_RES,
    INSTR_SET
};

enum ADDRESSING_MODE {
    MODE_NONE,
    MODE_R,
    MODE_R_R,
    MODE_R_D16,
    MODE_R_D8,
    MODE_R_A8,
    MODE_R_A16,
    MODE_R_R8,
    MODE_R_MR,

    MODE_MR,
    MODE_MR_MR,
    MODE_MR_R,
    MODE_MR_D8,
    MODE_MR_D16,
    MODE_MR_A8,
    MODE_MR_A16,
    MODE_MR_R8,

    MODE_A16_R,

    MODE_R8,
    MODE_D8,

    MODE_A16,

    MODE_A8_R



};

typedef struct {
    enum INSTR_TYPE instruction;
    unsigned char size;
    enum ADDRESSING_MODE mode;

    enum REGISTERS r1;
    enum REGISTERS r2;
    unsigned char conditionalFlag;

    char param;
    unsigned char opcode;

} INSTRUCTION;

typedef void (*INSTR_FUNCTION)(INSTRUCTION, unsigned char &);

extern unsigned long count;


void execute_process(INSTRUCTION instruction, unsigned char &arg1);

INSTRUCTION getInstruction(unsigned char opcode);
INSTRUCTION getCBInstruction(unsigned char opcode);

unsigned char &getFirstArg(INSTRUCTION instruction);

void instruction_process(INSTRUCTION instruction);

int valid_conditions(INSTRUCTION instruction);

void process_XOR(INSTRUCTION instruction, unsigned char &arg1);
void process_NOP(INSTRUCTION instruction, unsigned char &arg1);
void process_JP(INSTRUCTION instruction, unsigned char &arg1);
void process_LD(INSTRUCTION instruction, unsigned char &arg1);


void process_INC(INSTRUCTION instruction, unsigned char &arg1);
void process_ADD(INSTRUCTION instruction, unsigned char &arg1);
void process_ADC(INSTRUCTION instruction, unsigned char &arg1);
void process_SBC(INSTRUCTION instruction, unsigned char &arg1);
void process_SUB(INSTRUCTION instruction, unsigned char &arg1);
void process_DEC(INSTRUCTION instruction, unsigned char &arg1);

void process_CCF(INSTRUCTION instruction, unsigned char &arg1);
void process_DAA(INSTRUCTION instruction, unsigned char &arg1);

void process_LDI(INSTRUCTION instruction, unsigned char &arg1);
void process_LDD(INSTRUCTION instruction, unsigned char &arg1);
void process_JR(INSTRUCTION instruction, unsigned char &arg1);
void process_DI(INSTRUCTION instruction, unsigned char &arg1);
void process_EI(INSTRUCTION instruction, unsigned char &arg1);
void process_LDH(INSTRUCTION instruction, unsigned char &arg1);
void process_CP(INSTRUCTION instruction, unsigned char &arg1);
void process_OR(INSTRUCTION instruction, unsigned char &arg1);
void process_CPL(INSTRUCTION instruction, unsigned char &arg1);
void process_AND(INSTRUCTION instruction, unsigned char &arg1);

void process_POP(INSTRUCTION instruction, unsigned char &arg1);
void process_PUSH(INSTRUCTION instruction, unsigned char &arg1);

void process_STOP(INSTRUCTION instruction, unsigned char &arg1);

void process_CALL(INSTRUCTION instruction, unsigned char &arg1);
void process_RET(INSTRUCTION instruction, unsigned char &arg1);
void process_RETI(INSTRUCTION instruction, unsigned char &arg1);
void process_RST(INSTRUCTION instruction, unsigned char &arg1);

void process_HALT(INSTRUCTION instruction, unsigned char &arg1);

void process_SCF(INSTRUCTION instruction, unsigned char &arg1);
void process_CCF(INSTRUCTION instruction, unsigned char &arg1);

void process_CB(INSTRUCTION instruction, unsigned char &arg1);
void process_RLC(INSTRUCTION instruction, unsigned char &arg1);
void process_RLCA(INSTRUCTION instruction, unsigned char &arg1);
void process_RRC(INSTRUCTION instruction, unsigned char &arg1);
void process_RL(INSTRUCTION instruction, unsigned char &arg1);
void process_RLA(INSTRUCTION instruction, unsigned char &arg1);
void process_RR(INSTRUCTION instruction, unsigned char &arg1);
void process_RRA(INSTRUCTION instruction, unsigned char &arg1);
void process_SLA(INSTRUCTION instruction, unsigned char &arg1);
void process_SRA(INSTRUCTION instruction, unsigned char &arg1);
void process_SWAP(INSTRUCTION instruction, unsigned char &arg1);
void process_SRL(INSTRUCTION instruction, unsigned char &arg1);
void process_BIT(INSTRUCTION instruction, unsigned char &arg1);
void process_RES(INSTRUCTION instruction, unsigned char &arg1);
void process_SET(INSTRUCTION instruction, unsigned char &arg1);


#endif