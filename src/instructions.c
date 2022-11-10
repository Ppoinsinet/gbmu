#include <instructions.h>
#include <cpu.h>
#include <cartridge.h>
#include <common.h>
#include <bus.h>
#include <dbg.h>
#include <io.h>

const INSTRUCTION map_cb_instructions[] = {
    [0x00] = {INSTR_RLC,    1,  MODE_R,     REG_B},
    [0x01] = {INSTR_RLC,    1,  MODE_R,     REG_C},
    [0x02] = {INSTR_RLC,    1,  MODE_R,     REG_D},
    [0x03] = {INSTR_RLC,    1,  MODE_R,     REG_E},
    [0x04] = {INSTR_RLC,    1,  MODE_R,     REG_H},
    [0x05] = {INSTR_RLC,    1,  MODE_R,     REG_L},
    [0x06] = {INSTR_RLC,    1,  MODE_MR,    REG_HL},
    [0x07] = {INSTR_RLC,    1,  MODE_R,     REG_A},
    [0x08] = {INSTR_RRC,    1,  MODE_R,     REG_B},
    [0x09] = {INSTR_RRC,    1,  MODE_R,     REG_C},
    [0x0A] = {INSTR_RRC,    1,  MODE_R,     REG_D},
    [0x0B] = {INSTR_RRC,    1,  MODE_R,     REG_E},
    [0x0C] = {INSTR_RRC,    1,  MODE_R,     REG_H},
    [0x0D] = {INSTR_RRC,    1,  MODE_R,     REG_L},
    [0x0E] = {INSTR_RRC,    1,  MODE_MR,    REG_HL},
    [0x0F] = {INSTR_RRC,    1,  MODE_R,     REG_A},

    [0x10] = {INSTR_RL,     1,  MODE_R,     REG_B},
    [0x11] = {INSTR_RL,     1,  MODE_R,     REG_C},
    [0x12] = {INSTR_RL,     1,  MODE_R,     REG_D},
    [0x13] = {INSTR_RL,     1,  MODE_R,     REG_E},
    [0x14] = {INSTR_RL,     1,  MODE_R,     REG_H},
    [0x15] = {INSTR_RL,     1,  MODE_R,     REG_L},
    [0x16] = {INSTR_RL,     1,  MODE_MR,    REG_HL},
    [0x17] = {INSTR_RL,     1,  MODE_R,     REG_A},
    [0x18] = {INSTR_RR,     1,  MODE_R,     REG_B},
    [0x19] = {INSTR_RR,     1,  MODE_R,     REG_C},
    [0x1A] = {INSTR_RR,     1,  MODE_R,     REG_D},
    [0x1B] = {INSTR_RR,     1,  MODE_R,     REG_E},
    [0x1C] = {INSTR_RR,     1,  MODE_R,     REG_H},
    [0x1D] = {INSTR_RR,     1,  MODE_R,     REG_L},
    [0x1E] = {INSTR_RR,     1,  MODE_MR,    REG_HL},
    [0x1F] = {INSTR_RR,     1,  MODE_R,     REG_A},

    [0x20] = {INSTR_SLA,    1,  MODE_R,     REG_B},
    [0x21] = {INSTR_SLA,    1,  MODE_R,     REG_C},
    [0x22] = {INSTR_SLA,    1,  MODE_R,     REG_D},
    [0x23] = {INSTR_SLA,    1,  MODE_R,     REG_E},
    [0x24] = {INSTR_SLA,    1,  MODE_R,     REG_H},
    [0x25] = {INSTR_SLA,    1,  MODE_R,     REG_L},
    [0x26] = {INSTR_SLA,    1,  MODE_MR,    REG_HL},
    [0x27] = {INSTR_SLA,    1,  MODE_R,     REG_A},
    [0x28] = {INSTR_SRA,    1,  MODE_R,     REG_B},
    [0x29] = {INSTR_SRA,    1,  MODE_R,     REG_C},
    [0x2A] = {INSTR_SRA,    1,  MODE_R,     REG_D},
    [0x2B] = {INSTR_SRA,    1,  MODE_R,     REG_E},
    [0x2C] = {INSTR_SRA,    1,  MODE_R,     REG_H},
    [0x2D] = {INSTR_SRA,    1,  MODE_R,     REG_L},
    [0x2E] = {INSTR_SRA,    1,  MODE_MR,    REG_HL},
    [0x2F] = {INSTR_SRA,    1,  MODE_R,     REG_A},

    [0x30] = {INSTR_SWAP,   1,  MODE_R,     REG_B},
    [0x31] = {INSTR_SWAP,   1,  MODE_R,     REG_C},
    [0x32] = {INSTR_SWAP,   1,  MODE_R,     REG_D},
    [0x33] = {INSTR_SWAP,   1,  MODE_R,     REG_E},
    [0x34] = {INSTR_SWAP,   1,  MODE_R,     REG_H},
    [0x35] = {INSTR_SWAP,   1,  MODE_R,     REG_L},
    [0x36] = {INSTR_SWAP,   1,  MODE_MR,    REG_HL},
    [0x37] = {INSTR_SWAP,   1,  MODE_R,     REG_A},
    [0x38] = {INSTR_SRL,    1,  MODE_R,     REG_B},
    [0x39] = {INSTR_SRL,    1,  MODE_R,     REG_C},
    [0x3A] = {INSTR_SRL,    1,  MODE_R,     REG_D},
    [0x3B] = {INSTR_SRL,    1,  MODE_R,     REG_E},
    [0x3C] = {INSTR_SRL,    1,  MODE_R,     REG_H},
    [0x3D] = {INSTR_SRL,    1,  MODE_R,     REG_L},
    [0x3E] = {INSTR_SRL,    1,  MODE_MR,    REG_HL},
    [0x3F] = {INSTR_SRL,    1,  MODE_R,     REG_A},
    [0x40] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 0},
    [0x41] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 0},
    [0x42] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 0},
    [0x43] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 0},
    [0x44] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 0},
    [0x45] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 0},
    [0x46] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 0},
    [0x47] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 0},
    [0x48] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 1},
    [0x49] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 1},
    [0x4A] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 1},
    [0x4B] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 1},
    [0x4C] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 1},
    [0x4D] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 1},
    [0x4E] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 1},
    [0x4F] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 1},

    [0x50] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 2},
    [0x51] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 2},
    [0x52] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 2},
    [0x53] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 2},
    [0x54] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 2},
    [0x55] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 2},
    [0x56] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 2},
    [0x57] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 2},
    [0x58] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 3},
    [0x59] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 3},
    [0x5A] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 3},
    [0x5B] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 3},
    [0x5C] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 3},
    [0x5D] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 3},
    [0x5E] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 3},
    [0x5F] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 3},

    [0x60] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 4},
    [0x61] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 4},
    [0x62] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 4},
    [0x63] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 4},
    [0x64] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 4},
    [0x65] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 4},
    [0x66] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 4},
    [0x67] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 4},
    [0x68] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 5},
    [0x69] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 5},
    [0x6A] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 5},
    [0x6B] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 5},
    [0x6C] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 5},
    [0x6D] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 5},
    [0x6E] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 5},
    [0x6F] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 5},

    [0x70] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 6},
    [0x71] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 6},
    [0x72] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 6},
    [0x73] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 6},
    [0x74] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 6},
    [0x75] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 6},
    [0x76] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 6},
    [0x77] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 6},
    [0x78] = {INSTR_BIT,    1,  MODE_R,     REG_B,  REG_NONE, 0, 7},
    [0x79] = {INSTR_BIT,    1,  MODE_R,     REG_C,  REG_NONE, 0, 7},
    [0x7A] = {INSTR_BIT,    1,  MODE_R,     REG_D,  REG_NONE, 0, 7},
    [0x7B] = {INSTR_BIT,    1,  MODE_R,     REG_E,  REG_NONE, 0, 7},
    [0x7C] = {INSTR_BIT,    1,  MODE_R,     REG_H,  REG_NONE, 0, 7},
    [0x7D] = {INSTR_BIT,    1,  MODE_R,     REG_L,  REG_NONE, 0, 7},
    [0x7E] = {INSTR_BIT,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 7},
    [0x7F] = {INSTR_BIT,    1,  MODE_R,     REG_A,  REG_NONE, 0, 7},

    [0x80] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 0},
    [0x81] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 0},
    [0x82] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 0},
    [0x83] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 0},
    [0x84] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 0},
    [0x85] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 0},
    [0x86] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 0},
    [0x87] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 0},
    [0x88] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 1},
    [0x89] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 1},
    [0x8A] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 1},
    [0x8B] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 1},
    [0x8C] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 1},
    [0x8D] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 1},
    [0x8E] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 1},
    [0x8F] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 1},

    [0x90] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 2},
    [0x91] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 2},
    [0x92] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 2},
    [0x93] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 2},
    [0x94] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 2},
    [0x95] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 2},
    [0x96] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 2},
    [0x97] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 2},
    [0x98] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 3},
    [0x99] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 3},
    [0x9A] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 3},
    [0x9B] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 3},
    [0x9C] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 3},
    [0x9D] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 3},
    [0x9E] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 3},
    [0x9F] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 3},

    [0xA0] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 4},
    [0xA1] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 4},
    [0xA2] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 4},
    [0xA3] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 4},
    [0xA4] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 4},
    [0xA5] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 4},
    [0xA6] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 4},
    [0xA7] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 4},
    [0xA8] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 5},
    [0xA9] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 5},
    [0xAA] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 5},
    [0xAB] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 5},
    [0xAC] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 5},
    [0xAD] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 5},
    [0xAE] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 5},
    [0xAF] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 5},

    [0xB0] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 6},
    [0xB1] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 6},
    [0xB2] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 6},
    [0xB3] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 6},
    [0xB4] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 6},
    [0xB5] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 6},
    [0xB6] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 6},
    [0xB7] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 6},
    [0xB8] = {INSTR_RES,    1,  MODE_R,     REG_B,  REG_NONE, 0, 7},
    [0xB9] = {INSTR_RES,    1,  MODE_R,     REG_C,  REG_NONE, 0, 7},
    [0xBA] = {INSTR_RES,    1,  MODE_R,     REG_D,  REG_NONE, 0, 7},
    [0xBB] = {INSTR_RES,    1,  MODE_R,     REG_E,  REG_NONE, 0, 7},
    [0xBC] = {INSTR_RES,    1,  MODE_R,     REG_H,  REG_NONE, 0, 7},
    [0xBD] = {INSTR_RES,    1,  MODE_R,     REG_L,  REG_NONE, 0, 7},
    [0xBE] = {INSTR_RES,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 7},
    [0xBF] = {INSTR_RES,    1,  MODE_R,     REG_A,  REG_NONE, 0, 7},

    [0xC0] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 0},
    [0xC1] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 0},
    [0xC2] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 0},
    [0xC3] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 0},
    [0xC4] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 0},
    [0xC5] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 0},
    [0xC6] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 0},
    [0xC7] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 0},
    [0xC8] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 1},
    [0xC9] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 1},
    [0xCA] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 1},
    [0xCB] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 1},
    [0xCC] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 1},
    [0xCD] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 1},
    [0xCE] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 1},
    [0xCF] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 1},

    [0xD0] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 2},
    [0xD1] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 2},
    [0xD2] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 2},
    [0xD3] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 2},
    [0xD4] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 2},
    [0xD5] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 2},
    [0xD6] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 2},
    [0xD7] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 2},
    [0xD8] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 3},
    [0xD9] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 3},
    [0xDA] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 3},
    [0xDB] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 3},
    [0xDC] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 3},
    [0xDD] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 3},
    [0xDE] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 3},
    [0xDF] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 3},

    [0xE0] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 4},
    [0xE1] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 4},
    [0xE2] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 4},
    [0xE3] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 4},
    [0xE4] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 4},
    [0xE5] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 4},
    [0xE6] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 4},
    [0xE7] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 4},
    [0xE8] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 5},
    [0xE9] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 5},
    [0xEA] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 5},
    [0xEB] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 5},
    [0xEC] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 5},
    [0xED] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 5},
    [0xEE] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 5},
    [0xEF] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 5},

    [0xF0] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 6},
    [0xF1] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 6},
    [0xF2] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 6},
    [0xF3] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 6},
    [0xF4] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 6},
    [0xF5] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 6},
    [0xF6] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 6},
    [0xF7] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 6},
    [0xF8] = {INSTR_SET,    1,  MODE_R,     REG_B,  REG_NONE, 0, 7},
    [0xF9] = {INSTR_SET,    1,  MODE_R,     REG_C,  REG_NONE, 0, 7},
    [0xFA] = {INSTR_SET,    1,  MODE_R,     REG_D,  REG_NONE, 0, 7},
    [0xFB] = {INSTR_SET,    1,  MODE_R,     REG_E,  REG_NONE, 0, 7},
    [0xFC] = {INSTR_SET,    1,  MODE_R,     REG_H,  REG_NONE, 0, 7},
    [0xFD] = {INSTR_SET,    1,  MODE_R,     REG_L,  REG_NONE, 0, 7},
    [0xFE] = {INSTR_SET,    1,  MODE_MR,    REG_HL, REG_NONE, 0, 7},
    [0xFF] = {INSTR_SET,    1,  MODE_R,     REG_A,  REG_NONE, 0, 7},
};

const INSTRUCTION map_instructions[] = {
    [0x00] = {INSTR_NOP, 1, MODE_NONE},
    [0x01] = {INSTR_LD, 3, MODE_R_D16, REG_BC, MEM_D16},
    [0x02] = {INSTR_LD, 1, MODE_MR_R, REG_BC, REG_A},
    [0x03] = {INSTR_INC, 1, MODE_R, REG_BC},
    [0x04] = {INSTR_INC, 1, MODE_R, REG_B},
    [0x05] = {INSTR_DEC, 1, MODE_R, REG_B},
    [0x06] = {INSTR_LD, 2, MODE_R_D8, REG_B, MEM_D8},
    [0x07] = {INSTR_RLCA, 1},
    [0x08] = {INSTR_LD, 3, MODE_A16_R, MEM_A16, REG_SP}, //a verifier avec MR
    [0x09] = {INSTR_ADD, 1, MODE_R_R, REG_HL, REG_BC},
    [0x0A] = {INSTR_LD, 1, MODE_R_MR, REG_A, REG_BC},
    [0x0B] = {INSTR_DEC, 1, MODE_R, REG_BC},
    [0x0C] = {INSTR_INC, 1, MODE_R, REG_C},
    [0x0D] = {INSTR_DEC, 1, MODE_R, REG_C},
    [0x0E] = {INSTR_LD, 2, MODE_R_D8, REG_C, MEM_D8},
    [0x0F] = {INSTR_RRCA, 1},

    [0x10] = {INSTR_STOP, 2},
    [0x11] = {INSTR_LD, 3, MODE_R_D16, REG_DE, MEM_D16},
    [0x12] = {INSTR_LD, 1, MODE_MR_R, REG_DE, REG_A},
    [0x13] = {INSTR_INC, 1, MODE_R, REG_DE},
    [0x14] = {INSTR_INC, 1, MODE_R, REG_D},
    [0x15] = {INSTR_DEC, 1, MODE_R, REG_D},
    [0x16] = {INSTR_LD, 2, MODE_R_D8, REG_D, MEM_D8},
    [0x17] = {INSTR_RLA, 1},
    [0x18] = {INSTR_JR, 2, MODE_R8, MEM_R8},
    [0x19] = {INSTR_ADD, 1, MODE_R_R, REG_HL, REG_DE},
    [0x1A] = {INSTR_LD, 1, MODE_R_MR, REG_A, REG_DE},
    [0x1B] = {INSTR_DEC, 1, MODE_R, REG_DE},
    [0x1C] = {INSTR_INC, 1, MODE_R, REG_E},
    [0x1D] = {INSTR_DEC, 1, MODE_R, REG_E},
    [0x1E] = {INSTR_LD, 2, MODE_R_D8, REG_E, MEM_D8},
    [0x1F] = {INSTR_RRA, 1},

    [0x20] = {INSTR_JR, 2, MODE_R8, MEM_R8, REG_NONE, (1 << COND_NZ)},
    [0x21] = {INSTR_LD, 3, MODE_R_D16, REG_HL, MEM_D16},
    [0x22] = {INSTR_LDI, 1, MODE_R_R, REG_HL, REG_A}, //check
    [0x23] = {INSTR_INC, 1, MODE_R, REG_HL},
    [0x24] = {INSTR_INC, 1, MODE_R, REG_H},
    [0x25] = {INSTR_DEC, 1, MODE_R, REG_H},
    [0x26] = {INSTR_LD, 2, MODE_R_D8, REG_H, MEM_D8},
    [0x27] = {INSTR_DAA, 1},
    [0x28] = {INSTR_JR, 2, MODE_R8, MEM_R8, REG_NONE, (1 << FLAG_Z)},
    [0x29] = {INSTR_ADD, 1, MODE_R_R, REG_HL, REG_HL},
    [0x2A] = {INSTR_LDI, 1, MODE_R_MR, REG_A, REG_HL}, //LDI a check
    [0x2B] = {INSTR_DEC, 1, MODE_R, REG_HL},
    [0x2C] = {INSTR_INC, 1, MODE_R, REG_L},
    [0x2D] = {INSTR_DEC, 1, MODE_R, REG_L},
    [0x2E] = {INSTR_LD, 2, MODE_R_D8, REG_L, MEM_D8},
    [0x2F] = {INSTR_CPL, 1},

    [0x30] = {INSTR_JR, 2, MODE_R8, MEM_R8, REG_NONE, (1 << COND_NC)},
    [0x31] = {INSTR_LD, 3, MODE_R_D16, REG_SP, MEM_D16},
    [0x32] = {INSTR_LDD, 1, MODE_R_R, REG_HL, REG_A}, //check
    [0x33] = {INSTR_INC, 1, MODE_R, REG_SP},
    [0x34] = {INSTR_INC, 1, MODE_MR, REG_HL},
    [0x35] = {INSTR_DEC, 1, MODE_MR, REG_HL},
    [0x36] = {INSTR_LD, 2, MODE_MR_D8, REG_HL, MEM_D8},
    [0x37] = {INSTR_SCF, 1},
    [0x38] = {INSTR_JR, 2, MODE_R8, MEM_R8, REG_NONE, FLAG_C},
    [0x39] = {INSTR_ADD, 1, MODE_R_R, REG_HL, REG_SP},
    [0x3A] = {INSTR_LDD, 1, MODE_R_MR, REG_A, REG_HL}, //check
    [0x3B] = {INSTR_DEC, 1, MODE_R, REG_SP},
    [0x3C] = {INSTR_INC, 1, MODE_R, REG_A},
    [0x3D] = {INSTR_DEC, 1, MODE_R, REG_A},
    [0x3E] = {INSTR_LD, 2, MODE_R_D8, REG_A, MEM_D8},
    [0x3F] = {INSTR_CCF, 1},

    [0x40] =  {INSTR_LD, 1, MODE_R_R,REG_B, REG_B},
    [0x41] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_C},
    [0x42] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_D},
    [0x43] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_E},
    [0x44] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_H},
    [0x45] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_L},
    [0x46] = {INSTR_LD, 1, MODE_R_MR, REG_B, REG_HL},
    [0x47] = {INSTR_LD, 1, MODE_R_R, REG_B, REG_A},
    [0x48] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_B},
    [0x49] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_C},
    [0x4A] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_D},
    [0x4B] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_E},
    [0x4C] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_H},
    [0x4D] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_L},
    [0x4E] = {INSTR_LD, 1, MODE_R_MR, REG_C, REG_HL},
    [0x4F] = {INSTR_LD, 1, MODE_R_R, REG_C, REG_A},

    [0x50] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_B},
    [0x51] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_C},
    [0x52] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_D},
    [0x53] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_E},
    [0x54] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_H},
    [0x55] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_L},
    [0x56] = {INSTR_LD, 1, MODE_R_MR, REG_D, REG_HL},
    [0x57] = {INSTR_LD, 1, MODE_R_R, REG_D, REG_A},
    [0x58] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_B},
    [0x59] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_C},
    [0x5A] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_D},
    [0x5B] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_E},
    [0x5C] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_H},
    [0x5D] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_L},
    [0x5E] = {INSTR_LD, 1, MODE_R_MR, REG_E, REG_HL},
    [0x5F] = {INSTR_LD, 1, MODE_R_R, REG_E, REG_A},

    [0x60] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_B},
    [0x61] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_C},
    [0x62] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_D},
    [0x63] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_E},
    [0x64] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_H},
    [0x65] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_L},
    [0x66] = {INSTR_LD, 1, MODE_R_MR, REG_H, REG_HL},
    [0x67] = {INSTR_LD, 1, MODE_R_R, REG_H, REG_A},
    [0x68] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_B},
    [0x69] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_C},
    [0x6A] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_D},
    [0x6B] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_E},
    [0x6C] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_H},
    [0x6D] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_L},
    [0x6E] = {INSTR_LD, 1, MODE_R_MR, REG_L, REG_HL},
    [0x6F] = {INSTR_LD, 1, MODE_R_R, REG_L, REG_A},

    [0x70] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_B},
    [0x71] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_C},
    [0x72] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_D},
    [0x73] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_E},
    [0x74] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_H},
    [0x75] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_L},
    [0x76] = {INSTR_HALT, 1},
    [0x77] = {INSTR_LD, 1, MODE_MR_R, REG_HL, REG_A},
    [0x78] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_B},
    [0x79] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_C},
    [0x7A] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_D},
    [0x7B] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_E},
    [0x7C] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_H},
    [0x7D] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_L},
    [0x7E] = {INSTR_LD, 1, MODE_R_MR, REG_A, REG_HL},
    [0x7F] = {INSTR_LD, 1, MODE_R_R, REG_A, REG_A},

    [0x80] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_B},
    [0x81] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_C},
    [0x82] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_D},
    [0x83] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_E},
    [0x84] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_H},
    [0x85] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_L},
    [0x86] = {INSTR_ADD, 1, MODE_R_MR, REG_A, REG_HL},
    [0x87] = {INSTR_ADD, 1, MODE_R_R, REG_A, REG_A},
    [0x88] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_B},
    [0x89] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_C},
    [0x8A] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_D},
    [0x8B] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_E},
    [0x8C] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_H},
    [0x8D] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_L},
    [0x8E] = {INSTR_ADC, 1, MODE_R_MR, REG_A, REG_HL},
    [0x8F] = {INSTR_ADC, 1, MODE_R_R, REG_A, REG_A},

    [0x90] = {INSTR_SUB, 1, MODE_R, REG_B},
    [0x91] = {INSTR_SUB, 1, MODE_R, REG_C},
    [0x92] = {INSTR_SUB, 1, MODE_R, REG_D},
    [0x93] = {INSTR_SUB, 1, MODE_R, REG_E},
    [0x94] = {INSTR_SUB, 1, MODE_R, REG_H},
    [0x95] = {INSTR_SUB, 1, MODE_R, REG_L},
    [0x96] = {INSTR_SUB, 1, MODE_MR, REG_HL},
    [0x97] = {INSTR_SUB, 1, MODE_R, REG_A},
    [0x98] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_B},
    [0x99] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_C},
    [0x9A] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_D},
    [0x9B] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_E},
    [0x9C] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_H},
    [0x9D] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_L},
    [0x9E] = {INSTR_SBC, 1, MODE_R_MR, REG_A, REG_HL},
    [0x9F] = {INSTR_SBC, 1, MODE_R_R, REG_A, REG_A},

    [0xA0] = {INSTR_AND, 1, MODE_R, REG_B},
    [0xA1] = {INSTR_AND, 1, MODE_R, REG_C},
    [0xA2] = {INSTR_AND, 1, MODE_R, REG_D},
    [0xA3] = {INSTR_AND, 1, MODE_R, REG_E},
    [0xA4] = {INSTR_AND, 1, MODE_R, REG_H},
    [0xA5] = {INSTR_AND, 1, MODE_R, REG_L},
    [0xA6] = {INSTR_AND, 1, MODE_MR, REG_HL},
    [0xA7] = {INSTR_AND, 1, MODE_R, REG_A},
    [0xA8] = {INSTR_XOR, 1, MODE_R, REG_B},
    [0xA9] = {INSTR_XOR, 1, MODE_R, REG_C},
    [0xAA] = {INSTR_XOR, 1, MODE_R, REG_D},
    [0xAB] = {INSTR_XOR, 1, MODE_R, REG_E},
    [0xAC] = {INSTR_XOR, 1, MODE_R, REG_H},
    [0xAD] = {INSTR_XOR, 1, MODE_R, REG_L},
    [0xAE] = {INSTR_XOR, 1, MODE_MR, REG_HL},
    [0xAF] = {INSTR_XOR, 1, MODE_R, REG_A},

    [0xB0] = {INSTR_OR, 1, MODE_R, REG_B},
    [0xB1] = {INSTR_OR, 1, MODE_R, REG_C},
    [0xB2] = {INSTR_OR, 1, MODE_R, REG_D},
    [0xB3] = {INSTR_OR, 1, MODE_R, REG_E},
    [0xB4] = {INSTR_OR, 1, MODE_R, REG_H},
    [0xB5] = {INSTR_OR, 1, MODE_R, REG_L},
    [0xB6] = {INSTR_OR, 1, MODE_MR, REG_HL},
    [0xB7] = {INSTR_OR, 1, MODE_R, REG_A},
    [0xB8] = {INSTR_CP, 1, MODE_R, REG_B},
    [0xB9] = {INSTR_CP, 1, MODE_R, REG_C},
    [0xBA] = {INSTR_CP, 1, MODE_R, REG_D},
    [0xBB] = {INSTR_CP, 1, MODE_R, REG_E},
    [0xBC] = {INSTR_CP, 1, MODE_R, REG_H},
    [0xBD] = {INSTR_CP, 1, MODE_R, REG_L},
    [0xBE] = {INSTR_CP, 1, MODE_MR, REG_HL},
    [0xBF] = {INSTR_CP, 1, MODE_R, REG_A},

    [0xC0] = {INSTR_RET, 1, MODE_NONE, REG_NONE, REG_NONE, (1 << COND_NZ)},
    [0xC1] = {INSTR_POP, 1, MODE_R, REG_BC},
    [0xC2] = {INSTR_JP, 3, MODE_A16, MEM_A16, REG_NONE, (1 << COND_NZ)},
    [0xC3] = {INSTR_JP, 3, MODE_A16, MEM_A16},
    [0xC4] = {INSTR_CALL, 3, MODE_A16, REG_NONE, REG_NONE, (1 << COND_NZ)},
    [0xC5] = {INSTR_PUSH, 1, MODE_R, REG_BC},
    [0xC6] = {INSTR_ADD, 2, MODE_R_D8, REG_A, MEM_D8},
    [0xC7] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0},
    [0xC8] = {INSTR_RET, 1, MODE_NONE, REG_NONE, REG_NONE, (1 << FLAG_Z)},
    [0xC9] = {INSTR_RET, 1},
    [0xCA] = {INSTR_JP, 3, MODE_A16, MEM_A16, REG_NONE, (1 << FLAG_Z)},
    [0xCB] = {INSTR_CB, 1, MODE_D8, MEM_D8},
    [0xCC] = {INSTR_CALL, 3, MODE_A16, MEM_A16, REG_NONE, (1 << FLAG_Z)},
    [0xCD] = {INSTR_CALL, 3, MODE_A16, MEM_A16},
    [0xCE] = {INSTR_ADC, 2, MODE_R_D8, REG_A, MEM_D8},
    [0xCF] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x08},

    [0xD0] = {INSTR_RET, 1, MODE_NONE, REG_NONE, REG_NONE, (1 << COND_NC)},
    [0xD1] = {INSTR_POP, 1, MODE_R, REG_DE},
    [0xD2] = {INSTR_JP, 3, MODE_A16, MEM_A16, REG_NONE, (1 << COND_NC)},
    [0xD3] = {INSTR_NONE},
    [0xD4] = {INSTR_CALL, 3, MODE_A16, MEM_A16, REG_NONE, (1 << COND_NC)},
    [0xD5] = {INSTR_PUSH, 1, MODE_R, REG_DE},
    [0xD6] = {INSTR_SUB, 2, MODE_D8, MEM_D8},
    [0xD7] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x10},
    [0xD8] = {INSTR_RET, 1, MODE_NONE, REG_NONE, REG_NONE, (1 << FLAG_C)},
    [0xD9] = {INSTR_RETI, 1},
    [0xDA] = {INSTR_JP, 3, MODE_A16, MEM_A16, REG_NONE, (1 << FLAG_C)},
    [0xDB] = {INSTR_NONE},
    [0xDC] = {INSTR_CALL, 3, MODE_A16, MEM_A16, REG_NONE, (1 << FLAG_C)},
    [0xDD] = {INSTR_NONE},
    [0xDE] = {INSTR_SBC, 2, MODE_R_D8, REG_A, MEM_D8},
    [0xDF] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x18},

    [0xE0] = {INSTR_LDH, 2, MODE_MR_R, MEM_A8, REG_A},
    [0xE1] = {INSTR_POP, 1, MODE_R, REG_HL},
    [0xE2] = {INSTR_LD, 2, MODE_MR_R, REG_C, REG_A},
    [0xE3] = {INSTR_NONE},
    [0xE4] = {INSTR_NONE},
    [0xE5] = {INSTR_PUSH, 1, MODE_R, REG_HL},
    [0xE6] = {INSTR_AND, 2, MODE_D8, MEM_D8},
    [0xE7] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x20},
    [0xE8] = {INSTR_ADD, 2, MODE_R_R8, REG_SP, MEM_R8},
    [0xE9] = {INSTR_JP, 1, MODE_MR, REG_HL},
    [0xEA] = {INSTR_LD, 3, MODE_MR_R, MEM_A16, REG_A},
    [0xEB] = {INSTR_NONE},
    [0xEC] = {INSTR_NONE},
    [0xED] = {INSTR_NONE},
    [0xEE] = {INSTR_XOR, 2, MODE_D8, MEM_D8},
    [0xEF] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x28},

    [0xF0] = {INSTR_LDH, 2, MODE_R_MR, REG_A, MEM_A8},
    [0xF1] = {INSTR_POP, 1, MODE_R, REG_AF},
    [0xF2] = {INSTR_LD, 2, MODE_R_MR, REG_A, REG_C},
    [0xF3] = {INSTR_DI, 1},
    [0xF4] = {INSTR_NONE},
    [0xF5] = {INSTR_PUSH, 1, MODE_R, REG_AF},
    [0xF6] = {INSTR_OR, 2, MODE_D8, MEM_D8},
    [0xF7] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x30},
    [0xF8] = {INSTR_LD, 2, MODE_R_R, REG_HL, REG_SP}, //check
    [0xF9] = {INSTR_LD, 1, MODE_R_R, REG_SP, REG_HL},
    [0xFA] = {INSTR_LD, 3, MODE_R_MR, REG_A, MEM_A16},
    [0xFB] = {INSTR_EI, 1},
    [0xFC] = {INSTR_NONE},
    [0xFD] = {INSTR_NONE},
    [0xFE] = {INSTR_CP, 2, MODE_D8, MEM_D8},
    [0xFF] = {INSTR_RST, 1, MODE_NONE, REG_NONE, REG_NONE, 0, 0x38}    
};

static INSTR_ARG createArg(void *data, size_t size) {
    INSTR_ARG tmp;
    tmp.data = data;
    tmp.size = size;
    return tmp;
}

INSTR_ARG getFirstArg(INSTRUCTION instruction) {
    if (instruction.mode >= MODE_R && instruction.mode <= MODE_R_MR) {
        switch (instruction.r1)
        {
        case REG_A:
            return createArg(&cpu.registers.a, 1);
        case REG_F:
            return createArg(&cpu.registers.flags, 1);
        case REG_AF:
            return createArg(&cpu.registers.af, 2);
        case REG_B:
            return createArg(&cpu.registers.b, 1);
        case REG_C:
            return createArg(&cpu.registers.c, 1);
        case REG_BC:
            return createArg(&cpu.registers.bc, 2);
        case REG_D:
            return createArg(&cpu.registers.d, 1);
        case REG_E:
            return createArg(&cpu.registers.e, 1);
        case REG_DE:
            return createArg(&cpu.registers.de, 2);
        case REG_H:
            return createArg(&cpu.registers.h, 1);
        case REG_L:
            return createArg(&cpu.registers.l, 1);
        case REG_HL:
            return createArg(cpu.registers.hl, 2);
        case REG_PC:
            return createArg(&cpu.registers.pc, 2);
        case REG_SP:
            return createArg(&cpu.registers.sp, 2);
        default:
            break;
        }
    } else {
        switch (instruction.r1)
        {
        case REG_A:
            return createArg(&ram.data[0xFF00 + read_reg(REG_A)], 1);
        case REG_F:
            return createArg(&ram.data[0xFF00 + read_reg(REG_F)], 1);
        case REG_AF:
            return createArg(&ram.data[read_reg(REG_AF)], 2);
        case REG_B:
            return createArg(&ram.data[0xFF00 + read_reg(REG_B)], 1);
        case REG_C:
            return createArg(&ram.data[0xFF00 + read_reg(REG_C)], 1);
        case REG_BC:
            return createArg(&ram.data[read_reg(REG_BC)], 2);
        case REG_D:
            return createArg(&ram.data[0xFF00 + read_reg(REG_D)], 1);
        case REG_E:
            return createArg(&ram.data[0xFF00 + read_reg(REG_E)], 1);
        case REG_DE:
            return createArg(&ram.data[read_reg(REG_DE)], 2);
        case REG_H:
            return createArg((&ram.data[0xFF00 + read_reg(REG_H)]), 1);
        case REG_L:
            return createArg((&ram.data[0xFF00 + read_reg(REG_L)]), 1);
        case REG_HL:
            return createArg(&ram.data[read_reg(REG_HL)], 2);
        case MEM_A8:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 1);
        case MEM_D8:
        case MEM_R8:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 1);
        case MEM_A16:
        case MEM_D16:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 2);
        default:
            break;
        }
    }
    return createArg(NULL, 0);
}

INSTR_ARG getSecondArg(INSTRUCTION instruction) {
    if (instruction.mode == MODE_R_R || instruction.mode == MODE_A16_R || instruction.mode == MODE_A8_R || instruction.mode == MODE_MR_R) {
        switch (instruction.r2)
        {
        case REG_A:
            return createArg(&cpu.registers.a, 1);
        case REG_F:
            return createArg(&cpu.registers.flags, 1);
        case REG_AF:
            return createArg(&cpu.registers.af, 2);
        case REG_B:
            return createArg(&cpu.registers.b, 1);
        case REG_C:
            return createArg(&cpu.registers.c, 1);
        case REG_BC:
            return createArg(&cpu.registers.bc, 2);
        case REG_D:
            return createArg(&cpu.registers.d, 1);
        case REG_E:
            return createArg(&cpu.registers.e, 1);
        case REG_DE:
            return createArg(&cpu.registers.de, 2);
        case REG_H:
            return createArg(&cpu.registers.h, 1);
        case REG_L:
            return createArg(&cpu.registers.l, 1);
        case REG_HL:
            return createArg(cpu.registers.hl, 2);
        case REG_PC:
            return createArg(&cpu.registers.pc, 2);
        case REG_SP:
            return createArg(&cpu.registers.sp, 2);
        default:
            break;
        }
    } else {
        switch (instruction.r2)
        {
        case REG_A:
            return createArg(&ram.data[0xFF00 + read_reg(REG_A)], 1);
        case REG_F:
            return createArg(&ram.data[0xFF00 + read_reg(REG_F)], 1);
        case REG_AF:
            return createArg(&ram.data[read_reg(REG_AF)], 2);
        case REG_B:
            return createArg(&ram.data[0xFF00 + read_reg(REG_B)], 1);
        case REG_C:
            return createArg(&ram.data[0xFF00 + read_reg(REG_C)], 1);
        case REG_BC:
            return createArg(&ram.data[read_reg(REG_BC)], 2);
        case REG_D:
            return createArg(&ram.data[0xFF00 + read_reg(REG_D)], 1);
        case REG_E:
            return createArg(&ram.data[0xFF00 + read_reg(REG_E)], 1);
        case REG_DE:
            return createArg(&ram.data[read_reg(REG_DE)], 2);
        case REG_H:
            return createArg((&ram.data[0xFF00 + read_reg(REG_H)]), 1);
        case REG_L:
            return createArg((&ram.data[0xFF00 + read_reg(REG_L)]), 1);
        case REG_HL:
            return createArg(&ram.data[read_reg(REG_HL)], 2);
        case MEM_A8:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 1);
        case MEM_D8:
        case MEM_R8:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 1);
        case MEM_A16:
        case MEM_D16:
            return createArg(&ram.data[read_reg(REG_PC) + 1], 2);
        default:
            break;
        }
    }
    return createArg(NULL, 0);
}

INSTRUCTION getCBInstruction(unsigned char opcode) {
    return map_cb_instructions[opcode];
}

INSTRUCTION getInstruction(unsigned char opcode) {
    return map_instructions[opcode];
}

int valid_conditions(INSTRUCTION instruction) {
    if ((instruction.conditionalFlag & (1 << COND_NC)) && cpu_get_flag(FLAG_C))
        return 0;
    if ((instruction.conditionalFlag & (1 << COND_NH)) && cpu_get_flag(FLAG_H))
        return 0;
    if ((instruction.conditionalFlag & (1 << COND_NN)) && cpu_get_flag(FLAG_N))
        return 0;
    if ((instruction.conditionalFlag & (1 << COND_NZ)) && cpu_get_flag(FLAG_Z))
        return 0;

    if ((instruction.conditionalFlag & (1 << FLAG_C)) && !cpu_get_flag(FLAG_C))
        return 0;
    if ((instruction.conditionalFlag & (1 << FLAG_H)) && !cpu_get_flag(FLAG_H))
        return 0;
    if ((instruction.conditionalFlag & (1 << FLAG_N)) && !cpu_get_flag(FLAG_N))
        return 0;
    if ((instruction.conditionalFlag & (1 << FLAG_Z)) && !cpu_get_flag(FLAG_Z))
        return 0;
    return 1;
}

static unsigned long count = 0;

void debug_pc(INSTRUCTION instruction) {
    if (!enable_print)
        return ;
    printf("\n%ld :\n", count++);
    printf("%s", ANSI_GREEN);
    for (int i = 0; i < (instruction.size ? instruction.size : 1); i++)
        printf("%02hhX ", read_mem(read_reg(REG_PC) + 1));
    printf("%s\nPC: 0x%02hX\tAF: %04hX\033[0m\tBC: %04hX\tDE: %04hX\tHL: %04hX\tSP: %04hX\tLY: %02hX\tFLAGS: %c%c%c%c\tCondition: %s%c%c%c%c%c%s\n",
        ANSI_RESET,
        cpu.registers.pc,
        *(unsigned short*)cpu.registers.af,
        *(unsigned short*)cpu.registers.bc,
        *(unsigned short*)cpu.registers.de,
        *(unsigned short*)cpu.registers.hl,
        cpu.registers.sp,
        io_data[0x44],
        (cpu.registers.flags & (1 << FLAG_Z)) ? 'Z' : '-',
        (cpu.registers.flags & (1 << FLAG_N)) ? 'N' : '-',
        (cpu.registers.flags & (1 << FLAG_H)) ? 'H' : '-',
        (cpu.registers.flags & (1 << FLAG_C)) ? 'C' : '-',
        instruction.conditionalFlag <= (1 << 3) ? (instruction.conditionalFlag != 0 ? ANSI_RED : "") : ANSI_GREEN,
        instruction.conditionalFlag? '\0' : '-',
        (instruction.conditionalFlag & (1 << FLAG_Z)) || (instruction.conditionalFlag & (1 << COND_NZ)) ? 'Z' : '\0',
        (instruction.conditionalFlag & (1 << FLAG_N)) || (instruction.conditionalFlag & (1 << COND_NN)) ? 'N' : '\0',
        (instruction.conditionalFlag & (1 << FLAG_H)) || (instruction.conditionalFlag & (1 << COND_NH)) ? 'H' : '\0',
        (instruction.conditionalFlag & (1 << FLAG_C)) || (instruction.conditionalFlag & (1 << COND_NC)) ? 'C' : '\0',
        ANSI_RESET);

    // if (cpu.registers.sp < 0xDFFF) {
    //     printf("\tStack : ");
    //     for (int i = 0; i + cpu.registers.sp < 0xDFFF; i += 2) {
    //         printf("%04X ", read_mem16(cpu.registers.sp + i));
    //     }
    //     printf("\n");
    // }
}

void instruction_process(INSTRUCTION instruction) {

    INSTR_ARG firstArg = getFirstArg(instruction);
    INSTR_ARG secondArg = getSecondArg(instruction);
    debug_pc(instruction);
    usleep(1000 * 30);

    if (instruction.instruction == INSTR_NONE) {
        printf("NONE INSTRUCTION : error\n");
        set_reg(REG_PC, read_reg(REG_PC) + 1);
        return ;
    }

    if (!valid_conditions(instruction)) {
        GO_NEXT_INSTR
        dbg_update();
        dbg_print();
        return ;
    }

    map_process[instruction.instruction](instruction, firstArg, secondArg);
    dbg_update();
    dbg_print();
}