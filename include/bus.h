#ifndef BUS_H
#define BUS_H

void *get_mem_addr(unsigned short addr);

unsigned char &read_mem(unsigned short addr);
void write_mem(unsigned short addr, unsigned char val);

unsigned short read_mem16(unsigned short addr);
void write_mem16(unsigned short addr, unsigned short val);


#endif