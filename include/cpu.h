#ifndef CPU_H
#define CPU_H

#include <common.h>

typedef struct {
    struct {
        unsigned char af[2];
#define reg_a af[1]
#define reg_flags af[0]
        unsigned char bc[2];
#define reg_b bc[1]
#define reg_c bc[0]
        unsigned char de[2];
#define reg_d de[1]
#define reg_e de[0]
        unsigned char hl[2];
#define reg_h hl[1]
#define reg_l hl[0]
        unsigned short sp;
        unsigned short pc;
    } registers;

    unsigned char IME; //Interrupt Master Enable flag
    unsigned char enabling_IME;
    unsigned char halt; //Halt flag
    unsigned char int_flags;
    unsigned char ie_register;
    unsigned long ticks;
    
    unsigned increment_ly : 1;
} CPU;

enum REGISTERS {
    REG_NONE,
    REG_A,
    REG_F,
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_AF,
    REG_BC,
    REG_DE,
    REG_HL,
    REG_SP,
    REG_PC,

    MEM_A8,
    MEM_A16,
    MEM_D8,
    MEM_D16,
    MEM_R8
};

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4
#define COND_NC 1
#define COND_NZ 2
#define COND_NH 3
#define COND_NN 0

void init_cpu();

int cpu_get_flag(unsigned char flag);
void cpu_set_flag(char condition, int flag);

unsigned short read_reg(int reg);
void set_reg(int reg, unsigned short val);

extern CPU cpu;

void request_interrupt(int type);

void emu_cycle(unsigned short n);

#endif