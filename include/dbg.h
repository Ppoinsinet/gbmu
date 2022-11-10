#ifndef DBG_H
#define DBG_H

#include <stdio.h>

extern char enable_print;

extern unsigned short breakpoints[];

void dbg_print();
void dbg_update();

#endif