#ifndef RAM_H
#define RAM_H

typedef struct {
    unsigned char data[0xFFFF];
} RAM;

extern RAM ram;

void push_stack(unsigned short data);
unsigned short pop_stack();


#endif