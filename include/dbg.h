#ifndef DBG_H
#define DBG_H

#include <stdio.h>
#include <vector>

extern char enable_print;

extern const std::vector<int> breakpoints;

void dbg_print();
void dbg_update();

#endif