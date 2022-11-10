#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <common.h>

typedef struct {
    const char *size;
} RAM_INFO;

typedef struct {
    const char *size;
    int n_banks;
} ROM_INFO;

typedef struct {
    unsigned long size;
    char *data;
    const char *title;
    const char *licensee;
    char CGB;
    ROM_INFO rom_info;
    RAM_INFO ram_info;
    const char *type;
} CARTRIDGE;

#define CARTRIDGE_ENTRYPOINT 0x100
#define CARTRIDGE_OLD_LICENSEE 0x14B
#define CARTRIDGE_NEW_LICENSEE 0x144
#define CARTRIDGE_CGB_FLAG 0x143
#define CARTRIDGE_TITLE 0x134

extern const char *Licensee[];

extern const char* cartridge_type[];

extern const ROM_INFO rom_info[];
extern RAM_INFO ram_info[];
extern CARTRIDGE cart;

#endif