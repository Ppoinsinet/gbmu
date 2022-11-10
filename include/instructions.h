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

} INSTRUCTION;

typedef struct {
    void *data;
    unsigned long size;
} INSTR_ARG;

typedef void (*INSTR_FUNCTION)(INSTRUCTION, INSTR_ARG, INSTR_ARG);

extern const INSTRUCTION map_instructions[];
extern const INSTR_FUNCTION map_process[];

INSTRUCTION getInstruction(unsigned char opcode);
INSTRUCTION getCBInstruction(unsigned char opcode);

INSTR_ARG getFirstArg(INSTRUCTION instruction);
INSTR_ARG getSecondArg(INSTRUCTION instruction);

void instruction_process(INSTRUCTION instruction);

void process_XOR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_NOP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_JP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_LD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);


void process_INC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_ADD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_ADC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SBC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SUB(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_DEC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_CCF(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_DAA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_LDI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_LDD(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_JR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_DI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_EI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_LDH(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_CP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_OR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_CPL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_AND(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_POP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_PUSH(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_STOP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_CALL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RET(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RETI(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RST(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_HALT(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_SCF(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_CCF(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);

void process_CB(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RLC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RLCA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RRC(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RLA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RR(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RRA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SLA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SRA(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SWAP(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SRL(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_BIT(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_RES(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);
void process_SET(INSTRUCTION instruction, INSTR_ARG arg1, INSTR_ARG arg2);


#endif