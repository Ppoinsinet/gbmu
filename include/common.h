#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_IMPL {printf("Not implemented\n"); exit(1);}

#define ERROR(str) {printf("Error: %s\n", str ? str : ""); exit(3);}

#define ANSI_GREEN "\033[0;32m"
#define ANSI_RED "\033[0;31m"
#define ANSI_RESET "\033[0;0m"

#define SWAP(val)(((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8))

#define IS_BETWEEN(min, val, max)(val < min ? 0 : (val > max ? 0 : 1))

#endif