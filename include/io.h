#ifndef IO_H
#define IO_H

#include <common.h>

extern unsigned char io_data[0xFF];

unsigned char read_io(unsigned short addr);
void write_io(unsigned short addr, unsigned char val);

#endif